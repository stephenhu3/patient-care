#!/usr/bin/env python

import sys
import urllib
import urllib2
import json
import time
import datetime

class PiPrescription(object):
	def __init__(self, prescription_id, alerts, medication_name, instructions, num_dosages):
		self.prescription_id = prescription_id;
		self.alerts = alerts
		self.medication_name = medication_name
		self.instructions = instructions
		self.dayOfWeek = datetime.date.today().strftime("%A")
		self.num_dosages = num_dosages

class PiAlert(object):
	def __init__(self, hour, timeout, alert_id):
		self.hour = hour
		self.timeout = timeout
		self.alert_id = alert_id
		self.sent = False
		self.triggered = False
		self.past_timeout = False