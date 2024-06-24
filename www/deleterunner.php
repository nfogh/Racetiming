<?php
$title = 'Delete runner';
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

if (!$sqlite->query($sql)) {
    printf("<h2>Unable to delete runner. " . $sqlite->lastErrorMsg() . "</h2>");
}
require "listrunners.php"
?>

<?php

require "insertrunner.php"
?>
<?php require '_footer.php' ?>
