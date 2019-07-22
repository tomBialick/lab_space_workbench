var express = require('express');
var router = express.Router();

let conf_data = require('../config.json');

var https = require('../bin/www');
var io = require('socket.io')(https);

io.on('connection', () =>{
  console.log('a user is connected')
})

router.post('/messages', (req, res) => {
  var message = new Message(req.body);
  message.save((err) =>{
    if(err)
      sendStatus(500);
    io.emit('message', req.body);
    res.sendStatus(200);
  })
})

app.get('/messages', (req, res) => {
  Message.find({},(err, messages)=> {
    res.send(messages);
  })
})

var mongoose = require(‘mongoose’);
mongoose.connect('mongodb://' + conf_data["mongodb"]["user"] + ':' + conf_data["mongodb"]["pwd"] + '@localhost:27017/' + conf_data["mongodb"]["db"], {useNewUrlParser: true});
var Message = mongoose.model(‘Message’,{ name : String, message : String})

/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendFile(path.join(__dirname + '/..', '/public', 'index.html'));
});

module.exports = router;
