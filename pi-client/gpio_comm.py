#!/usr/bin/env python

import RPi.GPIO as GPIO

PI_WR_EN = 21
PI_DATA = 20

DE2_WR_EN = 7 
DE2_DATA = 8

TEST = 2

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

	while(!GPIO.input(DE2_WR_EN)):

	print "DE2 write enable recieved"

	GPIO.output(PI_DATA, 0)
	GPIO.output(PI_WR_EN, 0)

	print "Waiting for DE2 data"

	while(!GPIO.input(DE2_DATA))

	print "DE2 data recieved"

if __name__ == '__main__':
	initialize_pins()
	set_data_high()
	wait_for_de2()