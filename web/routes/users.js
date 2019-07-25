var express = require('express');
var router = express.Router();

var db = require('../db/db_utility.js')

let conf_data = require('../config.json');

/* POST a new user */

router.post('/createUser', function(req, res, next) {
  var user_id;
  let username = req.body.username;

  db.query('SELECT * FROM USERS WHERE USERNAME = $1', [username]).then(data => {
    if (data[0]) {
      res.status(400).json({body:{issue:"Username Already Exists"}});
    }
    else {
      db.query( 'SELECT MAX(ID) FROM USERS').then(results => {
        if (!results[0]) {
          user_id = 1;
        }
        else {
          user_id = results[0].max + 1;
        }

        let password = req.body.password;

        db.query('INSERT INTO USERS (ID, USERNAME, PASSWORD) VALUES ($1, $2, $3)', [user_id, username, password]).then(results => {
          res.status(200).json({body:{result:"User Profile Successfully Created"}})
        }).catch(error => {
          console.log('ERROR:', error);
          res.status(400).json({body:{issue:"Bad Request"}});
        })
      }).catch(error => {
        console.log('ERROR:', error);
        res.status(500).json({body:{issue:"Issue Connecting to Database"}});
      })
    }
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(500).json({body:{issue:"Issue Connecting to Database"}});
  })
});

/* POST a new authentication credential */
router.post('/auth', function(req, res, next) {
  let password = req.body.password;
  let username = req.body.username;

  //TODO create auth token(?)

  db.query('SELECT PASSWORD FROM USERS WHERE USERNAME = $1', [username]).then(data => {
    if (!data[0]) {
      res.status(404).json({body: {results: {status: "failure", reason: "Username does not exist"}}});
    }
    else if (data[0].password === password) {
      res.status(200).json({body: {results: {status: "success", reason: "everything matched"}}});
    }
    else {
      res.status(422).json({body: {results: {status: "failure", reason: "Incorrect Password"}}});
    }
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(400).json({body:{issue:"Bad Request"}});
  })

});

/* GET current authentication credential */
router.get('/auth', function(req, res, next) {
  res.status(200).send('respond with a resource');
});

module.exports = router;
