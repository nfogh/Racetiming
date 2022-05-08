<?php
$title = 'Add number';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";
require '_init.php';
require '_header.php';
?>

<?php
$runnerid = htmlspecialchars($_POST["runnerid"]);
$runnerid = mysqli_real_escape_string($db, $runnerid);

$raceid = htmlspecialchars($_POST["raceid"]);
$raceid = mysqli_real_escape_string($db, $raceid);

$number = htmlspecialchars($_POST["number"]);
$number = mysqli_real_escape_string($db, $number);

$sql = "INSERT INTO numbers (raceid, runnerid, number) VALUES (" . $raceid . "," . $runnerid . ", " . $number . ")";
if (!$db->query($sql)) {
    printf("<h2>Unable to insert new number. " . $db->error . "</h2>");
}
else {
    printf("<h1>Inserted number %s for race %s, runner %s</h1>", $number, $raceid, $runnerid);
}
require "listnumbers.php"
?>

<?php require "insertnumber.php" ?>

<?php require '_footer.php' ?>