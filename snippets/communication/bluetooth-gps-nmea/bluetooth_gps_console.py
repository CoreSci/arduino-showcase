"""Read a Bluetooth/serial GPS module and print a readable position fix.

Problem: GPS modules stream raw NMEA sentences, which are hard to read directly.
This script picks out the ``$GPRMC`` (recommended minimum) sentence and prints
time, latitude/longitude in degrees + minutes, direction, speed, course and date.

Usage: pair the GPS module (e.g. through an HC-05/06 serial Bluetooth adapter),
then run ``SERIAL_PORT=/dev/rfcomm0 python bluetooth_gps_console.py``.
``SERIAL_PORT`` defaults to ``COM5``. Stop with Ctrl+C. Requires ``pyserial``.
"""
from __future__ import annotations

import os

import serial

SERIAL_PORT = os.environ.get("SERIAL_PORT", "COM5")
CLEAR = "cls" if os.name == "nt" else "clear"


def nmea_to_deg_min(value: str) -> str:
    """'4717.11399' (ddmm.mmmm) -> '47 deg 17.11399 min' (also works for dddmm.mmmm)."""
    head, tail = value.split(".")
    return head[0:-2] + " deg " + head[-2:] + "." + tail + " min"


def parse_rmc(sentence: bytes) -> dict[str, str] | None:
    """Decode a $GPRMC sentence into display fields.

    Returns None for other sentence types or truncated lines, and
    ``{"status": "V"}`` when the receiver has no valid fix yet.
    """
    # Only handle RMC sentences: $GPRMC,time,status,lat,N/S,lon,E/W,speed,course,date,...
    if sentence[0:6] != b'$GPRMC':
        return None
    splitData = sentence.split(b',')
    if len(splitData) < 10:           # partial line (common right after connecting)
        return None
    # Status 'V' = no valid fix yet ('A' = valid)
    if splitData[2] == b'V':
        return {"status": "V"}
    try:
        return {
            "status": "A",
            "time": splitData[1][0:2].decode() + ":" + splitData[1][2:4].decode() + ":" + splitData[1][4:6].decode(),
            "lat": nmea_to_deg_min(splitData[3].decode()),
            "dirLat": splitData[4].decode(),
            "lon": nmea_to_deg_min(splitData[5].decode()),
            "dirLon": splitData[6].decode(),
            "speed": splitData[7].decode(),
            "trCourse": splitData[8].decode(),
            "date": splitData[9][0:2].decode() + "/" + splitData[9][2:4].decode() + "/" + splitData[9][4:6].decode(),
        }
    except (ValueError, UnicodeDecodeError):  # corrupted field
        return None


def main() -> None:
    serialPort = serial.Serial(port=SERIAL_PORT, baudrate=9600, bytesize=8, timeout=2,
                               stopbits=serial.STOPBITS_ONE)
    try:
        while True:
            fix = parse_rmc(serialPort.readline())
            if fix is None:
                continue
            os.system(CLEAR)
            if fix["status"] == "V":
                print("no satellite data available")
                continue
            for key in ("time", "lat", "dirLat", "lon", "dirLon", "speed", "trCourse", "date"):
                print(f"{key} : ", fix[key])
    except KeyboardInterrupt:
        pass
    finally:
        serialPort.close()


if __name__ == "__main__":
    main()
