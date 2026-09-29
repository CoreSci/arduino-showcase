/*
 * Lesson 8 - Robot turn signals  (Les clignotants du robot)
 * Goal: the robot's "blinkers". Button 1 flashes the LEFT lights, button 2 the RIGHT
 *       lights, and both buttons together turn on the hazard lights (all four flash).
 * Big idea: cause and effect. Each input (button) makes a different output (lights).
 *
 * Wiring: button 1 (left) D2 -> GND, button 2 (right) D3 -> GND,
 *         left lights = LEDs on D4 and D5, right lights = LEDs on D6 and D7 (220 ohm each).
 */

const int LEFT_BUTTON = 2;
const int RIGHT_BUTTON = 3;
const int LEFT_LEDS[2] = {4, 5};
const int RIGHT_LEDS[2] = {6, 7};
const int BLINKS = 3;                 // how many times the signal flashes
const int SPEED = 300;                // milliseconds on, then off

// Flash the chosen side(s) a few times.
void blinkSides(bool left, bool right) {
  for (int n = 0; n < BLINKS; n++) {
    for (int i = 0; i < 2; i++) {
      if (left)  digitalWrite(LEFT_LEDS[i], HIGH);
      if (right) digitalWrite(RIGHT_LEDS[i], HIGH);
    }
    delay(SPEED);
    for (int i = 0; i < 2; i++) {
      digitalWrite(LEFT_LEDS[i], LOW);
      digitalWrite(RIGHT_LEDS[i], LOW);
    }
    delay(SPEED);
  }
}

void setup() {
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  for (int i = 0; i < 2; i++) {
    pinMode(LEFT_LEDS[i], OUTPUT);
    pinMode(RIGHT_LEDS[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  bool left = digitalRead(LEFT_BUTTON) == LOW;
  bool right = digitalRead(RIGHT_BUTTON) == LOW;
  if (left || right) {
    delay(80);                        // give a moment to press both buttons together
    left = left || digitalRead(LEFT_BUTTON) == LOW;
    right = right || digitalRead(RIGHT_BUTTON) == LOW;
    if (left && right) Serial.println("Feux de detresse !");
    else if (left)     Serial.println("A gauche !");
    else               Serial.println("A droite !");
    blinkSides(left, right);
  }
}
