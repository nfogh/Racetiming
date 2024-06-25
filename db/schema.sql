CREATE TABLE `admins` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `name` varchar(50) NOT NULL
,  `password` varchar(255) NOT NULL
,  `created_at` datetime DEFAULT current_timestamp
, salt varchar(32),  UNIQUE (`name`)
);
CREATE TABLE `events` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `timestamp` timestamp NOT NULL
,  `msecs` integer NOT NULL
,  `event` integer NOT NULL
,  `numberid` integer NOT NULL
);
CREATE TABLE `numbers` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `raceid` integer NOT NULL
,  `runnerid` integer NOT NULL
,  `number` integer NOT NULL
,  `expected_laps` integer DEFAULT '1'
,  `expected_time` integer DEFAULT NULL
,  `teamid` integer DEFAULT NULL
,  UNIQUE (`raceid`,`runnerid`)
,  UNIQUE (`number`,`raceid`)
);
CREATE TABLE `permissions` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `adminid` integer NOT NULL
,  `raceid` integer NOT NULL
);
CREATE TABLE `races` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `description` text NOT NULL
,  `name` text NOT NULL
,  `start` datetime DEFAULT NULL
,  `address` text NOT NULL
,  `finish` datetime DEFAULT NULL
,  `lap_length` float NOT NULL
,  `latitude` double DEFAULT NULL
,  `longitude` double DEFAULT NULL
);
CREATE TABLE `rest_api_keys` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `adminid` integer NOT NULL
,  `api_key` varchar(128) NOT NULL
);
CREATE TABLE `runners` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `name` text NOT NULL
,  `surname` text NOT NULL
);
CREATE TABLE `tags` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT
,  `numberid` integer NOT NULL
,  `tid` varchar(64) NOT NULL
,  UNIQUE (`numberid`,`tid`)
);
CREATE TABLE `teams` (
  `id` integer NOT NULL
,  `name` text NOT NULL
,  `raceid` integer NOT NULL
,  PRIMARY KEY (`id`)
);
