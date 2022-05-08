<?php
require '_init.php';
require '_admin.php';

$title = 'Delete numbers';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';
?>

<?php
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
<?php require '_footer.php' ?>