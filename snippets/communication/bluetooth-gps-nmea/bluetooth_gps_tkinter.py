"""Minimal Tkinter front-end for reading GPS fixes on demand.

Problem: poll a serial/Bluetooth GPS on demand, from a button, instead of in a loop.
Each click reads one line and, if it is a ``$GPRMC`` sentence with a valid
fix, prints its fields (raw NMEA lat/lon). This was a first step towards a GUI
version of ``bluetooth_gps_console.py``.

Usage: ``SERIAL_PORT=/dev/rfcomm0 python bluetooth_gps_tkinter.py``
(``SERIAL_PORT`` defaults to ``COM5``). Requires ``pyserial`` and a desktop session.
"""
import os
import tkinter

import serial

SERIAL_PORT = os.environ.get("SERIAL_PORT", "COM5")


def main() -> None:
    frame = tkinter.Tk()
    frame.title("GPS reader")
    serialPort = serial.Serial(port=SERIAL_PORT, baudrate=9600, bytesize=8, timeout=2,
                               stopbits=serial.STOPBITS_ONE)

    # Button handler: read one NMEA line and decode it if it is an RMC sentence
    def callback() -> None:
        serialString = serialPort.readline()
        splitData = serialString.split(b',')

        if serialString[0:6] != b'$GPRMC' or len(splitData) < 10:
            print("no complete $GPRMC sentence in this read, click again")
        elif splitData[2] == b'V':
            print("no satellite data available")
        else:
            print("time : ", splitData[1][0:2].decode() + ":" + splitData[1][2:4].decode() + ":" + splitData[1][4:6].decode())
            print("lat : ", splitData[3].decode())
            print("dirLat : ", splitData[4].decode())
            print("lon : ", splitData[5].decode())
            print("dirLon : ", splitData[6].decode())
            print("speed : ", splitData[7].decode())
            print("trCourse : ", splitData[8].decode())
            print("date : ", splitData[9][0:2].decode() + "/" + splitData[9][2:4].decode() + "/" + splitData[9][4:6].decode())

    # One button triggers a single read
    button = tkinter.Button(frame, text="Read GPS fix", width=25, command=callback)
    button.pack()

    frame.mainloop()
    serialPort.close()


if __name__ == "__main__":
    main()
