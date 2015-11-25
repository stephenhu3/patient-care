import serial
import time

class PiSerialComm(object):
	def __init__(self, port=serial.Serial("/dev/ttyUSB0", baudrate=115200)):
		self.port = port

	def readline(self):

		out = ''

		while self.port.inWaiting() > 0:
			out += ser.read(1)

		return out

	def writeline(self, line):
		self.port.write(line)

if __name__ == '__main__':
	newPort = PiSerialComm()

	newPort.writeline("Hello DE2, this is the Pi. How are you?")

	time.sleep(1)

	message = newPort.readline()

	print message

