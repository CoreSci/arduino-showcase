# Relay dual toggle

Checks a 2-channel relay module and its wiring by cycling through every ON/OFF combination, one second each. Run it before connecting real loads.

- **Wiring:** IN1 → A0, IN2 → A1, VCC → 5 V, GND → GND.
- **Note:** most relay boards are active LOW, so `LOW` = ON and `HIGH` = OFF.
