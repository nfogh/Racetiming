<?php
$title = 'Delete runners';
$masthead_image = 'assets/races/smormosen.jpg';
$masthead_text = 'Race admin';

require '_adminpage.php';
require '_init.php';
?>

<?php
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

<?php require "insertrunner.php" ?>

<?php require '_footer.php' ?>