#!/usr/bin/env python

import sys
import urllib
import urllib2
import json
import time
import datetime
from prescription import PiPrescription

TEN_MINUTES = 600

class PiRestClient(object):
    def __init__(self, server='http://localhost:8080/api', reqs_per_sec=15):
        self.server = server
        self.reqs_per_sec = reqs_per_sec
        self.req_count = 0
        self.last_req = 0

    def perform_rest_action(self, endpoint, hdrs=None, params=None):
        if hdrs is None:
            hdrs = {}

        if 'Content-Type' not in hdrs:
            hdrs['Content-Type'] = 'application/json'

        if params:
            endpoint += '?' + urllib.urlencode(params)

        data = None

        # check if we need to rate limit ourselves
        if self.req_count >= self.reqs_per_sec:
            delta = time.time() - self.last_req
            if delta < 1:
                time.sleep(1 - delta)
            self.last_req = time.time()
            self.req_count = 0

        try:
            request = urllib2.Request(self.server + endpoint, headers=hdrs)
            response = urllib2.urlopen(request)
            content = response.read()
            if content:
                data = json.loads(content)
            self.req_count += 1

        except urllib2.HTTPError, e:
            # check if we are being rate limited by the server
            if e.code == 429:
                if 'Retry-After' in e.headers:
                    retry = e.headers['Retry-After']
                    time.sleep(float(retry))
                    self.perform_rest_action(endpoint, hdrs, params)
            else:
                sys.stderr.write('Request failed for {0}: Status code: {1.code} Reason: {1.reason}\n'.format(endpoint, e))

        return data

    def get_prescription_ids(self, patient_id):
        prescription_list = []

        patient = self.perform_rest_action(
            '/patients/{0}'.format(patient_id)
        )

        if patient:
            return patient['prescription_assigned']

        return None



    def get_prescriptions(self, prescription_ids):
        prescription_list = []

        for prescription_id in prescription_ids:
            prescription = self.perform_rest_action(
                '/prescriptions/{0}'.format(prescription_id)
            )

            if prescription:
                prescription_list.append(prescription)

        return prescription_list


    def get_medication_name(self, medication_id):
        medication = self.perform_rest_action(
            '/medications/{0}'.format(medication_id)
        )

        return medication['name']


    def get_pi_prescriptions(self, patient_id):
        prescription_ids = self.get_prescription_ids(patient_id)
        prescriptions = self.get_prescriptions(prescription_ids)

        pi_prescriptions = []

        if prescriptions:
            for prescription in prescriptions:
                alerts = prescription['alerts']
                medication_name = self.get_medication_name(prescription['medication_assigned'])
                instructions = prescription['instructions']
                pi_prescription = PiPrescription(alerts, medication_name, instructions)
                # print pi_prescription.medication_name + ": " + pi_prescription.instructions + " AT " + str(pi_prescription.alerts[0])
                pi_prescriptions.append(pi_prescription)

        return pi_prescriptions

def run(patient_id):
    client = PiRestClient()
    start = time.time()
    pi_prescriptions = client.get_pi_prescriptions(patient_id)
    
    while 1:
        current_time = datetime.datetime.now()

        # Actually minute for testing sake
        hour = current_time.minute % 10

        # Refresh prescriptions
        if time.time() - start >= TEN_MINUTES and hour == 0:
            pi_prescriptions = client.get_pi_prescriptions(patient_id)
            start = time.time()

        # Check the alert times
        for pi_prescription in pi_prescriptions:
            alert_index = 0
            while alert_index < len(pi_prescription.alerts):
                # Alert is triggered, do something
                if pi_prescription.triggered[alert_index] == 0 and hour == pi_prescription.alerts[alert_index]:
                    print str(hour)
                    print pi_prescription.medication_name + ': ' + pi_prescription.instructions
                    pi_prescription.triggered[alert_index] = 1

                if pi_prescription.triggered[alert_index] == 1 and hour == 0:
                    print "hiii"
                    pi_prescription.triggered[alert_index] = 0;

                alert_index += 1




if __name__ == '__main__':
    patient_id = "564396e4ef456e5202000001"

    run(patient_id)