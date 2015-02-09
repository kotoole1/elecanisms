import serial
import time
import matplotlib.pyplot as plt
import platform
import numpy as np

if platform.system() == 'Linux':
	ser = serial.Serial('/dev/ttyUSB0', 19200, timeout=1)
elif platform.system() == 'Darwin':
	ser = serial.Serial('/dev/tty.usbserial-FTGR7A7L', 19200, timeout=1)

timeConstant = 15

def getData():
	values = []
	startTime = time.time()
	print "started serial read"
	while 1:
		line = ser.readline()   # read a '\n' terminated line
		try:
			if int(line) < 600 and int(line) > 0:
				values.append(int(line))
		except:
			print line
		if time.time() - startTime > 3:
			break 
	print "Finished gathering data"
	ser.close()
	return values

def processDownData(data):
	data.remove(max(data))
	samplingTime = .0001 #seconds
	total = 0
	times = []
	for cycles in data:
		time = cycles * samplingTime
		total += time
		times.append(total)

	speeds = [1.0/(max(data[i:i+timeConstant]) * samplingTime) 
		for i in xrange(len(data) - timeConstant)]
	return times, speeds

def processCaliData(data):
	# times = [value * 0.0001 for value in range(len(data))]
	times = np.linspace(0,3, len(data))
	speeds = [int(value)/1024.0*3.3 for value in data]
	return times, speeds

def plotDownGraph(times, speeds):
	plt.plot(times[:-timeConstant], speeds)
	plt.ylabel("Speed of motor (rotations per second)")
	plt.xlabel("Time (s)")
	plt.title("Spin Down Test", fontsize="28")
	plt.show()


def plotCaliGraph(times, speeds):
	plt.plot(times, speeds)
	plt.ylabel("Voltage(V)")
	plt.xlabel("Time(s)")
	plt.title("Calibration Test")
	plt.show()

if __name__ == "__main__":
	plotType = "down"
	value = getData()
	if plotType == "cali":
		times, speeds = processCaliData(value)
		plotCaliGraph(times, speeds)
	elif plotType == "down":
		times, speeds = processDownData(value)
		plotDownGraph(times, speeds)	

