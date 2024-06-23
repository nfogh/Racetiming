<?php
   class MyDB extends SQLite3 {
      function __construct() {
         $this->open('database.db');
      }
   }
   $sqlite = new MyDB();
   if(!$sqlite) {
      die("Unable to open database" + $sqlite->lastErrorMsg());
   }

    session_start();
?>
