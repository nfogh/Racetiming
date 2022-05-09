<?php
$title = 'Add runner';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_init.php';
require '_adminpage.php';

if (empty($_POST['name']))
    $errorstring  = 'The name of the runner was not set';
$name = mysqli_real_escape_string($db, htmlspecialchars($_POST["name"]));

if (empty($_POST['surname']))
    $errorstring  = 'The surname of the runner was not set';
$surname = mysqli_real_escape_string($db, htmlspecialchars($_POST["surname"]));

if (!isset($errorstring)) {
    $sql = 'INSERT INTO runners (name, surname) VALUES ("' . $name . '", "' . $surname . '")';
    if (!$db->query($sql)) {
        $errorstring = "Unable to insert new runner. " . $db->error;
    }
    else {
        $successstring = 'Inserted runner ' . $runnername;
    }
}

require '_header.php';

if (isset($errorstring))
    printf('<div class="callout large alert">' . $errorstring . '</div>');
if (isset($successstring))
    printf('<div class="callout large success">' . $successstring . '</div>');
?>

<?php require "listrunners.php" ?>
<?php require "insertrunner.php" ?>

<?php require '_footer.php' ?>