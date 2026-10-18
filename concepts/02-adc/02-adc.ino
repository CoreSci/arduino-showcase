/*
 * Concepts 2 - ADC: from raw counts to calibrated values
 * ------------------------------------------------------
 * Reads a potentiometer on A0 and prints, twice per second:
 *
 *   raw      one analogRead(), 0..1023
 *   avg      the mean of 16 readings (less noise)
 *   V@5.00   volts, assuming the reference is exactly 5.00 V
 *   V@Vcc    volts, using the supply voltage actually measured (see below)
 *   cal      the average converted to engineering units with a 2-point calibration
 *
 * The Uno's ADC measures relative to its reference, by default the 5 V supply.
 * USB supplies are often 4.7-5.2 V, so "5.00 V" is a guess. On ATmega328P boards
 * this sketch measures the supply by reading the chip's internal ~1.1 V bandgap
 * reference against it. Then the unknown is the bandgap, which is fixed for a
 * given chip, instead of the USB supply, which varies.
 *
 * Wiring: potentiometer outer legs to 5V and GND, wiper (middle) to A0.
 */

const int INPUT_PIN = A0;
const int SAMPLES = 16;                 // readings per average

// Internal bandgap reference in millivolts. Nominally 1100, but it varies by
// about +/-10% between chips. Calibrate once per board (see README), then
// the Vcc measurement is accurate to a few millivolts.
const long BANDGAP_MV = 1100;

// Two-point calibration: two raw readings and the real values they correspond to.
// Replace with your own measurements (see README). Here they make "cal" read in
// percent of the knob's travel: raw 0 = 0 %, raw 1023 = 100 %.
const float RAW_LO = 0.0,    REAL_LO = 0.0;
const float RAW_HI = 1023.0, REAL_HI = 100.0;

// Mean of SAMPLES readings, as a float so the fraction isn't lost.
float readAverage(int pin) {
  long sum = 0;
  for (int i = 0; i < SAMPLES; i++) {
    sum += analogRead(pin);
  }
  return (float)sum / SAMPLES;
}

// Supply voltage in millivolts, measured against the internal bandgap.
long readVccMillivolts() {
#if defined(__AVR_ATmega328P__)
  // Reference = AVcc, input = the 1.1 V bandgap (MUX3..1 = 1110)
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);                             // let the reference settle after switching
  ADCSRA |= _BV(ADSC);                  // start a conversion
  while (bit_is_set(ADCSRA, ADSC)) {}   // wait for it to finish
  long counts = ADC;                    // bandgap as a fraction of Vcc
  // counts = 1.1 V * 1024 / Vcc, so Vcc = 1.1 V * 1024 / counts.
  // analogRead() rewrites ADMUX on its next call, so nothing needs restoring.
  return BANDGAP_MV * 1024L / counts;
#else
  return 5000;                          // other boards: fall back to nominal
#endif
}

// Straight line through (RAW_LO, REAL_LO) and (RAW_HI, REAL_HI).
float calibrate(float raw) {
  return REAL_LO + (raw - RAW_LO) * (REAL_HI - REAL_LO) / (RAW_HI - RAW_LO);
}

void setup() {
  Serial.begin(9600);
  Serial.println("raw\tavg\tV@5.00\tV@Vcc\tVcc(mV)\tcal");
}

void loop() {
  long vcc = readVccMillivolts();
  analogRead(INPUT_PIN);                // discard the first reading after the reference change
  int raw = analogRead(INPUT_PIN);
  float avg = readAverage(INPUT_PIN);

  // The ATmega328P datasheet defines ADC = Vin * 1024 / Vref, so Vin = ADC * Vref / 1024.
  float voltsNominal = avg * 5.00 / 1024.0;
  float voltsMeasured = avg * (vcc / 1000.0) / 1024.0;

  Serial.print(raw);                 Serial.print('\t');
  Serial.print(avg, 1);              Serial.print('\t');
  Serial.print(voltsNominal, 3);     Serial.print('\t');
  Serial.print(voltsMeasured, 3);    Serial.print('\t');
  Serial.print(vcc);                 Serial.print('\t');
  Serial.println(calibrate(avg), 1);
  delay(500);
}
