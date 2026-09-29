# System Controller

A keypad-operated controller for a small heating and circulation rig. It drives four relays (heater, pump, two spare outputs), reads two temperature probes, shows everything on a 20×4 LCD, and can stream telemetry to a Raspberry Pi or PC over serial.

**Problem it solves:** operating a rig with separate switches and a thermometer gives no single view of its state, and no log. This build puts manual control, a basic automatic temperature hold and machine-readable telemetry on one board.

## Parts List

- Arduino Uno (or compatible)
- 4×4 matrix keypad
- 2 × DS18B20 temperature probes (OneWire)
- 20×4 I2C LCD (address `0x27`)
- 4-channel relay module (active LOW)
- Toggle switch (mode select)
- Optional: Raspberry Pi or PC connected over serial, for telemetry

## Wiring

| Component | Arduino pin |
|---|---|
| Keypad rows / columns | D9, D8, D7, D6 / D5, D4, D3, D2 |
| DS18B20 data (with 4.7 kΩ pull-up) | D10 |
| Mode switch | D11 |
| Relays 1-4 (heater, pump, spare, spare) | A0, A1, A2, A3 |
| LCD (I2C) | SDA / SCL |

For the Pi link, cross the TX/RX lines and add a level shifter or a resistor divider on Arduino TX, because the Pi's GPIO runs at 3.3 V. Or just connect over USB. See `raspberry-pi/docs/`.

## How It Works

`system-controller.ino` is a state machine driven by the keypad:

| Key | Action |
|---|---|
| `*` | Read both probes, update the LCD and send one telemetry line |
| `A` / `B` / `C` / `D` | Toggle relay 1 (heater) / 2 (pump) / 3 / 4 |
| `#` | Automatic hold: switch the heater to keep probe 1 at about 50 °C for 100 readings |

With the mode switch ON, the controller streams telemetry continuously and ignores the keypad.

**Telemetry format** (9600 baud, one line per reading): `T1,T2,R1,R2,R3,R4`, where relay values are `0` = ON and `1` = OFF.

On the host side, `raspberry-pi/serial_monitor.py` validates and parses that line into a live console dashboard. Run `SERIAL_PORT=/dev/ttyUSB0 python serial_monitor.py`, or leave `SERIAL_PORT` unset for the `COM5` default. `raspberry-pi/docs/` holds the Raspberry Pi notes: serial port setup, GPIO checks, and the Apache CGI setup used for a small web interface.

`archive/system-controller-v1/` keeps the first iteration, a simple relay test triggered by any key, to show how the design evolved.

## Notes / Lessons Learned

- Active-LOW relay boards invert the logic: `digitalWrite(pin, LOW)` switches the load **ON**.
- A single CSV line per reading keeps the serial protocol easy to parse and to log.
- `notes/lcd-relay-snippet.txt` is a scratch fragment of the LCD status-display code.
