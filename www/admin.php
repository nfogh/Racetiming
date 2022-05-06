<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>

 <?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");
?>

<hr>
 <h1>Races</h1>
<?php require "listraces.php" ?>
<?php require "insertrace.php" ?>

<hr>
<h1>Runners</h1>
<?php require "listrunners.php" ?>
<?php require "insertrunner.php" ?>

<hr>
<h1>RFID tags</h1>
<?php require "listtags.php" ?>
<?php require "inserttag.php" ?>

<hr>
<h1>Manage numbers</h1>
<?php require "listnumbers.php" ?>
<?php require "insertnumber.php" ?>

</body>
</html>