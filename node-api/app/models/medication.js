var mongoose = require('mongoose');
var Schema = mongoose.Schema;

var MedicationSchema = new Schema({
    name: String,
    active_ingredient: String
});

module.exports = mongoose.model('Medication', MedicationSchema);