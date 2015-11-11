var mongoose = require('mongoose');
var Schema = mongoose.Schema;

var CaretakerSchema = new Schema({
    first_name: String,
    last_name: String,
    phone: String,
    email: String,
    // store _id of patients (can have multiple patients)
    patient_assigned: [String]
});

module.exports = mongoose.model('Caretaker', CaretakerSchema);