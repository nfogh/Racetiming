<?php
require '_init.php';
require '_admin.php';

$title = 'Add runner';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';
?>

<?php
$runnername = htmlspecialchars($_POST["runnername"]);
$runnername = mysqli_real_escape_string($db, $runnername);


$sql = "INSERT INTO runners (name) VALUES ('" . $runnername . "')";
printf($sql);
if (!$db->query($sql)) {
    printf("<h2>Unable to insert new runner. " . $db->error . "</h2>");
}
else {
    printf("<h1>Inserted runner %s</h1>", $runnername);
}
require "listrunners.php"
?>

<?php require "insertrunner.php"; ?>
<?php require '_footer.php'; ?>