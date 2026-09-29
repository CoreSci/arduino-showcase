/*
 * Mash Controller: sensor calibration helper
 * ------------------------------------------
 * Problem: the mash controller compares a raw analog reading with a set-point,
 * so you first need to know which raw value matches the target temperature.
 *
 * How it works: streams the raw analog value from the temperature sensor over
 * serial every 100 ms. Put the sensor in water next to a reference thermometer,
 * note the raw value at your target temperature, and use it (scaled) as
 * `criticalTemp` in mash-controller.ino.
 *
 * Hardware: sensor on A1. The heater (D4) and pump (D5) pins are configured
 * but stay off during calibration.
 */

// Same relay pin map as the main controller (unused while calibrating)
#define H 4
#define HeaterOn  digitalWrite (H,1);
#define HeaterOff digitalWrite (H,0);
#define P 5
#define PumpOn  digitalWrite (P,1);
#define PumpOff digitalWrite (P,0);

int sensorPin = A1;
int sensorValue = 0;
int timer = 0;
int max = 10;      // (unused here) cycle count shared with the main controller
int calibTime = 1000; // (unused here) 1 s cycle period

void setup() {
  pinMode (H,OUTPUT);
  pinMode (P,OUTPUT);
  Serial.begin(9600);

}

void loop() {
  sensorValue = analogRead(sensorPin);      // raw 0-1023 reading
  Serial.println(sensorValue);              // open the Serial Monitor/Plotter to read it
  delay(100);
}
