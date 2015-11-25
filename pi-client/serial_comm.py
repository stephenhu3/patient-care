import serial
import time

class PiSerialComm(object):
	def __init__(self, port=serial.Serial("/dev/ttyUSB0", baudrate=115200)):
		self.port = port

	def readline(self):

		out = ''

		while self.port.inWaiting() > 0:
			out += self.read(1)

		return out

	def writeline(self, line):
		self.port.write(line)

if __name__ == '__main__':
	newPort = PiSerialComm()

	newPort.writeline("Hi DE2\n")

	time.sleep(2)

	newPort.writeline("This is Pi\n")

	time.sleep(2)

	newPort.writeline("bye")

	time.sleep(5)

	message = newPort.readline()

	print message

