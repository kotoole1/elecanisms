import serial
import time
import matplotlib.pyplot as plt
import platform

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

def processData(data):
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

def makePlots(times, speeds, plotType):
	if plotType == "spin-down":
		plt.plot(times[:-timeConstant], speeds)
		plt.ylabel("Speed of motor (rotations per second)")
		plt.xlabel("Time (s)")
		plt.title("Spin Down Test", fontsize="28")

	elif plotType == "calibration":
		plt.plot(values)
		plt.ylabel("Voltage")
		plt.xlabel("Angle")
	plt.show()

if __name__ == "__main__":

	values = getData()
	times, speeds = processData(values)
	makePlots(times, speeds, 'spin-down')

