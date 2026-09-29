/*
 * Lesson 6 - Copy my timing  (Copie mon temps !)
 * Goal: hold button 1 for as long as you like; the light stays on while you hold it.
 *       Then press button 2: the light copies EXACTLY the same length of time.
 * Big idea: the Arduino has a stopwatch (millis) and a memory (a variable).
 *
 * Wiring: button 1 D2 -> GND, button 2 D3 -> GND, LED 1 on D4 (220 ohm to GND).
 */

const int RECORD_BUTTON = 2;
const int PLAY_BUTTON = 3;
const int LED_PIN = 4;

unsigned long recorded = 0;          // the remembered time, in milliseconds

void setup() {
  pinMode(RECORD_BUTTON, INPUT_PULLUP);
  pinMode(PLAY_BUTTON, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Tiens le bouton 1, puis appuie sur le bouton 2 !");
}

void loop() {
  // --- Record: measure how long button 1 is held down ---
  if (digitalRead(RECORD_BUTTON) == LOW) {
    unsigned long start = millis();              // stopwatch: start
    digitalWrite(LED_PIN, HIGH);
    while (digitalRead(RECORD_BUTTON) == LOW) { } // wait while the finger holds the button
    recorded = millis() - start;                 // stopwatch: stop
    digitalWrite(LED_PIN, LOW);
    Serial.print("J'ai retenu : ");
    Serial.print(recorded / 1000.0, 1);          // show it in seconds, e.g. 2.4
    Serial.println(" secondes");
    delay(50);
  }

  // --- Play: light up for exactly the recorded time ---
  if (digitalRead(PLAY_BUTTON) == LOW && recorded > 0) {
    delay(50);
    while (digitalRead(PLAY_BUTTON) == LOW) { }  // wait for release, then play
    digitalWrite(LED_PIN, HIGH);
    delay(recorded);
    digitalWrite(LED_PIN, LOW);
    Serial.println("Copie terminee !");
  }
}
