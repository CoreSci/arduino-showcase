/*
 * Lesson 3 - Magic knob light meter  (Le bouton magique)
 * Goal: turn the knob and watch more and more LEDs light up.
 * Big idea: a sensor turns the real world into a number (0 to 1023),
 *           and the program decides what to do with that number.
 *
 * Wiring: LEDs 1-4 on D4-D7 (each with a 220 ohm resistor to GND),
 *         potentiometer outer legs to 5V and GND, middle leg to A0.
 */

const int KNOB_PIN = A0;
const int LED_PINS[4] = {4, 5, 6, 7};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  int knob = analogRead(KNOB_PIN);            // 0 (turned left) .. 1023 (turned right)
  int ledsOn = map(knob, 0, 1023, 0, 4);      // turn that into 0, 1, 2, 3 or 4 LEDs

  for (int i = 0; i < 4; i++) {
    // LED number i is on if it is one of the first "ledsOn" LEDs
    digitalWrite(LED_PINS[i], i < ledsOn ? HIGH : LOW);
  }

  Serial.print("Bouton : ");
  Serial.print(knob);
  Serial.print("   lumieres : ");
  Serial.println(ledsOn);
  delay(100);                                 // check the knob 10 times per second
}
