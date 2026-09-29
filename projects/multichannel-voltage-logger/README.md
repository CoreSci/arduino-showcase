# Multichannel Voltage Logger

An Arduino data logger that measures five DC voltages, including rails above the Arduino's 5 V input limit, and appends them to a CSV file on an SD card. It needs no PC. A host-side Python script plots and summarises the log afterwards.

**Problem it solves:** watching how several supply rails or sensor outputs behave over hours (sag under load, ripple, dropouts) usually needs a multi-channel data acquisition unit. A few resistors, an SD module and an Uno do the job for a few dollars.

> **Adapted from:** the Arduino SD library's "Datalogger" example (Tom Igoe, public domain). Extended to five channels with per-channel voltage calibration. Host-side tooling is original.

## Parts List

- Arduino Uno (or compatible, 5 V logic)
- SD card module or shield (SPI) + microSD card (FAT16/FAT32)
- 3 voltage dividers (6 resistors), one each for A0–A2. Example values below.
- Jumper wires / breadboard

## Wiring

**SD module (SPI):** MOSI → D11, MISO → D12, SCK → D13, CS → **D4**, plus VCC and GND. D4 is the chip-select pin used by most SD and Ethernet shields. Change `chipSelect` if yours differs.

**Voltage inputs.** An Arduino analog pin must never go above 5 V. Channels A0–A2 therefore measure through a divider (R1 from the input to the pin, R2 from the pin to GND), which scales the input down. The full-scale value in the sketch is the input voltage that produces a reading of 1023:

`full scale = 5 V × (R1 + R2) / R2`

| Channel | Full scale in sketch | Implied divider ratio (R1 + R2) / R2 | Example standard resistors (R1 / R2) |
|---|---|---|---|
| A0 | 7.86 V | ≈ 1.57 | 5.6 kΩ / 10 kΩ (nominal 7.80 V) |
| A1 | 6.06 V | ≈ 1.21 | 2.2 kΩ / 10 kΩ (nominal 6.10 V) |
| A2 | 5.97 V | ≈ 1.19 | 2.0 kΩ / 10 kΩ (nominal 6.00 V) |
| A3 | 5.0 V | 1 (direct) | none. Input must stay within 0–5 V |
| A4 | 5.0 V | 1 (direct) | none. Input must stay within 0–5 V |

The "odd" values (7.86 rather than 7.80, and so on) come from calibration: resistor tolerance and the real 5 V supply shift the nominal figure. The example resistors are one way to reach those ratios; the original divider values weren't recorded. With these values, each divider's source impedance (R1 ∥ R2 ≈ 3.6 kΩ, 1.8 kΩ and 1.7 kΩ) stays under the 10 kΩ the ATmega ADC is designed for, so readings settle correctly. Connect all grounds together.

## How It Works

Each pass of `loop()` does four things:

1. Reads A0–A4 (10-bit, 0–1023).
2. Converts each reading to volts: `reading × FULL_SCALE_V[channel] / 1023`. The five per-channel constants live in one calibration table.
3. Prints every channel to serial (one value per line), then the CSV line prefixed with `,`.
4. Opens `datalog.txt`, appends the line, and **closes the file straight away**, so a power loss costs at most the sample being written.

There's no delay in the loop, so the sample rate is set by the SD writes (typically tens of samples per second). Add a `delay()` at the end of `loop()` for slower logging.

### `datalog.txt` format

One line per sample: five comma-separated voltages with two decimals, in channel order A0–A4, with no header and no timestamps.

```
7.12,5.98,5.01,3.30,1.65
7.11,5.97,5.01,3.31,1.68
```

## Calibration

1. Power the rig from its normal supply. Calibration absorbs the error in the real 5 V reference.
2. Apply a steady, known voltage to a divider channel, near the top of its range, and measure it at the input with a multimeter: `V_meter`.
3. Read the value that channel reports on the serial monitor: `V_logged`.
4. Update that channel's constant: `FULL_SCALE_V_new = FULL_SCALE_V_old × V_meter / V_logged`.
5. Re-upload and check again at a second voltage near the low end of the range. Repeat for each divider channel.

## Host-side analysis (`host/`)

`plot_datalog.py` reads a `datalog.txt` copied off the card. It skips and counts malformed lines, such as the partial last line a power loss leaves behind. It prints min / mean / max per channel, **flags samples at full scale** (a clipped input means the divider is too small for the signal), and saves a stacked plot of all five channels.

```bash
cd host
pip install -r requirements.txt
python plot_datalog.py sample-data/datalog_synthetic.txt --rate 1 --out datalog.png
python -m pytest -q                      # unit + end-to-end tests
python make_synthetic_datalog.py         # regenerate the sample log
```

`sample-data/datalog_synthetic.txt` is **synthetic**: 10 minutes at 1 Hz, quantised exactly like the sketch's ADC. It includes a sagging battery rail (A0), a rail with a brief dropout (A1), a ramp that saturates at full scale (A2), a steady 3.3 V rail (A3) and a slow sensor-like sine (A4), and ends with one deliberately truncated line.

## Notes / Lessons Learned

- **Bug fixed from the prototype:** A4 was written to the SD card but never printed to serial. The prototype repeated the same `if` branch five times, which is where that slipped in. A calibration table plus a single code path prevents it.
- `analogRead` is relative to the 5 V supply, so a USB supply that sags to 4.8 V shifts every reading by 4%. For better accuracy, use the internal 1.1 V reference (`analogReference(INTERNAL)`) with larger divider ratios.
- Closing the file after every line trades speed for robustness, which is the right call for unattended logging.
