<?php
$title = 'Delete numbers';
$masthead_image = 'assets/races/smormosen.jpg';
$masthead_text = 'Race admin';

require '_init.php';
require '_adminpage.php';
?>

<?php
$ids = [];
foreach ($_POST['check'] as $id => $value)
    array_push($ids, $id);

$sql = "DELETE FROM numbers WHERE id IN (" . implode(", ", $ids) . ")";

if (!$sqlite->query($sql))
    $errorstring = "<h2>Unable to delete numbers. " . $sqlite->lastErrorMsg() . "</h2>";
?>

<? require '_header.php' ?>

<?php
    if (isset($errorstring))
        printf('<div class="callout large alert">' . $errorstring . '</div>');
    else
        printf('<div class="callout large success">Deleted numbers with IDs ' . implode(", ", $ids) . "</div>");
?>

<?php require 'listnumbers.php' ?>
<?php require 'insertnumber.php' ?>
<?php require '_footer.php' ?>
