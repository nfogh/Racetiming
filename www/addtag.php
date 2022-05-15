<?php
require '_init.php';
require '_adminpage.php';

$title = 'Add RFID Tag';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';
?>

<?php
$tid = mysqli_real_escape_string($db, htmlspecialchars($_POST["tid"]));

$numberid = mysqli_real_escape_string($db, htmlspecialchars($_POST["numberid"]));

$sql = "INSERT INTO rfidtags (tid, numberid) VALUES ('" . $tid . "'," . $numberid . ")";
if (!$db->query($sql)) {
    printf("<h2>Unable to insert new tag. " . $db->error . "</h2> {$sql}.");
}
else {
    printf("<h1>Inserted tag %s</h1>", $tid);
}
?>

<?php require "listtags.php" ?>

<?php require "inserttag.php" ?>

<?php require "_footer.php" ?>
