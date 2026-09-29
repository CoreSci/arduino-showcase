/*
 * System Controller (v1, archived)
 * --------------------------------
 * The first iteration of the system controller: kept to show how the design evolved.
 * It continuously displays probe 1's temperature. Any key press switches all
 * four relays ON for 5 seconds, shows which key was pressed, then switches them OFF.
 * v2 (../../system-controller.ino) replaced this with a keypad-driven state
 * machine, per-relay toggles, an automatic temperature hold and serial telemetry.
 *
 * Hardware: same as v2 (keypad D2-D9, DS18B20 on D10, 20x4 I2C LCD, relays A0-A3).
 */
#include <Keypad.h>

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

int RELAY1 = A0;
int RELAY2 = A1;
int RELAY3 = A2;
int RELAY4 = A3;
 
void setup(void)
{
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  sensors.begin();

  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight 
  
  lcd.setCursor ( 0, 0 );
  lcd.print("TEMP : 00.00 oC");
  lcd.setCursor ( 0, 1 );
  lcd.print("PUMP : OFF     ");
  lcd.setCursor ( 0, 2 );
  lcd.print("HEATER : OFF   ");
  lcd.setCursor ( 0, 3 );
  lcd.print("E3:OFF | E4:OFF");
}
 
 
// Main loop: show the temperature, then run a 5-second relay test when a key is pressed
void loop(void)
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  lcd.setCursor ( 0, 0 );
  lcd.print("TEMP : ");
  lcd.setCursor ( 7, 0 );
  lcd.print(sensors.getTempCByIndex(0));
  lcd.setCursor ( 12, 0 );
  lcd.print(" oC");

  char customKey = customKeypad.getKey();
  
  // Key pressed: switch all relays ON (active LOW), wait 5 s, then OFF
  if (customKey){
    
    digitalWrite(RELAY1, LOW);
    lcd.setCursor ( 0, 1 );
    lcd.print("PUMP : ON      ");
    
    digitalWrite(RELAY2, LOW);
    lcd.setCursor ( 0, 2 );
    lcd.print("HEATER : ON    ");
    
    digitalWrite(RELAY3, LOW);
    lcd.setCursor ( 0, 3 );
    lcd.print("E3:ON | E4:OFF ");
    
    digitalWrite(RELAY4, LOW);
    lcd.setCursor ( 0, 3 );
    lcd.print("customKey  : ");
    
    //lcd.print("E3:ON  |  E4:ON");
    
    lcd.setCursor ( 13, 3 );
    lcd.print(customKey);

    delay(5000);

    digitalWrite(RELAY1, HIGH);
    lcd.setCursor ( 0, 1 );
    lcd.print("PUMP : OFF     ");
    
    digitalWrite(RELAY2, HIGH);
    lcd.setCursor ( 0, 2 );
    lcd.print("HEATER : OFF   ");
  
    digitalWrite(RELAY3, HIGH);
    lcd.setCursor ( 0, 3 );
    lcd.print("E3:OFF | E4:ON ");
    
    digitalWrite(RELAY4, HIGH);
    lcd.setCursor ( 0, 3 );
    lcd.print("E3:OFF | E4:OFF");
    }
}
