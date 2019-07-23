-- Run this to set up the postgresql database table
-- Should only need to be ran once

--Drop table(s) before making

DROP TABLE MESSAGES;

CREATE TABLE MESSAGES(
      MESSAGE_ID  INT          NOT NULL,
      USERNAME    VARCHAR(255) NOT NULL,
      MESSAGE     VARCHAR(255) NOT NULL,
      PRIMARY KEY (MESSAGE_ID)
);
