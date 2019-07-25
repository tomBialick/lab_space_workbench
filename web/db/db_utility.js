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

module.exports = db;
