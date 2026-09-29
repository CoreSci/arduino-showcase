/*
 * Multichannel Voltage Logger
 * ---------------------------
 * Problem: record several DC voltages at once (e.g. supply rails or sensor
 * outputs above 5 V) to an SD card for later analysis, with no PC attached.
 *
 * How it works: each loop reads analog inputs A0-A4, converts the raw 10-bit
 * reading (0-1023) to volts with a per-channel full-scale calibration value,
 * prints every channel to serial, and appends one CSV line to datalog.txt.
 * A0-A2 measure through resistor voltage dividers, so their full scale is
 * above 5 V. A3-A4 are direct 0-5 V inputs (see README for wiring and calibration).
 *
 * datalog.txt: one line per sample, five comma-separated voltages, e.g.
 *   7.12,5.98,5.01,3.30,1.65
 *
 * Adapted from: Arduino SD library "Datalogger" example (Tom Igoe, public domain).
 * Extended to five channels, with per-channel voltage calibration.
 *
 * Hardware: Arduino Uno (or compatible) + SD card module/shield (SPI, CS on D4).
 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;             // SD card chip-select pin (D4 on most SD/Ethernet shields)

// Per-channel full-scale voltage: the input voltage that produces a reading of 1023.
// A0-A2 sit behind voltage dividers. The values were calibrated against a meter
// (see README). A3-A4 are direct inputs referenced to the 5 V supply.
const int NUM_CHANNELS = 5;
const float FULL_SCALE_V[NUM_CHANNELS] = {7.86, 6.06, 5.97, 5.0, 5.0};

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait for the serial port to connect (needed on native-USB boards only)
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);                         // halt: nothing to log to
  }
  Serial.println("card initialized.");
}

void loop() {
  String dataString = "";

  for (int analogPin = 0; analogPin < NUM_CHANNELS; analogPin++) {
    // volts = raw reading x (full-scale volts / 1023)
    float sensor = analogRead(analogPin) * (FULL_SCALE_V[analogPin] / 1023.0);
    Serial.println(sensor);            // bug fix: A4 was logged to SD but never printed

    dataString += String(sensor);
    if (analogPin < NUM_CHANNELS - 1) {
      dataString += ",";
    }
  }

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();                  // close after every line so a power loss costs at most one sample
    Serial.print(",");
    Serial.print(dataString);
    Serial.println("");
  }
  else {
    Serial.println("error opening datalog.txt");
  }
}
