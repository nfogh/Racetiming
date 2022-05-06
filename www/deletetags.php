<html>
 <head>
  <title>Admin</title>
 </head>
 <body>

 <?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");

$ids = [];
foreach ($_POST['check'] as $id => $value)
    array_push($ids, "0x$id");

$sql = "DELETE FROM rfidtags WHERE tid IN (" . implode(", ", $ids) . ")";

if (!$db->query($sql)) {
    printf("<h2>Unable to delete tags. " . $db->error . "</h2>");
} else {
    printf("<h1>Deletee tags %s</h1>", implode(", ", $ids));
}
?>

<?php
require "listtags.php";
require "inserttag.php";
?>
 </body>
</html>