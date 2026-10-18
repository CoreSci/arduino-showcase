/*
 * Concepts 1a - Traffic light as an if/else chain
 * -----------------------------------------------
 * The starting point: a three-state traffic light (green -> yellow -> red)
 * written as a chain of if/else tests. It works, and for three states it's
 * perfectly readable. Compare it with ../traffic-light-switch/, which adds a
 * pedestrian button and a fourth state, and see the README for why the
 * switch/case form scales better.
 *
 * Non-blocking: no delay(). loop() runs thousands of times per second and
 * checks how long the current state has lasted with millis(), so the board
 * stays free to read buttons or sensors at the same time.
 *
 * Wiring: red LED D4, yellow LED D5, green LED D6, each through 220 ohm to GND.
 */

const int RED_PIN = 4;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;

const unsigned long GREEN_MS = 5000;
const unsigned long YELLOW_MS = 2000;
const unsigned long RED_MS = 5000;

// States as plain numbers: 0 = green, 1 = yellow, 2 = red.
// Nothing stops a typo like "state = 3" from compiling.
int state = 0;
unsigned long stateStart = 0;           // millis() when the current state began

void setLights(bool red, bool yellow, bool green) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  Serial.begin(9600);
  setLights(false, false, true);
  stateStart = millis();
  Serial.println("state 0: green");
}

void loop() {
  unsigned long inState = millis() - stateStart;   // subtraction stays correct when millis() wraps

  // Each branch has to test the state AND its own exit condition,
  // and has to remember to set the lights, reset the timer and log.
  if (state == 0 && inState >= GREEN_MS) {
    state = 1;
    setLights(false, true, false);
    stateStart = millis();
    Serial.println("state 1: yellow");
  } else if (state == 1 && inState >= YELLOW_MS) {
    state = 2;
    setLights(true, false, false);
    stateStart = millis();
    Serial.println("state 2: red");
  } else if (state == 2 && inState >= RED_MS) {
    state = 0;
    setLights(false, false, true);
    stateStart = millis();
    Serial.println("state 0: green");
  }
}
