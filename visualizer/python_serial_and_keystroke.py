# run this with 'sudo', otherwise the serial port access has no permission.

import serial
import os, time

ser = serial.Serial('/dev/ttyACM0', 9600)
while True:
    key_code = ser.readline()[9]
    os.system("xte 'key " + key_code + "'")
    print key_code