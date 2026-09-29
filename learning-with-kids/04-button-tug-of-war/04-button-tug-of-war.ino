/*
 * Lesson 4 - Button tug-of-war  (Le tir a la corde)
 * Goal: two players press their own button as fast as they can. Each press
 *       pulls the "rope" toward that player. First to pull it all the way wins!
 * Big idea: the program keeps a score (a number that goes up and down)
 *           and compares it to a goal.
 *
 * Wiring: button 1 (player 1) D2 -> GND, button 2 (player 2) D3 -> GND,
 *         LEDs 1-4 on D4-D7 (220 ohm to GND), piezo buzzer (+) on D8.
 *         Player 1 sits on the LED 1 side, player 2 on the LED 4 side.
 */

const int BUTTON_1 = 2;
const int BUTTON_2 = 3;
const int LED_PINS[4] = {4, 5, 6, 7};
const int BUZZER_PIN = 8;
const int WIN_AT = 10;              // how far the rope must go to win

int rope = 0;                       // 0 = middle, negative = player 1 side, positive = player 2 side
bool wasPressed1 = false;           // remembers the last button state, so holding a button
bool wasPressed2 = false;           // down counts as ONE pull (you have to press again)

// Show where the rope is with the 4 LEDs.
void showRope() {
  bool on[4] = {false, false, false, false};
  if (rope <= -WIN_AT / 2)     on[0] = true;              // far on player 1's side
  else if (rope < 0)           on[1] = true;
  else if (rope == 0)        { on[1] = true; on[2] = true; }  // right in the middle
  else if (rope < WIN_AT / 2)  on[2] = true;
  else                         on[3] = true;              // far on player 2's side
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], on[i] ? HIGH : LOW);
  }
}

// Little victory song and flashing lights on the winner's side.
void celebrate(int winner) {
  Serial.print("Le joueur ");
  Serial.print(winner);
  Serial.println(" gagne !");
  int winnerLeds[2] = {winner == 1 ? 0 : 2, winner == 1 ? 1 : 3};
  int melody[4] = {523, 659, 784, 1047};      // do, mi, sol, do (C E G C)
  for (int round = 0; round < 3; round++) {
    for (int n = 0; n < 4; n++) {
      digitalWrite(LED_PINS[winnerLeds[0]], n % 2 == 0 ? HIGH : LOW);
      digitalWrite(LED_PINS[winnerLeds[1]], n % 2 == 0 ? LOW : HIGH);
      tone(BUZZER_PIN, melody[n], 150);
      delay(180);
    }
  }
  noTone(BUZZER_PIN);
  rope = 0;                                   // new game
  showRope();
  delay(1000);
}

void setup() {
  pinMode(BUTTON_1, INPUT_PULLUP);            // pressed = LOW, thanks to the built-in resistor
  pinMode(BUTTON_2, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
  Serial.begin(9600);
  Serial.println("Tir a la corde ! Appuyez vite !");
  showRope();
}

void loop() {
  bool pressed1 = digitalRead(BUTTON_1) == LOW;
  bool pressed2 = digitalRead(BUTTON_2) == LOW;

  if (pressed1 && !wasPressed1) {             // a NEW press by player 1
    rope--;
    tone(BUZZER_PIN, 440, 30);                // low click
  }
  if (pressed2 && !wasPressed2) {             // a NEW press by player 2
    rope++;
    tone(BUZZER_PIN, 880, 30);                // high click
  }
  wasPressed1 = pressed1;
  wasPressed2 = pressed2;

  showRope();
  if (rope <= -WIN_AT) celebrate(1);
  if (rope >= WIN_AT)  celebrate(2);
  delay(15);                                  // tiny pause so one press isn't counted twice
}
