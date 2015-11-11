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
		self.triggered = [0] * len(alerts)
