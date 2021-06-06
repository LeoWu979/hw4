import sys
import time
import serial
import sys,tty,termios
serdev = '/dev/ttyUSB0'                # use the device name you get from `ls /dev/ttyUSB*`
s = serial.Serial(serdev, 9600)

cmd = input("Enter RPC command : ")

print(cmd)

s.write(("%s\r\n" %cmd).encode())