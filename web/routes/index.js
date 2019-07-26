var express = require('express');
var router = express.Router();
var path = require('path');

var db = require('../db/db_utility.js')
let conf_data = require('../config.json');

var socketApi = require('../socketApi');
var io = socketApi.io;

/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendFile(path.join(__dirname + '/..', '/client/build', 'index.html'));
});

/* POST a new chat */
router.post('/chat', [check('username').isLength({min:1}).trim().escape(),
                      check('message').isLength({min:1}).escape()], function(req, res, next) {
  let m_id;
  let username = req.body.username;
  let message = req.body.message;
  db.query('SELECT MAX(MESSAGE_ID) FROM MESSAGES').then(data => {
    if (!data[0]) {
      m_id = 0;
    }
    else {
      m_id = data[0].max + 1;
    }
    db.query('INSERT INTO MESSAGES ( MESSAGE_ID, USERNAME, MESSAGE) VALUES ($1, $2, $3)', [m_id, username, message]).then(results => {
      socketApi.sendNotification('messages', m_id, username, message)
      res.status(200).send("message sent")
    }).catch(error => {
      console.log('ERROR:', error);
      res.status(400).send("Bad Request");
    })
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(500).send("Issue Connecting to Database");
  })
});

/* GET all chats from messageID to newest */
router.get('/chat', function(req, res, next) {
  if (typeof(req.query.messageID) !== "number") {
    console.log('ERROR:', error);
    res.status(400).send("Bad Request");
  }
  let messageQuery = req.query.messageID;
  db.query('SELECT * FROM MESSAGES WHERE MESSAGE_ID > $1', [messageQuery]).then(results => {
    res.status(200).json({body : results})
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(400).send("Bad Request");
  })

});

/**/
router.get('/chat/old', function(req, res, next) {
  if (typeof(req.query.messageID) !== "number") {
    console.log('ERROR:', error);
    res.status(400).send("Bad Request");
  }
  let messageQuery = req.query.messageID;
  db.query('SELECT * FROM MESSAGES WHERE MESSAGE_ID < $1', [messageQuery]).then(results => {
    res.status(200).json({body : results})
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(400).send("Bad Request");
  })

});

module.exports = router;
