var express = require('express');
var router = express.Router();

let conf_data = require('../config.json');

/*const promise = require('bluebird');
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
});*/

/* POST a new user */
/*
router.post('/createUser', function(req, res, next) {
  var user_id;
  let username = req.body.username;

  db.query('SELECT * FROM USERS WHERE USERNAME = $1', [username]).then(data => {
    if (results[0]) {
      res.status(400).send("Username Already Exists");
    }
    else {
      db.query( 'SELECT * FROM USERS ORDER BY ID DESC').then(results => {
        if (!results[0]) {
          user_id = 1;
        }
        else {
          user_id = results[0].id + 1;
        }

        let password = req.body.password;

        db.query('INSERT INTO APPRAISAL ( ID, USERNAME, PASSWORD) VALUES ($1, $2, $3)', [user_id, username, password]).then(results => {
          res.status(200).send("User Profile Successfully Created")
        }).catch(error => {
          console.log('ERROR:', error);
          res.status(400).send("Bad Request");
        })
      }).catch(error => {
        console.log('ERROR:', error);
        res.status(500).send("Issue Connecting to Database");
      })
    }
  }).catch(error => {
    console.log('ERROR:', error);
    res.status(500).send("Issue Connecting to Database");
  })
});*/

/* POST a new authentication credential */
router.post('/auth', function(req, res, next) {
  res.status(200).send('respond with a resource');
});

/* GET current authentication credential */
router.get('/auth', function(req, res, next) {
  res.status(200).send('respond with a resource');
});

module.exports = router;
