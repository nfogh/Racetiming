<?php
$title = 'Delete eventst';
$masthead_image = 'assets/races/smormosen.jpg';
$masthead_text = 'Race admin';

require '_init.php';
require '_adminpage.php';
?>

<?php
$ids = [];
foreach ($_POST['check'] as $id => $value)
    array_push($ids, $id);

$sql = "DELETE FROM events WHERE id IN (" . implode(", ", $ids) . ")";

if (!$db->query($sql))
    $errorstring = "<h2>Unable to delete events. " . $db->error . "</h2>";
?>

<? require '_header.php' ?>

<?php
    if (isset($errorstring))
        printf('<div class="callout large alert">' . $errorstring . '</div>');
    else
        printf('<div class="callout large success">Deleted events with IDs ' . implode(", ", $ids) . "</div>");
?>

<?php require 'listevents.php' ?>
<?php require 'insertevent.php' ?>
<?php require '_footer.php' ?>
