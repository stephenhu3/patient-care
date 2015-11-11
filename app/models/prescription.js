var mongoose = require('mongoose');
var Schema = mongoose.Schema;


var PrescriptionSchema = new Schema({
	// object id of medication
	medication_assigned: String,
    instructions: String,
    // currently just in 24 hours, use Date object when time tracking implemented
    alerts: [Number]
});

module.exports = mongoose.model('Prescription', PrescriptionSchema);