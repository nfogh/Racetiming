<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>

 <?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");
$name = mysqli_real_escape_string($db, htmlspecialchars($_POST["name"]));
$description = mysqli_real_escape_string($db, htmlspecialchars($_POST["description"]));
$start = mysqli_real_escape_string($db, htmlspecialchars($_POST["start"]));
$finish = mysqli_real_escape_string($db, htmlspecialchars($_POST["finish"]));
$address = mysqli_real_escape_string($db, htmlspecialchars($_POST["address"]));
$latitude = mysqli_real_escape_string($db, htmlspecialchars($_POST["latitude"]));
$longitude = mysqli_real_escape_string($db, htmlspecialchars($_POST["longitude"]));

print_r($_POST);

printf("<h1>Inserted race %s</h1>", $racename);

$sql = 'INSERT INTO races (name, description, start, finish, address, gpscoords) VALUES (' . 
    '"' . $name .        '",' . 
    '"' . $description . '",' . 
    '"' . $start .       '",' . 
    '"' . $finish .      '",' . 
    '"' . $address .     '",' . 
    'POINT(' . $latitude . ', ' . $longitude . '))';
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