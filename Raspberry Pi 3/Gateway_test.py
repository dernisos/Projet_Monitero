#!/usr/bin/python
import serial
import RPi.GPIO as GPIO
import os,time

GPIO.setmode(GPIO.BOARD)

port = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=1)


port.write("AT\n\r")

time.sleep(.1)
x = port.read(port.inWaiting())
print x
