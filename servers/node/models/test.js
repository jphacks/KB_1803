var mongoose   = require('mongoose');
var Schema = mongoose.Schema;

var Test = new Schema({
    time: Date,
    host: String,
    location: String,
    value: Number
});

module.exports = mongoose.model('test', Test);
