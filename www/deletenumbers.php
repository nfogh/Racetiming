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

$sql = "DELETE FROM numbers WHERE id IN (" . implode(", ", $ids) . ")";

if (!$db->query($sql)) {
    printf("<h2>Unable to delete number. " . $db->error . "</h2>");
} else {
    printf("<h1>Delete numbers %s</h1>", implode(", ", $ids));
}
?>

<?php
require "listnumbers.php";
require "insertnumbers.php";
?>
 </body>
</html>