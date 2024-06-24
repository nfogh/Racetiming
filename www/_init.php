<?php
if (!class_exists('MyDB')) {
   class MyDB extends SQLite3 {
      function __construct() {
         $this->open('../db/database.db');
      }
   }
   $sqlite = new MyDB();
   if(!$sqlite) {
      die("Unable to open database" + $sqlite->lastErrorMsg());
   }
}

    session_start();
?>
