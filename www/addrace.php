<?php
require '_init.php';
require '_adminpage.php';

$title = 'Add race';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";

require '_header.php';
?>

<?php
    if (empty($_POST['name']))
        $errorstring  = 'The name of the race was not set';
    $name = SQLite3::escapeString(htmlspecialchars($_POST["name"]));

    if (empty($_POST['description']))
        $errorstring = 'The description of the race was not set';
    $description = SQLite3::escapeString($_POST["description"]);

    if (empty($_POST['start']))
        $errorstring = 'The start time of the race was not set';
    $start = SQLite3::escapeString(htmlspecialchars($_POST["start"]));

    if (empty($_POST['finish']))
        $errorstring = 'The finish time of the race was not set';
    $finish = SQLite3::escapeString(htmlspecialchars($_POST["finish"]));

    if (empty($_POST['address']))
        $errorstring = 'The address of the race was not set';
    $address = SQLite3::escapeString(htmlspecialchars($_POST["address"]));

    if (empty($_POST['latitude']))
        $errorstring = 'The latitude of the race was not set';
    $latitude = SQLite3::escapeString(htmlspecialchars($_POST["latitude"]));

    if (empty($_POST['longitude']))
        $errorstring  = 'The longitude of the race was not set';
    $longitude = SQLite3::escapeString(htmlspecialchars($_POST['longitude']));

    if (empty($_POST['lap_length']))
        $errorstring  = 'The lap length of the race was not set';
    $lap_length = SQLite3::escapeString(htmlspecialchars($_POST['lap_length']));

    if (!isset($errorstring)) {
        $sql = 'INSERT INTO races (name, description, start, finish, address, latitude, longitude, lap_length) VALUES (' . 
            '"' . $name .        '",' . 
            '"' . $description . '",' . 
            '"' . $start .       '",' . 
            '"' . $finish .      '",' . 
            '"' . $address .     '",' . 
            $latitude . ', ' .
            $longitude . ', ' .
             $lap_length . ')';

        if (!$sqlite->query($sql))
            printf('<div class="callout large alert"><h5>Unable to insert new race. ' . $sqlite->lastErrorMsg() . "</h5></div>");
        else
            printf('<div class="callout large success"><h5>Inserted race ' . $name . '</h5></div>');
    } else {
        print('<div class="callout large alert"><h5>Error: ' . $errorstring . '</h5></div>');
    }
?>


<?php require "listraces.php" ?>

<?php require "insertrace.php" ?>

<?php require '_footer.php' ?>
