#!/usr/bin/env python

import sys
import urllib
import urllib2
import json
import time
import datetime

class PiPrescription(object):
	def __init__(self, alerts, medication_name, instructions):
		self.alerts = alerts
		self.medication_name = medication_name
		self.instructions = instructions
		self.dayOfWeek = datetime.date.today().strftime("%A")

class PiAlerts(object):
	def __init__(self, hour, timeout, schedule):
		self.hour = hour
		self.timeout = timeout
		self.schedule = schedule