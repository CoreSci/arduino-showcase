/*
 * Lesson 1 - Hello, light!  (Bonjour, lumiere !)
 * Goal: make one LED blink, then change how fast it blinks.
 * Big idea: a program is a list of steps the computer follows in order,
 *           over and over again (the loop).
 *
 * Wiring: LED 1 on D4 (through a 220 ohm resistor to GND).
 * Inspired by the Arduino "Blink" example (public domain).
 */

const int LED_PIN = 4;         // the LED is plugged into pin 4
int blinkTime = 1000;          // how long the light stays on (and off), in milliseconds

void setup() {
  pinMode(LED_PIN, OUTPUT);    // pin 4 will send power OUT to the LED
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // light ON
  delay(blinkTime);            // wait (1000 ms = 1 second)
  digitalWrite(LED_PIN, LOW);  // light OFF
  delay(blinkTime);            // wait again, then loop() starts over
}
