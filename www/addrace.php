<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>

 <?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");
$racename = htmlspecialchars($_POST["racename"]);
$racename = mysqli_real_escape_string($db, $racename);

printf("<h1>Inserted race %s</h1>", $racename);

$sql = "INSERT INTO races (description) VALUES ('" . $racename . "')";
printf($sql);
if (!$db->query($sql)) {
    printf("<h2>Unable to insert new race. " . $db->error . "</h2>");
}
require "listraces.php"
?>

<?php

require "insertrace.php"
?>
 </body>
</html>