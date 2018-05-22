import RPi.GPIO as GPIO
import serial
import time, sys, os
import datetime

def setup():
	GPIO.setmode(GPIO.BCM)
	GPIO.setwarnings(False)
	GPIO.setup(18,GPIO.OUT) # GSM Hardware reset
	GPIO.setup(23,GPIO.IN) # GSM Clear to Send
	GPIO.setup(24,GPIO.OUT) # GSM ON/OFF module
	GPIO.setup(25,GPIO.OUT) # GSM Request to send
	GPIO.setup(12,GPIO.IN) # GSM Battery status
	GPIO.output(24,1)
	
SERIAL_PORT = "/dev/ttyS0" #Rasp 3 UART Port

port = serial.Serial(SERIAL_PORT,baudrate=9600, timeout=10)
setup()
time.sleep(2)

port.write("AT+CGATT\r") #check if the SIM hat internet access
time.sleep(1)
rcv = port.read(10);
print rcv

