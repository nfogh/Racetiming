<?php
require '_init.php';
require '_adminpage.php';

$title = 'Add event';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';

if (!isset($_POST['raceid']))
    exit("Missing raceid parameter");
$raceid = mysqli_real_escape_string($db, htmlspecialchars($_POST["raceid"]));

if (!isset($_POST['runnerid']))
    exit("Missing runnerid parameter");
$runnerid = mysqli_real_escape_string($db, htmlspecialchars($_POST["runnerid"]));

if (!isset($_POST['timestamp']))
    exit("Missing timestamp parameter");
$timestamp = mysqli_real_escape_string($db, htmlspecialchars($_POST["timestamp"]));

if (!isset($_POST['msecs']))
    exit("Missing msecs parameter");
$msecs = mysqli_real_escape_string($db, htmlspecialchars($_POST["msecs"]));

if (!isset($_POST['event']))
    exit("Missing event parameter");
$event = mysqli_real_escape_string($db, htmlspecialchars($_POST["event"]));

$sql = 'INSERT INTO events (raceid, runnerid, timestamp, msecs, event) VALUES ("' . $raceid . '", "' . $runnerid . '", "' . $timestamp . '", "' . $msecs . '", "' . $event . '")';
     if ($db->query($sql)) {
         printf("<div class='callout large success'>Inserted event</div>");
     } else {
         printf("<div class='callout large alert'>Error {$db->error}. SQL: {$sql}</div>");
    }
?>

<?php require "listevents.php" ?>

<?php require "insertevent.php" ?>

<?php require '_footer.php' ?>
