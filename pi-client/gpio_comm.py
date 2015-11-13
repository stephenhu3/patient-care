#!/usr/bin/env python

import RPi.GPIO as GPIO
import time

PI_WR_EN = 11
PI_DATA = 9

DE2_WR_EN = 19
DE2_DATA = 26

TEST = 5

def initialize_pins():
	GPIO.setmode(GPIO.BCM)

	GPIO.setup(PI_WR_EN, GPIO.OUT)
	GPIO.setup(PI_DATA, GPIO.OUT)
	GPIO.setup(TEST, GPIO.OUT)

	GPIO.setup(DE2_WR_EN, GPIO.IN)
	GPIO.setup(DE2_DATA, GPIO.IN)

	GPIO.output(TEST, 1)
	GPIO.output(PI_WR_EN, 1)
	GPIO.output(PI_DATA, 0)

def set_data_high():
	GPIO.output(PI_DATA, 1)

def wait_for_de2():
	print "Waiting for DE2 write enable"

	while not GPIO.input(DE2_WR_EN):
		time.sleep(0.1)

	print "DE2 write enable recieved"

	GPIO.output(PI_DATA, 0)
	GPIO.output(PI_WR_EN, 0)

	print "Waiting for DE2 data"

	while not GPIO.input(DE2_DATA):
		time.sleep(0.1)

	print "DE2 data recieved"

def run():
	initialize_pins()
	set_data_high()
	wait_for_de2()

if __name__ == '__main__':
	run()