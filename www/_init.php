<?php
if (!file_exists('config/config.ini')) {
      die('Unable to open config file. Please create a config.ini file in the config directory.');
}
$config = parse_ini_file('config/config.ini', true);

if (!class_exists('MyDB')) {
   class MyDB extends SQLite3 {
      function __construct() {
         if (!file_exists('../db/database.db')) {
            $this->open('../db/database.db');
            $init_sql = file_get_contents('../db/schema.sql');
            $this->exec($init_sql);
         } else {
            $this->open('../db/database.db', SQLITE3_OPEN_READWRITE);
         }
      }
   }
   
   $sqlite = new MyDB();
   if(!$sqlite) {
      die("Unable to open database" + $sqlite->lastErrorMsg());
   }
}

    session_start();
?>
