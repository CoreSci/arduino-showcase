/*
 * System Controller (v2)
 * ----------------------
 * Problem: control a small heating/circulation rig (heater, pump and two spare
 * outputs) from a keypad, show its status on an LCD, and optionally report
 * telemetry to a host computer (e.g. a Raspberry Pi) over serial.
 *
 * Hardware: 4x4 matrix keypad (D2-D9), two DS18B20 temperature probes on a
 * OneWire bus (D10), 20x4 I2C LCD at 0x27, 4-channel relay board on A0-A3
 * (active LOW), and a mode switch on D11.
 *
 * Keypad map (only when the mode switch is OFF / manual):
 *   *  -> read both probes, update the LCD and send one CSV telemetry line
 *   A  -> toggle heater (relay 1)      B -> toggle pump (relay 2)
 *   C  -> toggle relay 3 (spare)       D -> toggle relay 4 (spare)
 *   #  -> automatic hold: keep probe 1 at ~50 C for 100 readings, then exit
 * With the mode switch ON, it streams telemetry continuously.
 *
 * Serial telemetry format (9600 baud): T1,T2,R1,R2,R3,R4
 * (relay values: 0 = ON, 1 = OFF). raspberry-pi/serial_monitor.py reads this line.
 */
#include <Keypad.h>

// ---- 4x4 keypad layout and wiring ----
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


// ---- Temperature probes (DS18B20 on OneWire) and 20x4 I2C LCD ----
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
 
// Data wire is plugged into pin 10 on the Arduino
#define ONE_WIRE_BUS 10
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// ---- Relay outputs (active LOW: writing LOW switches the load ON) ----
// RELAY1 = heater, RELAY2 = pump, RELAY3/RELAY4 = spare outputs
int RELAY1 = A0;
int RELAY2 = A1;
int RELAY3 = A2;
int RELAY4 = A3;

// Buffers used to build the CSV telemetry line
String a;
String b;
String c;
String d;
String e;
String f;
String g;

// Mode switch: 1 = stream telemetry continuously, 0 = keypad control
#define Switch 11

int StateSwitch;

// Number of readings taken during the automatic hold (state7)
int counter = 0;

void setup(void)
{
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  // All relays OFF at power-up
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  pinMode(Switch, INPUT);
  
  sensors.begin();
  //add serial init
  Serial.begin(9600);

  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight 
  
  lcd.setCursor ( 0, 1 );
  lcd.print("HEATER : OFF     ");
  lcd.setCursor ( 0, 2 );
  lcd.print("PUMP : OFF   ");

  StateSwitch = digitalRead(Switch);
  lcd.setCursor ( 0, 3 );
  lcd.print(StateSwitch);
}
 
// State machine: state1 = wait for input, state2 = telemetry, state3-6 = toggle relay 1-4,
// state7 = automatic temperature hold
enum {state1, state2, state3, state4, state5, state6, state7} state=state1;
void loop(void)
{
  switch(state) {

    // --- Automatic hold: bang-bang control of the heater around 50 C ---
    case state7:

      lcd.setCursor ( 2, 3 );
      lcd.print(counter);
      
      sensors.requestTemperatures();
      lcd.setCursor ( 0, 0 );
      lcd.print("T1: ");
      lcd.setCursor ( 4, 0 );
      lcd.print(sensors.getTempCByIndex(0));
      lcd.setCursor ( 11 , 0 );
      lcd.print("T2: ");
      lcd.setCursor ( 15 , 0 );
      lcd.print(sensors.getTempCByIndex(1));

      if (sensors.getTempCByIndex(0) >= 50.00) {
        counter = counter + 1;
      }
      
      if ((sensors.getTempCByIndex(0) < 50.00) && (digitalRead(RELAY1) == 1)) {
        digitalWrite(RELAY1, LOW);
        lcd.setCursor ( 0, 1 );
        lcd.print("HEATER : ON    ");
      }

      if ((sensors.getTempCByIndex(0) > 50.00) && (digitalRead(RELAY1) == 0)) {
        digitalWrite(RELAY1, HIGH);
        lcd.setCursor ( 0, 1 );
        lcd.print("HEATER : OFF   ");
      }

      if (counter == 100) {
        state = state1;
        counter = 0;
        lcd.setCursor ( 2, 3 );
        lcd.print("Exit");
      }

      else {
        state = state7;
      }
      
      break;
    
    // --- Toggle relay 4 (spare) ---
    case state6:

      if (digitalRead(RELAY4) == 1) {
        digitalWrite(RELAY4, LOW);
      }
      
      else {
        digitalWrite(RELAY4, HIGH);
      }
      
      state = state1;
      break;

    // --- Toggle relay 3 (spare) ---
    case state5:

      if (digitalRead(RELAY3) == 1) {
        digitalWrite(RELAY3, LOW);
      }
      
      else {
        digitalWrite(RELAY3, HIGH);
      }
      
      state = state1;
      break;
    
    // --- Toggle pump (relay 2) ---
    case state4:

      if (digitalRead(RELAY2) == 1) {
        digitalWrite(RELAY2, LOW);
        lcd.setCursor ( 0, 2 );
        lcd.print("PUMP : ON    ");
      }
      
      else {
        digitalWrite(RELAY2, HIGH);
        lcd.setCursor ( 0, 2 );
        lcd.print("PUMP : OFF   ");
      }
      
      state = state1;
      break;
    
    // --- Toggle heater (relay 1) ---
    case state3:
    
      if (digitalRead(RELAY1) == 1) {
        digitalWrite(RELAY1, LOW);
        lcd.setCursor ( 0, 1 );
        lcd.print("HEATER : ON    ");
      }
      
      else {
        digitalWrite(RELAY1, HIGH);
        lcd.setCursor ( 0, 1 );
        lcd.print("HEATER : OFF   ");
      }
      
      state = state1;
      break;
    
    // --- Read probes, refresh LCD, send CSV telemetry: T1,T2,R1,R2,R3,R4 ---
    case state2:
      
      sensors.requestTemperatures();
      
      lcd.setCursor ( 0, 0 );
      lcd.print("T1: ");
      lcd.setCursor ( 4, 0 );
      lcd.print(sensors.getTempCByIndex(0));
      lcd.setCursor ( 11 , 0 );
      lcd.print("T2: ");
      lcd.setCursor ( 15 , 0 );
      lcd.print(sensors.getTempCByIndex(1));

      a = String(sensors.getTempCByIndex(0));
      b = String(sensors.getTempCByIndex(1));
      c = String(digitalRead(RELAY1));
      d = String(digitalRead(RELAY2));
      e = String(digitalRead(RELAY3));
      f = String(digitalRead(RELAY4));

      g = a+","+b+","+c+","+d+","+e+","+f;
      
      Serial.println(g);
      
      state = state1;
      break;
    
    // --- Idle: dispatch on the mode switch / keypad ---
    case state1:
      char customKey = customKeypad.getKey();
      
      if (StateSwitch == 1) {lcd.setCursor ( 0, 3 ); state = state2;}
      if (customKey == '*' && StateSwitch == 0) {lcd.setCursor ( 0, 3 ); state = state2;}
      if (customKey == 'A' && StateSwitch == 0) {lcd.setCursor ( 0, 3 ); state = state3;}
      if (customKey == 'B' && StateSwitch == 0) {lcd.setCursor ( 0, 3 ); state = state4;}
      if (customKey == 'C' && StateSwitch == 0) {lcd.setCursor ( 0, 3 ); state = state5;}
      if (customKey == 'D' && StateSwitch == 0) {lcd.setCursor ( 0, 3 ); state = state6;}
      if (customKey == '#' && StateSwitch == 0) {lcd.setCursor ( 0, 3 ); state = state7;}
      break;
      }
}
