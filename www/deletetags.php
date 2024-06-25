<?php
$title = 'Delete tags';
$masthead_image = 'assets/races/smormosen.jpg';
$masthead_text = 'Race admin';

require '_init.php';
require '_adminpage.php';
require '_header.php';
?>

<?php
$ids = [];
foreach ($_POST['check'] as $id => $value)
    array_push($ids, "$id");

$sql = "DELETE FROM tags WHERE id IN (" . implode(", ", $ids) . ")";

if (!$mysql->query($sql)) {
    printf("<h2>Unable to delete tags. " . $mysql->lastErrorMsg() . "</h2>");
} else {
    printf("<h1>Deleted tags %s</h1>", implode(", ", $ids));
}
?>

<?php
require "listtags.php";
require "inserttag.php";
?>

<?php require '_footer.php' ?>
