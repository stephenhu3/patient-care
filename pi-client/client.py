#!/usr/bin/env python

import sys
import urllib
import urllib2
import json
import time
import datetime
from prescription import PiPrescription
from prescription import PiAlert
from serial_comm import PiSerialComm
import requests

TEN_MINUTES = 60
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
                print prescription['alert_assigned']
                alerts_assigned = self.get_alerts(prescription['alert_assigned'])
                medication_name = self.get_medication_name(prescription['medication_assigned'])
                instructions = prescription['instructions']
                num_dosages = prescription['num_dosages']
                prescription_id = prescription['_id']
                pi_prescription = PiPrescription(prescription_id, alerts_assigned, medication_name, instructions, num_dosages)
                # print pi_prescription.medication_name + ": " + pi_prescription.instructions + " AT " + str(pi_prescription.alerts[0])
                pi_prescriptions.append(pi_prescription)

        return pi_prescriptions

    def get_alerts(self, alert_ids):
        currentDayString = datetime.date.today().strftime("%A")
        alerts = []

        print currentDayString

        for alert_id in alert_ids:
            alert = requests.get(self.server + '/alerts/{0}'.format(alert_id))
            jsonAlert = alert.json()

            if jsonAlert["schedule"][currentDayString]:
                tempPiAlert = PiAlert(jsonAlert["hour"], jsonAlert["timeout"], alert_id)
                print tempPiAlert.alert_id + ": (Hour: " + str(tempPiAlert.hour) + ") (Timeout: " + str(tempPiAlert.timeout) + ")"  
                alerts.append(tempPiAlert)

        return alerts

    def decriment_dosages(self, pi_prescription):
        print "Decrimenting " + pi_prescription.prescription_id + " before - " + str(pi_prescription.num_dosages)

        pi_prescription.num_dosages = pi_prescription.num_dosages - 1

        payload = {'num_dosages': pi_prescription.num_dosages}

        requests.put(self.server + '/prescriptions/{0}'.format(pi_prescription.prescription_id), data=payload)

    def post_history(self, patient_assigned, alert_assigned, taken):
        payload = {'patient_assigned': patient_assigned,
                   'alert_assigned': alert_assigned,
                   'taken': taken,
                   'date': str(datetime.datetime.now())}
        headers = {'content-type': 'application/json'}

        r = requests.post(self.server + '/patientHistory', data=json.dumps(payload), headers=headers)

        print str(r.status_code) + str(r.reason)


def run(patient_id):
    client = PiRestClient()
    serialComm = PiSerialComm()

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

        # Iterate through prescriptions
        for pi_prescription in pi_prescriptions:
            # Iterate through alters in prescriptions
            for alert in pi_prescription.alerts:
                # If it is time for the alert to be sent, the alert has not already been sent, and it is not past the timeout
                if hour >= alert.hour and not alert.sent and not alert.past_timeout:
                    # If the timeout period has passed
                    if alert.hour * 60 + alert.timeout < (datetime.datetime.now().minute % 10)*60 + datetime.datetime.now().second:
                        print alert.alert_id + ": Timeout Passed"
                        alert.past_timeout = True
                        client.post_history(patient_id, alert.alert_id, False)

                    # Trigger sending the instructions to the DE2
                    else:
                        print "Alert sent: " + alert.alert_id

                        alert.sent = True
                        
                        minutes_before_timeout = alert.hour * 60 + alert.timeout - (datetime.datetime.now().minute % 10)*60 - datetime.datetime.now().second
                        
                        serialComm.writeline(pi_prescription.medication_name + ": " + pi_prescription.instructions)
                        triggered = serialComm.waitForResponse(minutes_before_timeout)

                        # startSend = time.time()

                        # triggered = False

                        # if time.time() - startSend < minutes_before_timeout:
                        #     triggered = True

                        if(triggered):
                            print "Alert triggered: " + alert.alert_id
                            alert.triggered = True
                            client.decriment_dosages(pi_prescription)
                            client.post_history(patient_id, alert.alert_id, True)

                        else:
                            print "Alert passed timeout: " + alert.alert_id
                            alert.past_timeout = True
                            client.post_history(patient_id, alert.alert_id, False)



if __name__ == '__main__':
    patient_id = "5645227e3fc36e1100149818"

    run(patient_id)
