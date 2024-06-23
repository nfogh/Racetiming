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
    $name = sqlite_escape_string(htmlspecialchars($_POST["name"]));

    if (empty($_POST['description']))
        $errorstring = 'The description of the race was not set';
    $description = sqlite_escape_string($_POST["description"]);

    if (empty($_POST['start']))
        $errorstring = 'The start of the race was not set';
    $start = sqlite_escape_string(htmlspecialchars($_POST["start"]));

    if (empty($_POST['finish']))
        $errorstring = 'The finish of the race was not set';
    $finish = sqlite_escape_string(htmlspecialchars($_POST["finish"]));

    if (empty($_POST['address']))
        $errorstring = 'The address of the race was not set';
    $address = sqlite_escape_string(htmlspecialchars($_POST["address"]));

    if (empty($_POST['latitude']))
        $errorstring = 'The latitude of the race was not set';
    $latitude = sqlite_escape_string(htmlspecialchars($_POST["latitude"]));

    if (empty($_POST['longitude']))
        $errorstring  = 'The longitude of the race was not set';
    $longitude = sqlite_escape_string(htmlspecialchars($_POST['longitude']));

    if (!isset($errorstring)) {
        $sql = 'INSERT INTO races (name, description, start, finish, address, gpscoords) VALUES (' . 
            '"' . $name .        '",' . 
            '"' . $description . '",' . 
            '"' . $start .       '",' . 
            '"' . $finish .      '",' . 
            '"' . $address .     '",' . 
            'POINT(' . $latitude . ', ' . $longitude . '))';

        if (!$sqlite->query($sql))
            printf('<div class="callout large alert"><h5>Unable to insert new race. ' . $sqlite->lastErrorStr() . "</h5></div>");
        else
            printf('<div class="callout large success"><h5>Inserted race ' . $name . '</h5></div>');
    } else {
        print('<div class="callout large alert"><h5>Error: ' . $errorstring . '</h5></div>');
    }
?>


<?php require "listraces.php" ?>

<?php require "insertrace.php" ?>

<?php require '_footer.php' ?>
