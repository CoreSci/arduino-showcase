/*
 * Lesson 2 - Secret Morse flashlight  (La lampe de poche Morse secrete)
 * Goal: flash and beep a message in Morse code: first SOS, then your own name.
 * Big idea: a secret code is just a list of short and long signals in the right order.
 *
 * Wiring: LED 1 on D4 (220 ohm to GND), piezo buzzer (+) on D8, (-) on GND.
 */

const int LED_PIN = 4;
const int BUZZER_PIN = 8;
const int BEEP_PITCH = 700;          // buzzer note, in hertz (higher number = higher sound)
const int DOT = 200;                 // length of a short signal ".", in milliseconds

// Change the message here! Letters A-Z and spaces only.
const char MESSAGE[] = "SOS";

// Morse code for A to Z: "." = short, "-" = long
const char* MORSE[26] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",   // A-J
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",     // K-T
  "..-", "...-", ".--", "-..-", "-.--", "--.."                             // U-Z
};

// One signal: light + sound for a short or long time, then a short pause.
void signal(int length) {
  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, BEEP_PITCH);
  delay(length);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  delay(DOT);                        // gap between signals inside a letter
}

// Send one letter, e.g. 'S' -> "..."
void sendLetter(char letter) {
  if (letter == ' ') {               // space between words
    delay(DOT * 4);
    return;
  }
  if (letter >= 'a' && letter <= 'z') {
    letter = letter - 'a' + 'A';     // accept lowercase too
  }
  if (letter < 'A' || letter > 'Z') {
    return;                          // skip anything else
  }
  const char* code = MORSE[letter - 'A'];
  Serial.print(letter);
  Serial.print("  ");
  Serial.println(code);
  for (int i = 0; code[i] != '\0'; i++) {
    signal(code[i] == '.' ? DOT : DOT * 3);   // a long signal "-" lasts 3 dots
  }
  delay(DOT * 2);                    // gap between letters
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Message secret en Morse :");
}

void loop() {
  for (int i = 0; MESSAGE[i] != '\0'; i++) {
    sendLetter(MESSAGE[i]);
  }
  Serial.println("--- on recommence ---");
  delay(DOT * 10);                   // long pause, then send the message again
}
