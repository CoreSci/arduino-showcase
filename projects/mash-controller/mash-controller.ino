/*
 * Mash Controller
 * ---------------
 * Problem: hold a brewing mash near a target temperature for a fixed number of
 * cycles, without someone watching the pot and switching the heater by hand.
 *
 * How it works (3-state machine):
 *   state1 - heat-up: heater + pump ON until the sensor reading passes criticalTemp.
 *   state2 - mash hold: each 1 s cycle toggles the heater around criticalTemp while
 *            the pump keeps circulating; after `max` cycles, go to state3.
 *   state3 - done: heater and pump OFF, nothing more to do.
 *
 * Hardware: analog temperature sensor on A0, heater relay on D4, pump relay on D5.
 * The raw reading is scaled (x100) and compared with criticalTemp. Use the
 * calibration-sensor sketch to find the raw value that matches your target temperature.
 */

// Relay outputs: heater on D4, circulation pump on D5
#define H 4
#define HeaterOn  digitalWrite (H,1);
#define HeaterOff digitalWrite (H,0);
#define P 5
#define PumpOn  digitalWrite (P,1);
#define PumpOff digitalWrite (P,0);

int sensorPin = A0;                         // analog temperature sensor
float sensorValue = analogRead(sensorPin)*100;
int timer = 0;                              // number of hold cycles completed
int max = 15;      // hold cycles before finishing (3600 cycles = 1 hour at 1 s/cycle)
int calibTime = 1000; // cycle period: 1 s
int criticalTemp = 500;                     // scaled set-point (from calibration)

void setup() {
  pinMode (H,OUTPUT);
  pinMode (P,OUTPUT);
  Serial.begin(9600);                       // log readings over USB serial

}

enum {state1, state2, state3} state=state1;
void loop() {
  switch(state) {
    // --- Heat-up: run the heater until the set-point is first reached ---
    case state1:
      sensorValue = analogRead(sensorPin)*100;
      if (sensorValue <= criticalTemp) {HeaterOn; PumpOn; Serial.print("heater ON temp : "); Serial.println(sensorValue); delay(calibTime); state = state1;}
      if (sensorValue > criticalTemp) {HeaterOff; PumpOn; Serial.print("heater OFF temp : "); Serial.println(sensorValue); delay(calibTime); state = state2;}
      break;

     // --- Mash hold: on/off regulation around the set-point for `max` cycles ---
     case state2:
      sensorValue = analogRead(sensorPin)*100;
      timer++;
      Serial.print("cycle : "); Serial.println(timer);
      if (sensorValue <= criticalTemp and timer < max) {HeaterOn; PumpOn; Serial.print("mash heater ON temp : "); Serial.println(sensorValue); delay(calibTime); state = state2;}
      if (sensorValue > criticalTemp and timer < max) {HeaterOff; PumpOn; Serial.print("mash heater OFF temp : "); Serial.println(sensorValue); delay(calibTime); state = state2;}
      if (timer == max) {HeaterOff; PumpOff; Serial.println("Completed!"); state = state3;}
      break;

     // --- Done: everything off, idle ---
     case state3:
      break;
  }
}
