var mongoose = require('mongoose');
var Schema = mongoose.Schema;

var AlertSchema = new Schema({
	hour: Number,
	timeout: Number,
	schedule: {
		"Monday": Boolean,
		"Tuesday": Boolean,
		"Wednesday": Boolean,
		"Thursday": Boolean,
		"Friday": Boolean,
		"Saturday": Boolean,
		"Sunday": Boolean
	},
	prescription_assigned: String
});

module.exports = mongoose.model('Alert', AlertSchema);