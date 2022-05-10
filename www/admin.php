<?php
$title = 'Admin interface';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "BK Health";
require '_init.php';
require '_adminpage.php';
require '_header.php';
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

<hr>
<h1>Manage events</h1>
<?php require "listevents.php" ?>
<?php require "insertevent.php" ?>

<?php require '_footer.php' ?>