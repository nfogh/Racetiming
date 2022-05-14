<?php
require "_init.php";

if (!isset($_POST['apikey']))
    exit("{ \"addevent\": \"error: Missing apikey parameter\" }");

$apikey = htmlspecialchars($_POST["apikey"]);

if (!isset($_POST['numberid']))
    exit("{ \"addevent\": \"error: Missing numberid parameter\" }");
         
$numberid = mysqli_real_escape_string($db, htmlspecialchars($_POST["numberid"]));

$sql = "
    SELECT 
        numbers.id
    FROM numbers
    JOIN permissions ON (numbers.raceid = permissions.raceid)
    JOIN rest_api_keys ON (rest_api_keys.adminid = permissions.adminid) 
    WHERE rest_api_keys.api_key = '{$apikey}' AND numbers.id='{$numberid}'";
if ($res = $db->query($sql)) {
    if (!($row = $res->fetch_assoc())) {
        printf('PERMISSION DENIED');
        exit();
    }
    $res->close();
} else {
    exit("{ \"addevent\": \"error {$db->error}\" }");
}

if (!isset($_POST['timestamp']))
    exit("{ \"addevent\": \"error: Missing timestamp parameter\" }");
$timestamp = mysqli_real_escape_string($db, htmlspecialchars($_POST["timestamp"]));

if (!isset($_POST['msecs']))
    exit("{ \"addevent\": \"error: Missing msecs parameter\" }");

$msecs = mysqli_real_escape_string($db, htmlspecialchars($_POST["msecs"]));

if (!isset($_POST['event']))
    exit("{ \"addevent\": \"error: Missing event parameter\" }");

$event = mysqli_real_escape_string($db, htmlspecialchars($_POST["event"]));

$sql = 'INSERT INTO events (numberid, timestamp, msecs, event) VALUES ("' . $numberid . '", "' . $timestamp . '", "' . $msecs . '", "' . $event . '")';
if ($db->query($sql)) {
    exit("{ \"addevent\": \"success\" }");
} else {
    exit("{ \"addevent\": \"error: {$db->error}\" }");
}
?>
