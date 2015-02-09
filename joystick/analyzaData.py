import serial
import time
import matplotlib.pyplot as plt
import platform

if platform.system() == 'Linux':
	ser = serial.Serial('/dev/ttyUSB0', 19200, timeout=1)
elif platform.system() == 'Darwin':
	ser = serial.Serial('/dev/tty.usbserial-FTGR7A7L', 19200, timeout=1)

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

def processData(data):
	timeConstant = 5
	samplingTime = .0001 #seconds
	total = 0
	times = []
	for cycles in data:
		time = cycles * samplingTime
		total += time
		times.append(total)

	speeds = [1.0/max(times[i:i+timeConstant]) for i in xrange(len(times) - timeConstant)]
	return times, speeds

def makePlots(times, speeds, plotType):
	if plotType == "spin-down":
		plt.plot(times, values)
		plt.ylabel("Cycles Between Flips")
		plt.xlabel("Flip Value")

	elif plotType == "calibration":
		plt.plot(values)
		plt.ylabel("Voltage")
		plt.xlabel("Angle")
	# plt.title(plotType)
	# plt.show()


if __name__ == "__main__":

	values = getData()
	times, speed = processData(values)
	makePlots(times, speeds, 'spin-down')

