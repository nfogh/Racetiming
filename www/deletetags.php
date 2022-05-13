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
    array_push($ids, "0x$id");

$sql = "DELETE FROM rfidtags WHERE id IN (" . implode(", ", $ids) . ")";

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

<?php require '_footer.php' ?>
