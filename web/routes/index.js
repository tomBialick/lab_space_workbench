var express = require('express');
var router = express.Router();

let conf_data = require('../config.json');

//var mongoose = require('mongoose');
//mongoose.connect('mongodb://' + conf_data["mongodb"]["user"] + ':' + conf_data["mongodb"]["pwd"] + '@localhost:27017/' + conf_data["mongodb"]["db"], {useNewUrlParser: true});
//var Message = mongoose.model('Message',{ name : String, message : String})

/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendFile(path.join(__dirname + '/..', '/client/build', 'index.html'));
});

module.exports = router;
