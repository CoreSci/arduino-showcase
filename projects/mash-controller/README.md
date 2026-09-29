# Mash Controller

An Arduino temperature controller for the mash step of home brewing. It heats the mash to a set-point, holds it there for a fixed number of cycles while a pump keeps the liquid circulating, then shuts everything off.

**Problem it solves:** holding a mash temperature by hand means watching a thermometer and switching the heater on and off for an hour or more. Here, a simple state machine does that job.

## Parts List

- Arduino Uno (or compatible)
- Analog temperature sensor (reading on A0; A1 for the calibration sketch)
- 2 relay channels: heater (D4) and circulation pump (D5)
- Heater element and small pump, switched through the relays

## Wiring

| Signal | Arduino pin |
|---|---|
| Temperature sensor output | A0 (A1 in `calibration-sensor`) |
| Heater relay input | D4 |
| Pump relay input | D5 |

No wiring diagram yet. The table above is the complete pin map.

## How It Works

`mash-controller.ino` runs a three-state machine at one cycle per second:

1. **Heat-up:** heater and pump ON until the scaled sensor reading passes `criticalTemp`.
2. **Mash hold:** each cycle switches the heater ON below the set-point and OFF above it, while the pump runs. After `max` cycles (3600 = 1 hour), go to step 3.
3. **Done:** heater and pump OFF.

Each step logs its readings over serial at 9600 baud.

`calibration-sensor/calibration-sensor.ino` streams raw sensor values so you can find the reading that matches your target temperature. Measure against a reference thermometer, then set `criticalTemp` from that reading.

## Notes / Lessons Learned

- The set-point is a raw scaled ADC value, not °C. Calibrate once per sensor.
- On/off control is simple and good enough for a large thermal mass like a mash. A PID loop would reduce overshoot.
- `max` sets the hold time. It's 15 cycles in the code for bench testing and 3600 for a real one-hour mash.
