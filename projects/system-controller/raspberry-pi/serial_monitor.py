"""Console dashboard for the System Controller's serial telemetry.

Problem: see the rig's live status (two temperatures, four relay states) on a
computer without opening the Arduino IDE.

How it works: reads CSV lines ``T1,T2,R1,R2,R3,R4`` sent by
``system-controller.ino`` at 9600 baud, validates each line (6 fields,
numeric temperatures, relay states 0/1), then clears the console and prints
the values. Malformed or partial lines are skipped. Stop with Ctrl+C.

Usage: ``SERIAL_PORT=/dev/ttyUSB0 python serial_monitor.py``. The port comes
from the ``SERIAL_PORT`` environment variable (default ``COM5``), e.g. ``COM5``
on Windows or ``/dev/ttyUSB0`` / ``/dev/ttyS0`` on a Raspberry Pi.
Requires ``pyserial``.
"""
from __future__ import annotations

import os

import serial

SERIAL_PORT = os.environ.get("SERIAL_PORT", "COM5")
CLEAR = "cls" if os.name == "nt" else "clear"


def parse_telemetry(line: str) -> list[str] | None:
    """Return the 6 telemetry fields if ``line`` is a valid T1,T2,R1,R2,R3,R4 record, else None."""
    fields = [f.strip() for f in line.split(',')]
    if len(fields) != 6:
        return None
    try:
        float(fields[0])
        float(fields[1])
    except ValueError:
        return None
    if any(f not in ("0", "1") for f in fields[2:]):
        return None
    return fields


def main() -> None:
    # Serial link to the Arduino (8N1, 9600 baud, 2 s read timeout)
    serialPort = serial.Serial(port=SERIAL_PORT, baudrate=9600, bytesize=8, timeout=2,
                               stopbits=serial.STOPBITS_ONE)
    try:
        while True:
            serialString = serialPort.readline()
            try:
                newString = serialString.decode()
            except UnicodeDecodeError:  # noise on the line (e.g. at connect time)
                continue

            fields = parse_telemetry(newString)
            if fields is None:
                continue

            os.system(CLEAR)
            print("Temperature 1 : " + fields[0])
            print("Temperature 2 : " + fields[1])
            print("Relay 1 : " + fields[2])
            print("Relay 2 : " + fields[3])
            print("Relay 3 : " + fields[4])
            print("Relay 4 : " + fields[5])
    except KeyboardInterrupt:
        pass
    finally:
        serialPort.close()


if __name__ == "__main__":
    main()
