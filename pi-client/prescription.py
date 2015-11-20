#!/usr/bin/env python

import sys
import urllib
import urllib2
import json
import time
import datetime

class PiPrescription(object):
	def __init__(self, alertsForToday, medication_name, instructions, timeout):
		self.alertsForToday = alertsForToday
		self.medication_name = medication_name
		self.instructions = instructions
		self.triggered = [0] * len(alertsForToday)
		self.timeout = timeout
		self.dayOfWeek = datetime.date.today().strftime("%A")
