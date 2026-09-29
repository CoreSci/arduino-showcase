/*
 * Lesson 7 - Toy-car parking lot  (Le stationnement des petites voitures)
 * Goal: a parking lot with 4 spots. Press button 1 when a toy car drives in,
 *       button 2 when one drives out. Each lit LED is a taken spot.
 *       When the lot is full, the lights flash to say "COMPLET!"
 * Big idea: a counter that goes up and down, with a smallest (0) and biggest (4) value.
 *
 * Wiring: button 1 ("car in") D2 -> GND, button 2 ("car out") D3 -> GND,
 *         LEDs 1-4 on D4-D7 (220 ohm to GND).
 */

const int CAR_IN = 2;
const int CAR_OUT = 3;
const int LED_PINS[4] = {4, 5, 6, 7};
const int SPOTS = 4;

int cars = 0;                         // how many cars are parked right now

// Wait until a button is let go, so one press counts as one car.
void waitRelease(int pin) {
  delay(30);
  while (digitalRead(pin) == LOW) { }
  delay(30);
}

void showCars() {
  for (int i = 0; i < SPOTS; i++) {
    digitalWrite(LED_PINS[i], i < cars ? HIGH : LOW);
  }
  Serial.print("Voitures : ");
  Serial.print(cars);
  Serial.print(" / ");
  Serial.println(SPOTS);
}

void flashFull() {
  Serial.println("COMPLET ! Plus de place.");
  for (int n = 0; n < 3; n++) {
    for (int i = 0; i < SPOTS; i++) digitalWrite(LED_PINS[i], LOW);
    delay(200);
    for (int i = 0; i < SPOTS; i++) digitalWrite(LED_PINS[i], HIGH);
    delay(200);
  }
}

void setup() {
  pinMode(CAR_IN, INPUT_PULLUP);
  pinMode(CAR_OUT, INPUT_PULLUP);
  for (int i = 0; i < SPOTS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
  Serial.begin(9600);
  showCars();
}

void loop() {
  if (digitalRead(CAR_IN) == LOW) {
    waitRelease(CAR_IN);
    if (cars < SPOTS) {
      cars++;                         // one more car parked
      showCars();
    } else {
      flashFull();                    // no room: the car has to wait
    }
  }
  if (digitalRead(CAR_OUT) == LOW) {
    waitRelease(CAR_OUT);
    if (cars > 0) {
      cars--;                         // one car leaves
    }
    showCars();                       // never goes below zero
  }
}
