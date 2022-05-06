<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>

 <?php
//   print_r($_POST);

$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");
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
 </body>
</html>