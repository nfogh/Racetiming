<?php
$title = 'Delete race';
$masthead_image = 'assets/races/smormosen.jpg';
$masthead_text = 'Race admin';

require '_init.php';
require '_adminpage.php';

?>

<?php
    if (empty($_POST['check'])) {
        $errorstring = "No races selected for deletion";
    }

    if (empty($errorstring)) {
    $ids = [];
    foreach ($_POST['check'] as $id => $value)
        array_push($ids, $id);

    $res = $sqlite->query('DELETE FROM races WHERE id IN (' . implode(', ', $ids) . ')');
    if (!$res) {
        $errorstring = 'Unable to delete race. ' . $sqlite->lastErrorMsg();
    } else {
        if ($sqlite->changes() != count($ids))
            $errorstring = 'Unable to delete all races (' . implode(', ', $ids) . '). One or more of the races was not found.';
        else {
            $successstring = 'Deleted races with id ' . implode(", ", $ids) . ' ' . $res->num_rows;
        }
    }
}

    require '_header.php';

    if (!empty($successstring))
        print('<div class="callout large success"><h5>' . $successstring . '</h5></div>');

    if (!empty($errorstring))
        print('<div class="callout large success"><h5>' . $errorstring . '</h5></div>');
?>

<?php require 'listraces.php' ?>
<?php require "insertrace.php" ?>
<?php require '_footer.php' ?>
