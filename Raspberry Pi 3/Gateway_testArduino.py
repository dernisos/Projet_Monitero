import serial
import os.path
ser = serial.Serial('/dev/ttyACM0', 9600)
if not os.path.exists("essai.txt"):
	with open("essai.txt","w") as f:
		f.write("Fichier des capteurs\n")
while 1 :
	temp = ser.readline()
	print temp
	if "Capteur" in temp:
		typcap,numcap,upTime,valcap = temp.split(",")
		print typcap+" "+numcap+" "+upTime+" "+valcap+'\n'
		with open("essai.txt","r") as f:
			lines = f.readlines()
		pos = -1
		for line in lines:
			if numcap in line:
				pos = lines.index(line)
				print pos
		if pos != -1:
			lines.pop(pos)
		lines.append(temp)
		with open("essai.txt","w") as f:
			for line in lines:
				f.write(line)

	
			