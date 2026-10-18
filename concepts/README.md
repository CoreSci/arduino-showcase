# Concepts

Hands-on units on the ideas underneath most embedded projects, starting with **state machines** and **analog input (ADC)**. They sit between [`learning-with-kids/`](../learning-with-kids/) and [`projects/`](../projects/). Each unit explains the *why* as well as the *how*, with a sketch you can modify, a technical README and a "why this matters" section connecting it to real embedded work.

**Level:** intermediate (teenagers and adults with some programming). You should be comfortable uploading a sketch, using the Serial Monitor and wiring an LED with its resistor.

| # | Unit | You learn | Key parts |
|---|---|---|---|
| 1 | [State machines](01-state-machines/) | Structuring behaviour as states and transitions: `if/else` chain vs `switch/case` + `enum`, non-blocking timing with `millis()` | 4 LEDs, 1 button |
| 2 | [ADC](02-adc/) | Resolution, reference voltage, ratiometric vs absolute measurement, averaging, measuring the real supply, 2-point calibration | potentiometer, multimeter |

## Common setup

- **Board:** Arduino Uno (ATmega328P). Unit 2 uses a few ATmega328P registers, marked in the code. Everything else runs on any Arduino.
- **Pins:** LEDs on D4–D7, button on D2 (internal pull-up), potentiometer on A0. This is the same layout as `learning-with-kids/`, so one breadboard carries over.
- **Serial Monitor** at 9600 baud. Unit 2 prints tab- or space-separated values that also work in the **Serial Plotter**.
- **Safety:** USB power only. Mains voltage is out of scope for every unit.
