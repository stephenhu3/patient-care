import serial
import time

class PiSerialComm(object):
    def __init__(self, port=serial.Serial("/dev/ttyAMA0", baudrate = 7200, timeout = 2)):
        self.port = port

def readline(self):
    rv = ""
    while True:
    ch = self.port.read()
    rv += ch
    if ch == '\r' or ch == '':
    	return rv

def writeline(self, line):
	self.port.write(line)
