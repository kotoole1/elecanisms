import serial
import time
import numpy as np
import matplotlib.pyplot as plt


ser = serial.Serial('/dev/ttyUSB0', 19200, timeout=1)

def getData():
	values = []
	startTime = time.time()
	while 1:
		line = ser.readline()   # read a '\n' terminated line
		if (line != ''):
			if int(line) < 800 and int(line) > 0:
				values.append(int(line))
		if time.time() - startTime > 3:
			break 
	ser.close()
	return values

def makePlots(values, plotType):
	if plotType == "spin-down":
		plt.plot(values)
		plt.ylabel("Cycles Between Flips")
		plt.xlabel("Flip Value")

	elif plotType == "calibration"
		plt.plot(values)
		plt.ylabel("Voltage")
		plt.xlabel("Angle")
	plt.title(plotType)
	plt.show()


if __name__ == "__main__":

	values = getData()
	makePlots(values)
