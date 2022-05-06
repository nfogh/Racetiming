<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>

 <?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");
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

<?php require "insertrunner.php" ?>
 </body>
</html>