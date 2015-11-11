#!/usr/bin/env python

import sys
import urllib
import urllib2
import json
import time

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

        return patient['prescription_assigned']

    def get_prescriptions(prescription_ids):
        prescription_list = []

        for prescription_id in prescription_ids:
            prescription = self.perform_rest_action(
                '/prescriptions/{0}'.format(prescription_id)
            )

            if prescription:
                prescription_list.append(prescription)

        return prescription_list


def run(patient_id):
    client = PiRestClient()
    prescription_ids = client.get_prescription_ids(patient_id)
    if prescription_ids:
        for pid in prescription_ids:
            print pid

if __name__ == '__main__':
    patient_id = "56427f49b3b21e9322000001"

    run(patient_id)