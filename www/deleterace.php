<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>

 <?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");

$ids = [];
foreach ($_POST['check'] as $id => $value) {
    array_push($ids, $id);
    // do stuff with your database
}

$sql = "DELETE FROM races WHERE id IN (" . implode(", ", $ids) . ")";

printf("<h1>Delete races %s</h1>", implode(", ", $ids));

if (!$db->query($sql)) {
    printf("<h2>Unable to delete race. " . $db->error . "</h2>");
}
require "listraces.php"
?>

<?php

require "insertrace.php"
?>
 </body>
</html>