import serial
import os.path
lines = ["maison","patate","poirre","plouf","pierre","poulet"]
for line in lines:
	if "tate" in line:
		print lines.index(line)
lines.pop(1)
lines.insert(1,"pouel")
print lines
