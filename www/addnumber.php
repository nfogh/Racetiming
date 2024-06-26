<?php
$title = 'Add number';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";
require '_init.php';
require '_adminpage.php';
require '_header.php';
?>

<?php
if (empty($_POST['runnerid']))
    $errorstring  = 'The name ID the runner was not set';
$runnerid = SQLite3::escapeString(htmlspecialchars($_POST['runnerid']));

if (empty($_POST['raceid']))
    $errorstring  = 'The name ID the race was not set';
$raceid = SQLite3::escapeString(htmlspecialchars($_POST['raceid']));

if (empty($_POST['number']))
    $errorstring  = 'The number was not set';
$number = SQLite3::escapeString(htmlspecialchars($_POST['number']));

if (empty($_POST['laps']))
    $errorstring  = 'The number of laps was not set';
$laps = SQLite3::escapeString(htmlspecialchars($_POST['laps']));

if (empty($_POST['time']))
    $errorstring  = 'The expected time was not set';
$time = SQLite3::escapeString(htmlspecialchars($_POST['time']));

$sql = "INSERT INTO numbers (raceid, runnerid, number, expected_laps, expected_time) VALUES ( {$raceid} , {$runnerid}, {$number}, {$laps}, {$time} )";
if (!$sqlite->query($sql)) {
    printf("<h2>Unable to insert new number. " . $sqlite->getLastError() . "</h2>");
}
else {
    printf("<h1>Inserted number %s for race %s, runner %s</h1>", $number, $raceid, $runnerid);
}
require "listnumbers.php"
?>

<?php require "insertnumber.php" ?>

<?php require '_footer.php' ?>
