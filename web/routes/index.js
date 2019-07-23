var express = require('express');
var router = express.Router();
var path = require('path');

let conf_data = require('../config.json');

const promise = require('bluebird');
const initOptions = {
    promiseLib: promise
};

const pgp = require('pg-promise')(initOptions);
const db = pgp({
  user: conf_data["postgresql"]["username"],
  host: conf_data["postgresql"]["host"],
  database: conf_data["postgresql"]["db"],
  password: conf_data["postgresql"]["password"],
  port: conf_data["postgresql"]["port"],
});

/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendFile(path.join(__dirname + '/..', '/client/build', 'index.html'));
});

/* POST a new chat */
router.post('/chat', function(req, res, next) {
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

/* GET new chats */
router.get('/chat', function(req, res, next) {
  let messageQuery = req.query.messageID;
  db.query('SELECT * FROM MESSAGES WHERE MESSAGE_ID > $1', [messageQuery]).then(results => {
    res.status(200).send(results)
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(400).send("Bad Request");
  })

});

module.exports = router;
