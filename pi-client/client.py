#!/usr/bin/env python

import sys
import urllib
import urllib2
import json
import time
import datetime
from prescription import PiPrescription
# import gpio_comm
import requests

TEN_MINUTES = 600
true = True
false = False

class PiRestClient(object):
    def __init__(self, server='http://patient-care-rest.herokuapp.com/api'):
        self.server = server

    def get_prescription_ids(self, patient_id):
        prescription_list = []

        patient = requests.get(self.server + '/patients/{0}'.format(patient_id))

        if patient:
            return patient.json()['prescription_assigned']

        return None

    def get_prescriptions(self, prescription_ids):
        prescription_list = []

        for prescription_id in prescription_ids:
            prescription = requests.get(self.server + '/prescriptions/{0}'.format(prescription_id))

            if prescription:
                prescription_list.append(prescription.json())

        return prescription_list


    def get_medication_name(self, medication_id):
        medication = requests.get(self.server + '/medications/{0}'.format(medication_id))

        return medication.json()['name']


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
                    requests.put(self.server, data=payload1)

                if pi_prescription.triggered[alert_index] == 1 and hour == 0:
                    pi_prescription.triggered[alert_index] = 0;

                alert_index += 1

def webClientRealTimeTest(patient_id):
    client = PiRestClient()
    start = time.time()
    flag = True;
    payload1 = { 'medication_taken':'false'}
    payload2 = { 'medication_taken':'false'}

    while True:
        if time.time() - start >= 10:
            start = time.time()

            if flag:
                requests.put(client.server + '/patients/{0}'.format(patient_id), data=payload1)
                print "true"
                flag = False

            else:
                requests.put(client.server + '/patients/{0}'.format(patient_id), data=payload2)
                print "false"
                flag = True



if __name__ == '__main__':
    patient_id = "5645227e3fc36e1100149818"

    # run(patient_id)

    webClientRealTimeTest(patient_id)
