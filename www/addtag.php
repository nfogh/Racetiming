<?php
require '_init.php';
require '_adminpage.php';

$title = 'Add RFID Tag';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';
?>

<?php
$tid = SQLite3::escapeString(htmlspecialchars($_POST["tid"]));

$numberid = SQLite3::escapeString(htmlspecialchars($_POST["numberid"]));

$sql = "INSERT INTO tags (tid, numberid) VALUES ('" . $tid . "'," . $numberid . ")";
if (!$sqlite->query($sql)) {
    printf("<h2>Unable to insert new tag. " . $sqlite->lastErrorMsg() . "</h2> {$sql}.");
}
else {
    printf("<h1>Inserted tag %s</h1>", $tid);
}
?>

<?php require "listtags.php" ?>

<?php require "inserttag.php" ?>

<?php require "_footer.php" ?>
