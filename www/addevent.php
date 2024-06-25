<?php
require '_init.php';
require '_adminpage.php';

$title = 'Add event';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';

if (!isset($_POST['numberid']))
    exit("Missing numberid parameter");
$numberid = sqlite_escape_string(htmlspecialchars($_POST["numberid"]));

if (!isset($_POST['timestamp']))
    exit("Missing timestamp parameter");
$timestamp = sqlite_escape_string(htmlspecialchars($_POST["timestamp"]));

if (!isset($_POST['msecs']))
    exit("Missing msecs parameter");
$msecs = sqlite_escape_string(htmlspecialchars($_POST["msecs"]));

if (!isset($_POST['event']))
    exit("Missing event parameter");
$event = sqlite_escape_string(htmlspecialchars($_POST["event"]));


$sql = 'INSERT INTO events (numberid, timestamp, msecs, event) VALUES ("' . $numberid . '", "' . $timestamp . '", "' . $msecs . '", "' . $event . '")';
     if ($sqlite->query($sql)) {
         printf("<div class='callout large success'>Inserted event</div>");
     } else {
         printf("<div class='callout large alert'>Error {$sqlite->lastErrorMsg()}. SQL: {$sql}</div>");
    }
?>

<?php require "listevents.php" ?>

<?php require "insertevent.php" ?>

<?php require '_footer.php' ?>
