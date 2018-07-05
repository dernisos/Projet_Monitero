#This program test is use to post on a website
import RPi.GPIO as GPIO
import serial
import time, sys, os
import datetime

def setupHardGSM():
	GPIO.setmode(GPIO.BCM)
	GPIO.setwarnings(False)
	GPIO.setup(18,GPIO.OUT) # GSM Hardware reset
	GPIO.setup(24,GPIO.OUT) # GSM ON/OFF module
	GPIO.setup(25,GPIO.OUT) # GSM Request to send
	GPIO.setup(12,GPIO.IN) # GSM Battery status
	GPIO.output(24,1)
	
def setupSoftGSM():
	port.write("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r") #Set the connection type to GPRS
	time.sleep(2)
	rcv = port.read(10);
	print rcv

	port.write("AT+SAPBR=3,1,\"APN\",\"m2minternet\"\r") #Set the APN, here for SFR sim card it's "m2minternet"
	time.sleep(2)
	rcv = port.read(10);
	print rcv

	port.write("AT+SAPBR=1,1") #Enable the GPRS
	time.sleep(2)
	rcv = port.read(10);
	print rcv

	port.write("AT+SAPBR=2,1")#It should give us a IP address, then we can proceed to post
	time.sleep(5)
	rcv = port.read(20);
	print rcv
	
SERIAL_PORT = "/dev/ttyS0" #Rasp 3 UART Port

port = serial.Serial(SERIAL_PORT,baudrate=9600, timeout=10)
setupHardGSM()
time.sleep(2)
 
setupSoftGSM()

port.write("AT+HTTPINIT\r") #Enable the HTTP mode
time.sleep(2)
rcv = port.read(10);
print rcv

port.write("AT+HTTPPARA=\"CID\",1\r") #Setup the HTTP bearer profile identifier
time.sleep(2)
rcv = port.read(10);
print rcv

port.write("AT+HTTPPARA=\"URL\",\r") #Set the URL to the address of the webpage you want to post to
time.sleep(2)
rcv = port.read(10);
print rcv

port.write("AT+HTTPPARA=\"CONTENT\",\" \"") #Set the content type of data send
time.sleep(5)
rcv = port.read(10);
print rcv

port.write("AT+HTTPACTION=0\r") #Execute the HTTP POST command so that the buffer contents are POST to the serveur
time.sleep(2)
rcv = port.read(10);
print rcv
