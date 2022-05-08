<?php
require '_init.php';
require '_admin.php';

$title = 'Add RFID Tag';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';
?>

<?php
$tid = htmlspecialchars($_POST["tid"]);
$tid = mysqli_real_escape_string($db, $tid);

$runnerid = htmlspecialchars($_POST["runnerid"]);
$runnerid = mysqli_real_escape_string($db, $runnerid);

$sql = "INSERT INTO rfidtags (tid, runnerid) VALUES (0x" . $tid . "," . $runnerid . ")";
printf($sql);
if (!$db->query($sql)) {
    printf("<h2>Unable to insert new tag. " . $db->error . "</h2>");
}
else {
    printf("<h1>Inserted tag %s</h1>", $runnername);
}
require "listtags.php"
?>

<?php require "inserttag.php" ?>

<?php require "_footer.php" ?>
