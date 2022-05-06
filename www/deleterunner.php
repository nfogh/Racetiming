<html>
 <head>
  <title>Admin</title>
 </head>
 <body>

 <?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");

$ids = [];
foreach ($_POST['check'] as $id => $value)
    array_push($ids, $id);

$sql = "DELETE FROM runners WHERE id IN (" . implode(", ", $ids) . ")";

printf("<h1>Delete runners %s</h1>", implode(", ", $ids));

if (!$db->query($sql)) {
    printf("<h2>Unable to delete runner. " . $db->error . "</h2>");
}
require "listrunners.php"
?>

<?php

require "insertrunner.php"
?>
 </body>
</html>