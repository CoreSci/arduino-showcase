/*
 * Relay dual toggle
 * -----------------
 * Problem: check a 2-channel relay module (and its wiring) before connecting
 * real loads.
 * How it works: cycles through the four ON/OFF combinations of two relays, one
 * second each. Most relay boards are active LOW: LOW = ON, HIGH = OFF.
 * Wiring: relay IN1 -> A0, IN2 -> A1, plus VCC and GND.
 */
int RELAY1 = A0;
int RELAY2 = A1;

// Both relays start OFF (HIGH on an active-LOW board)
void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

}

// Sequence: both ON -> both OFF -> R1 only -> R2 only (1 s each)
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  delay(1000);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  delay(1000);
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, HIGH);
  delay(1000);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, LOW);
  delay(1000);
}
