<?php
$title = 'Admin interface';
$masthead_image = 'assets/images/masthead.png';

require '_init.php';
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

<?php require '_footer.php' ?>