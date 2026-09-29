/*
 * Lesson 5 - Secret-code lock  (Le cadenas secret)
 * Goal: press the two buttons in the secret order to "unlock" the green light.
 *       One mistake and the lock starts over!
 * Big idea: a computer follows a sequence exactly. Order matters.
 *
 * Wiring: button 1 D2 -> GND, button 2 D3 -> GND,
 *         LED 1 (red, "locked") on D4, LED 3 (green, "unlocked") on D6,
 *         LED 2 (yellow) on D5 blinks for each correct press. All with 220 ohm resistors.
 */

const int BUTTON_1 = 2;
const int BUTTON_2 = 3;
const int RED_LED = 4;
const int YELLOW_LED = 5;
const int GREEN_LED = 6;

// The secret code: 1 = button 1, 2 = button 2. Change it to make your own!
const int SECRET[] = {1, 1, 2, 1, 2};
const int CODE_LENGTH = sizeof(SECRET) / sizeof(SECRET[0]);

int step = 0;                         // how many correct presses so far

// Wait for a button press and return which one (1 or 2).
int waitForButton() {
  while (true) {
    if (digitalRead(BUTTON_1) == LOW) {
      delay(30);                                      // ignore the bounce of the contact
      while (digitalRead(BUTTON_1) == LOW) { }        // wait until the finger lets go
      delay(30);
      return 1;
    }
    if (digitalRead(BUTTON_2) == LOW) {
      delay(30);
      while (digitalRead(BUTTON_2) == LOW) { }
      delay(30);
      return 2;
    }
  }
}

void blink(int pin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(150);
    digitalWrite(pin, LOW);
    delay(150);
  }
}

void setup() {
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);        // start locked
  Serial.begin(9600);
  Serial.println("Cadenas ferme. Entre le code secret !");
}

void loop() {
  int button = waitForButton();

  if (button == SECRET[step]) {       // right button for this step
    step++;
    blink(YELLOW_LED, 1);
    Serial.print("Bon ! ");
    Serial.print(step);
    Serial.print(" sur ");
    Serial.println(CODE_LENGTH);
  } else {                            // wrong button: start over
    step = 0;
    blink(RED_LED, 3);
    digitalWrite(RED_LED, HIGH);
    Serial.println("Oups ! On recommence.");
  }

  if (step == CODE_LENGTH) {          // the whole code was right
    Serial.println("Cadenas ouvert !");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(3000);                      // stay open for 3 seconds
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);      // lock again
    step = 0;
    Serial.println("Cadenas ferme.");
  }
}
