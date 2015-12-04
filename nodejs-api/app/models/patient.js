var mongoose = require('mongoose');
var Schema = mongoose.Schema;

// _id represents patient's health_id
var PatientSchema = new Schema({
    first_name: String,
    last_name: String,
    phone: String,
    age: Number,
    medication_taken: Boolean,
    // assigned to only one caretaker's object id
    caretaker_assigned: String,
    // store object id of prescriptions
    prescription_assigned: [String]
});

module.exports = mongoose.model('Patient', PatientSchema);