# Arduino Showcase

A structured library of embedded systems work — from small reusable building blocks to complete projects, including simplified versions designed for learning-by-building with a child.

## Structure

- `snippets/` — Small, reusable code blocks organized by function:
  - `sensors/` — Reading input from sensors (temperature, distance, light, etc.)
  - `actuators/` — Driving motors, servos, relays, etc.
  - `communication/` — Serial, I2C, SPI, wireless communication patterns.
- `projects/` — Complete builds, one folder per project, each with its own README, wiring diagram, and code.
- `learning-with-kids/` — Simplified, heavily annotated versions of concepts above, designed for building alongside a child.
- `lib/` — Custom reusable Arduino libraries developed along the way.

## Projects

| Project | What it does |
|---|---|
| [`mash-controller`](projects/mash-controller/) | Heats a brewing mash to a set-point, holds it for a timed number of cycles with a pump circulating, then shuts off. Includes a sensor-calibration sketch. |
| [`multichannel-voltage-logger`](projects/multichannel-voltage-logger/) | Logs five DC voltages (three through calibrated dividers for rails above 5 V) to an SD card as CSV. Includes a tested host-side Python script that plots the log and flags clipping and malformed lines. *Adapted from the Arduino SD "Datalogger" example* |
| [`system-controller`](projects/system-controller/) | Keypad + LCD control of four relays and two DS18B20 probes, an automatic temperature hold, and CSV serial telemetry to a Raspberry Pi. Includes a host-side monitor script and an archived v1. |

## Snippets

| Snippet | Category |
|---|---|
| [`relay-dual-toggle`](snippets/actuators/relay-dual-toggle/) | actuators: 2-channel relay wiring test |
| [`bluetooth-gps-nmea`](snippets/communication/bluetooth-gps-nmea/) | communication: decode `$GPRMC` from a serial/Bluetooth GPS on the host side (Python) |

## Learning with kids

[`learning-with-kids/`](learning-with-kids/) holds eight hands-on lessons for a child aged 6–8 with a parent. Parent guides are **in French**; code and comments are in English. All eight use one shared breadboard wiring: 4 LEDs, 2 buttons, a potentiometer and a piezo buzzer, USB-powered only.

| # | Lesson | Big idea |
|---|---|---|
| 1 | Hello, light! *(Bonjour, lumière !)* | on/off and waiting |
| 2 | Secret Morse flashlight, with sound | sequences and codes |
| 3 | Magic knob light meter | a sensor gives a number |
| 4 | Button tug-of-war, with sound | a two-player score |
| 5 | Secret-code lock | exact sequences |
| 6 | Copy my timing | stopwatch and memory |
| 7 | Toy-car parking lot | counting up and down |
| 8 | Robot turn signals | cause and effect |

Third-party libraries used by these sketches (Keypad, OneWire, DallasTemperature, LiquidCrystal_I2C) come from the Arduino Library Manager and aren't vendored here.

## Philosophy

This library bridges two things: focused embedded systems work from a prototyping period, and a present-day motivation to build and learn hands-on with family. The `snippets/` and `projects/` folders are the reference and portfolio layers; `learning-with-kids/` is where things get simplified and shared.

## Status

🚧 Under active development — migrating and organizing existing sketches.
