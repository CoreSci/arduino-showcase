# Bluetooth GPS (NMEA) reader, host side

Python scripts that read a GPS module over a serial or Bluetooth link (e.g. an HC-05/06 adapter) and decode the `$GPRMC` sentence.

| Script | What it does |
|---|---|
| `bluetooth_gps_console.py` | Continuous console readout: time, lat/lon in degrees + minutes, direction, speed, course, date |
| `bluetooth_gps_tkinter.py` | A minimal Tkinter button that reads and prints one fix per click |

**Setup:** `pip install pyserial`, then point `SERIAL_PORT` at your adapter's port, e.g. `SERIAL_PORT=/dev/rfcomm0 python bluetooth_gps_console.py` (default `COM5`). Truncated or corrupted sentences are skipped, not fatal.
