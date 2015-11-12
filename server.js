// BASE SETUP
// =============================================================================

// call the packages we need
var express    = require('express');        // call express
var app        = express();                 // define our app using express
var bodyParser = require('body-parser');
var uriUtil    = require('mongodb-uri');

// configure app to use bodyParser()
// this will let us get the data from a POST
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Fill in credentials
var DBUSER = process.env.MONGOLAB_USER;
var DBPASS = process.env.MONGOLAB_PASS;

var mongoose = require('mongoose');
// connect to hosted mongolab db
var mongodbUri = 'mongodb://' + DBUSER + ':' + DBPASS + '@ds051524.mongolab.com:51524/group9';
var mongooseUri = uriUtil.formatMongoose(mongodbUri);
var mongooseLocalUri = 'mongodb://localhost/node_api'; // connect to local db

mongoose.connect(mongooseUri, function (error) {
    if (error) {
        console.log('Failed to connect to server:\n' + error);
    }
});


var Patient = require('./app/models/patient');
var Caretaker = require('./app/models/caretaker');
var Prescription = require('./app/models/prescription');
var Medication = require('./app/models/medication');

var port = process.env.PORT || 8080; // set our port

// ROUTES FOR OUR API
// =============================================================================
var router = express.Router();              // get an instance of the express Router

// middleware to use for all requests
router.use(function(req, res, next) {
    // do logging
    console.log('Request was made');
    next(); // make sure we go to the next routes and don't stop here
});

// test route to make sure everything is working (accessed at GET http://localhost:8080/api)
router.get('/', function(req, res) {
    res.json({ message: 'You are using Patient-Care REST API' });   
});

// on routes that end in /patients
// ----------------------------------------------------
router.route('/patients')

    // create a patient (accessed at POST http://localhost:8080/api/patients)
    .post(function(req, res) {
        var patient = new Patient();      // create a new instance of the Patient model
        patient.first_name = req.body.first_name;  // set the patients first name (comes from the request)
        patient.last_name = req.body.last_name;
        patient.phone = req.body.phone;
        patient.age = req.body.age;
        patient.medication_taken = req.body.medication_taken;
        patient.caretaker_assigned = req.body.caretaker_assigned;
        patient.prescription_assigned = req.body.prescription_assigned;

        // save the patient and check for errors
        patient.save(function(err) {
            if (err)
                res.send(err);
            res.json({ message: 'Patient successfully created' });
        });
        
    })

    // get all the patients (accessed at GET http://localhost:8080/api/patients)
    .get(function(req, res) {
        Patient.find(function(err, patients) {
            if (err)
                res.send(err);
            res.json(patients);
        });
    });

router.route('/patients/:patient_id')

    // get the patient with that id (accessed at GET http://localhost:8080/api/patients/:patient_id)
    .get(function(req, res) {
        Patient.findById(req.params.patient_id, function(err, patient) {
            if (err)
                res.send(err);
            res.json(patient);
        });
    })

    // update the patient with this id (accessed at PUT http://localhost:8080/api/patients/:patient_id)
    .put(function(req, res) {
        // use our patient model to find the patient we want
        Patient.findById(req.params.patient_id, function(err, patient) {
            if (err)
                res.send(err);
          
            // set attributes if they're defined in the PUT request
            patient.first_name = req.body.first_name ? req.body.first_name : patient.first_name;
            patient.last_name = req.body.last_name ? req.body.last_name : patient.last_name;
            patient.phone = req.body.phone ? req.body.phone : patient.phone;
            patient.age = req.body.age ? req.body.age : patient.age;
            patient.medication_taken = req.body.medication_taken ? req.body.medication_taken : patient.medication_taken;
            patient.caretaker_assigned = req.body.caretaker_assigned ? req.body.caretaker_assigned : patient.caretaker_assigned;
            patient.prescription_assigned = req.body.prescription_assigned ? req.body.prescription_assigned : patient.prescription_assigned;

            // save the patient
            patient.save(function(err) {
                if (err)
                    res.send(err);
                res.json({ message: 'Patient successfully updated' });
            });

        });
    })

    // delete the patient with this id (accessed at DELETE http://localhost:8080/api/patients/:patient_id)
    .delete(function(req, res) {
        Patient.remove({
            _id: req.params.patient_id
        }, function(err, patient) {
            if (err)
                res.send(err);
            res.json({ message: 'Patient successfully deleted' });
        });
    });

// on routes that end in /caretakers
// ----------------------------------------------------
router.route('/caretakers')

    // create a caretaker (accessed at POST http://localhost:8080/api/caretakers)
    .post(function(req, res) {
        var caretaker = new Caretaker();      // create a new instance of the Caretaker model
        caretaker.first_name = req.body.first_name;
	    caretaker.last_name = req.body.last_name;
	    caretaker.phone = req.body.phone;
	    caretaker.email = req.body.email;
	    caretaker.patient_assigned = req.body.patient_assigned;

        // save the caretaker and check for errors
        caretaker.save(function(err) {
            if (err)
                res.send(err);
            res.json({ message: 'Caretaker successfully created' });
        });
        
    })

    // get all the caretakers (accessed at GET http://localhost:8080/api/caretakers)
    .get(function(req, res) {
        Caretaker.find(function(err, caretakers) {
            if (err)
                res.send(err);
            res.json(caretakers);
        });
    });

router.route('/caretakers/:caretaker_id')

    // get the caretaker with that id (accessed at GET http://localhost:8080/api/caretakers/:caretaker_id)
    .get(function(req, res) {
        Caretaker.findById(req.params.caretaker_id, function(err, caretaker) {
            if (err)
                res.send(err);
            res.json(caretaker);
        });
    })

    // update the caretaker with this id (accessed at PUT http://localhost:8080/api/caretakers/:caretaker_id)
    .put(function(req, res) {
        // use our caretaker model to find the caretaker we want
        Caretaker.findById(req.params.caretaker_id, function(err, caretaker) {
            if (err)
                res.send(err);

            // set attributes if they're defined in the POST request
		    caretaker.first_name = req.body.first_name ? req.body.first_name : caretaker.first_name;
		    caretaker.last_name = req.body.last_name ? req.body.last_name : caretaker.last_name;
		    caretaker.phone = req.body.phone ? req.body.phone : caretaker.phone;
		    caretaker.email = req.body.email ? req.body.email : caretaker.email;
		    caretaker.patient_assigned = req.body.patient_assigned ? req.body.patient_assigned : caretaker.patient_assigned;

            // save the caretaker
            caretaker.save(function(err) {
                if (err)
                    res.send(err);
                res.json({ message: 'Caretaker successfully updated' });
            });

        });
    })

    // delete the caretaker with this id (accessed at DELETE http://localhost:8080/api/caretakers/:caretaker_id)
    .delete(function(req, res) {
        Caretaker.remove({
            _id: req.params.caretaker_id
        }, function(err, caretaker) {
            if (err)
                res.send(err);
            res.json({ message: 'Caretaker successfully deleted' });
        });
    });

// on routes that end in /prescriptions
// ----------------------------------------------------
router.route('/prescriptions')

    // create a prescription (accessed at POST http://localhost:8080/api/prescriptions)
    .post(function(req, res) {
        var prescription = new Prescription();      // create a new instance of the Prescription model
        prescription.medication_assigned = req.body.medication_assigned;
        prescription.instructions = req.body.instructions;
        prescription.alerts = req.body.alerts;

        // save the prescription and check for errors
        prescription.save(function(err) {
            if (err)
                res.send(err);
            res.json({ message: 'Prescription successfully created' });
        });
        
    })

    // get all the prescriptions (accessed at GET http://localhost:8080/api/prescriptions)
    .get(function(req, res) {
        Prescription.find(function(err, prescriptions) {
            if (err)
                res.send(err);
            res.json(prescriptions);
        });
    });

router.route('/prescriptions/:prescription_id')

    // get the prescription with that id (accessed at GET http://localhost:8080/api/prescriptions/:prescription_id)
    .get(function(req, res) {
        Prescription.findById(req.params.prescription_id, function(err, prescription) {
            if (err)
                res.send(err);
            res.json(prescription);
        });
    })

    // update the prescription with this id (accessed at PUT http://localhost:8080/api/prescriptions/:prescription_id)
    .put(function(req, res) {
        // use our prescription model to find the prescription we want
        Prescription.findById(req.params.prescription_id, function(err, prescription) {
            if (err)
                res.send(err);

            // set attributes if they're defined in the PUT request
		    prescription.medication_assigned = req.body.medication_assigned ? req.body.medication_assigned : prescription.medication_assigned;
	        prescription.instructions = req.body.instructions ? req.body.instructions : prescription.instructions;
	        prescription.alerts = req.body.alerts ? req.body.alerts : prescription.alerts;

            // save the prescription
            prescription.save(function(err) {
                if (err)
                    res.send(err);
                res.json({ message: 'Prescription successfully updated' });
            });

        });
    })

    // delete the prescription with this id (accessed at DELETE http://localhost:8080/api/prescriptions/:prescription_id)
    .delete(function(req, res) {
        Prescription.remove({
            _id: req.params.prescription_id
        }, function(err, prescription) {
            if (err)
                res.send(err);
            res.json({ message: 'Prescription successfully deleted' });
        });
    });

// on routes that end in /medications
// ----------------------------------------------------
router.route('/medications')

    // create a medication (accessed at POST http://localhost:8080/api/medications)
    .post(function(req, res) {
        var medication = new Medication();      // create a new instance of the Medication model
	    medication.name = req.body.name;
	    medication.active_ingredient = req.body.active_ingredient;

        // save the medication and check for errors
        medication.save(function(err) {
            if (err)
                res.send(err);
            res.json({ message: 'Medication successfully created' });
        });
        
    })

    // get all the medications (accessed at GET http://localhost:8080/api/medications)
    .get(function(req, res) {
        Medication.find(function(err, medications) {
            if (err)
                res.send(err);
            res.json(medications);
        });
    });

router.route('/medications/:medication_id')

    // get the medication with that id (accessed at GET http://localhost:8080/api/medications/:medication_id)
    .get(function(req, res) {
        Medication.findById(req.params.medication_id, function(err, medication) {
            if (err)
                res.send(err);
            res.json(medication);
        });
    })

    // update the medication with this id (accessed at PUT http://localhost:8080/api/medications/:medication_id)
    .put(function(req, res) {
        // use our medication model to find the medication we want
        Medication.findById(req.params.medication_id, function(err, medication) {
            if (err)
                res.send(err);

            // set attributes if they're defined in the PUT request
		    medication.name = req.body.name ? req.body.name : medication.name;
		    medication.active_ingredient = req.body.active_ingredient ? req.body.active_ingredient : medication.active_ingredient;

            // save the medication
            medication.save(function(err) {
                if (err)
                    res.send(err);
                res.json({ message: 'Medication successfully updated' });
            });

        });
    })

    // delete the medication with this id (accessed at DELETE http://localhost:8080/api/medications/:medication_id)
    .delete(function(req, res) {
        Medication.remove({
            _id: req.params.medication_id
        }, function(err, medication) {
            if (err)
                res.send(err);
            res.json({ message: 'Medication successfully deleted' });
        });
    });


// REGISTER OUR ROUTES -------------------------------
// all of our routes will be prefixed with /api
app.use('/api', router);

// START THE SERVER
// =============================================================================
app.listen(port);
console.log('Magic happens on port ' + port);