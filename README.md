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
| [`system-controller`](projects/system-controller/) | Keypad + LCD control of four relays and two DS18B20 probes, an automatic temperature hold, and CSV serial telemetry to a Raspberry Pi. Includes a host-side monitor script and an archived v1. |

## Snippets

| Snippet | Category |
|---|---|
| [`relay-dual-toggle`](snippets/actuators/relay-dual-toggle/) | actuators: 2-channel relay wiring test |
| [`bluetooth-gps-nmea`](snippets/communication/bluetooth-gps-nmea/) | communication: decode `$GPRMC` from a serial/Bluetooth GPS on the host side (Python) |

Third-party libraries used by these sketches (Keypad, OneWire, DallasTemperature, LiquidCrystal_I2C) come from the Arduino Library Manager and aren't vendored here.

## Philosophy

This library bridges two things: focused embedded systems work from a prototyping period, and a present-day motivation to build and learn hands-on with family. The `snippets/` and `projects/` folders are the reference and portfolio layers; `learning-with-kids/` is where things get simplified and shared.

## Status

🚧 Under active development — migrating and organizing existing sketches.
