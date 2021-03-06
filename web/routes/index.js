var express = require('express');
var router = express.Router();
var path = require('path');

var db = require('../db/db_utility.js')
let conf_data = require('../config.json');

var socketApi = require('../socketApi');
var io = socketApi.io;

var mime = require('mime-types')
const { check, validationResult } = require('express-validator');

const fs = require('fs');
const AWS = require('aws-sdk');
//CHANGE THESE KEYS FOR S3 ACCESS
const s3 = new AWS.S3({
  accessKeyId: '',
  secretAccessKey: '',
  region: 'us-east-2'
});


/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendFile(path.join(__dirname + '/..', '/client/build', 'index.html'));
});

/* POST a new chat */
router.post('/chat',  [check('username').isLength({min:1}),
                       check('message').isLength({min:1})], function(req, res, next) {
  const errors = validationResult(req)
  if (!errors.isEmpty()) {
    return res.status(422).json({ errors: errors.array() })
  }

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
    db.query('INSERT INTO MESSAGES ( MESSAGE_ID, USERNAME, TYPE, MESSAGE) VALUES ($1, $2, $3, $4)', [m_id, username, 'text/html', message]).then(results => {
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
  let messageQuery = req.query.messageID;
  db.query('SELECT * FROM MESSAGES WHERE MESSAGE_ID > $1', [messageQuery]).then(results => {
    res.status(200).json({body : results})
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(400).send("Bad Request");
  })
});

/* GET all chats older than messageID */
router.get('/chat/old', function(req, res, next) {
  let messageQuery = req.query.messageID;
  db.query('SELECT * FROM MESSAGES WHERE MESSAGE_ID < $1', [messageQuery]).then(results => {
    res.status(200).json({body : results})
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(400).send("Bad Request");
  })
});

/* POST a new file */
router.post('/file', function(req, res, next) {
  if (Object.keys(req.files).length == 0) {
    res.status(400).send('Error uploading file');
  }
  let m_id;
  if (!req.body.username) {
    res.status(401).send('User not logged in');
  }
  let username = req.body.username;
  let fileObj = req.files.file;
  let fileName = fileObj.name;
  let keyName = conf_data["s3"]["folder"] + '/' +username + "_" + fileName;

  fileObj.mv('./resources/' + fileName, function(err) {
    if (err) {
      console.log(err)
      res.status(500).send("Internal Error Storing File");
    }
    let file_loc = './resources/' + fileName;
    let type = mime.lookup(file_loc);
    fs.readFile(file_loc, (err, data) => {
       if (err) throw err;
       const params = {
           Bucket: conf_data["s3"]["bucketName"],
           Key: keyName,
           ACL: 'public-read',
           Body: data
       };
       s3.upload(params, function(s3Err, data) {
         if (s3Err){
           console.log('ERROR:', s3Err);
           res.status(500).send("Error Adding File to S3 Bucket");
         }
         else {
           let file_location = data.Location;
           db.query('SELECT MAX(MESSAGE_ID) FROM MESSAGES').then(data => {
             if (!data[0]) {
               m_id = 0;
             }
             else {
               m_id = data[0].max + 1;
             }
             db.query( 'INSERT INTO MESSAGES (MESSAGE_ID, USERNAME, TYPE, ATTACHMENT_NAME, ATTACHMENT_LOCATION) VALUES ($1, $2, $3, $4, $5)', [m_id, username, type, keyName, file_location])
             .then(results => {
               console.log(`File uploaded successfully at ${file_location}`)
               fs.unlink(file_loc, (err) => {
                 if (err) {
                   console.log('ERROR:', err);
                 }
                 socketApi.sendAttachementNotification('attachment', m_id, type, username, keyName, file_location)
                 res.status(201).send("File successully uploaded");
               })
             })
             .catch(error => {
               console.log('ERROR:', error);
               res.status(500).send("Error Adding File to Database");
             });
           }).catch(error => {
             console.log('ERROR:', error);
             res.status(500).send("Error Connecting to Database");
           });
         }
       });
     });
  });
})

module.exports = router;
