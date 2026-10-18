/*
 * Concepts 1b - Traffic light with a pedestrian button, as a switch/case state machine
 * ------------------------------------------------------------------------------------
 * Four states instead of three, plus an input event:
 *
 *   GREEN --(time up, or button after the minimum green)--> YELLOW
 *   YELLOW --(time up)--> RED
 *   RED ----(time up; a request is waiting)--> WALK
 *   RED ----(time up; no request)-----------> GREEN
 *   WALK ---(time up)--> GREEN
 *
 * The structure that makes it scale:
 *   * an enum names the states, so the compiler rejects a state that doesn't exist;
 *   * enterState() holds every state's entry action (lights, timer, log) in one place;
 *   * loop() has one case per state, and each case only decides *when to leave*.
 * Adding a fifth state means one enum value, one entry in enterState() and one case.
 *
 * Wiring: red D4, yellow D5, green D6, walk (white or blue) D7, each LED through
 *         220 ohm to GND. Push-button between D2 and GND (internal pull-up).
 */

const int RED_PIN = 4;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;
const int WALK_PIN = 7;
const int BUTTON_PIN = 2;

const unsigned long MIN_GREEN_MS = 3000;   // green always lasts at least this long
const unsigned long GREEN_MS = 10000;      // ...and at most this long without a request
const unsigned long YELLOW_MS = 2000;
const unsigned long RED_MS = 2000;         // all-red gap before walk or green
const unsigned long WALK_MS = 6000;

enum State { GREEN, YELLOW, RED, WALK };

State state;
unsigned long stateStart = 0;
bool walkRequested = false;               // latched: a press is remembered until served

void setLights(bool red, bool yellow, bool green, bool walk) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(WALK_PIN, walk ? HIGH : LOW);
}

// Entry actions: everything that happens once, on the way into a state.
void enterState(State next) {
  state = next;
  stateStart = millis();
  switch (state) {
    case GREEN:  setLights(false, false, true,  false); Serial.println("GREEN");  break;
    case YELLOW: setLights(false, true,  false, false); Serial.println("YELLOW"); break;
    case RED:    setLights(true,  false, false, false); Serial.println("RED");    break;
    case WALK:   setLights(true,  false, false, true);  Serial.println("WALK");
                 walkRequested = false;                  // request served
                 break;
  }
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(WALK_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);       // pressed = LOW
  Serial.begin(9600);
  enterState(GREEN);
}

void loop() {
  // Events are read once per pass, before the state logic.
  if (digitalRead(BUTTON_PIN) == LOW && !walkRequested) {
    walkRequested = true;
    Serial.println("  button: walk requested");
  }

  unsigned long inState = millis() - stateStart;

  // Transitions: each case only answers "is it time to leave, and to where?"
  switch (state) {
    case GREEN:
      if (inState >= GREEN_MS || (walkRequested && inState >= MIN_GREEN_MS)) {
        enterState(YELLOW);
      }
      break;
    case YELLOW:
      if (inState >= YELLOW_MS) enterState(RED);
      break;
    case RED:
      if (inState >= RED_MS) enterState(walkRequested ? WALK : GREEN);
      break;
    case WALK:
      if (inState >= WALK_MS) enterState(GREEN);
      break;
  }
}
