<?php
if (!isset($_GET['apikey']))
    exit("Missing apikey parameter");
$apikey = htmlspecialchars($_GET["apikey"]);

if (!isset($_GET['raceid']))
    exit("Missing raceid parameter");
$raceid = mysqli_real_escape_string($db, htmlspecialchars($_GET["raceid"]));

if ($res = $db->query('SELECT permissions.raceid as raceid from permissions JOIN rest_api_keys ON (rest_api_keys.adminid = permissions.adminid) WHERE rest_api_keys.api_key = "' . $apikey . '" AND raceid=' . $raceid)) {
    if (!($row = $res->fetch_assoc())) {
        printf('PERMISSION DENIED');
        exit();
    }
    $res->close();
} else {
    printf("Error " . $db->error);
}

if (!isset($_GET['runnerid']))
    exit("Missing runnerid parameter");
$runnerid = mysqli_real_escape_string($db, htmlspecialchars($_GET["runnerid"]));

if (!isset($_GET['timestamp']))
    exit("Missing timestamp parameter");
$timestamp = mysqli_real_escape_string($db, htmlspecialchars($_GET["timestamp"]));

if (!isset($_GET['msecs']))
    exit("Missing msecs parameter");
$msecs = mysqli_real_escape_string($db, htmlspecialchars($_GET["msecs"]));

if (!isset($_GET['event']))
    exit("Missing event parameter");
$event = mysqli_real_escape_string($db, htmlspecialchars($_GET["event"]));

$sql = 'INSERT INTO events (raceid, runnerid, timestamp, msecs, event) VALUES ("' . $raceid . '", "' . $runnerid . '", "' . $timestamp . '", "' . $msecs . '", "' . $event . '")';
if ($db->query($sql)) {
} else {
    printf("Error " . $db->error . ". SQL: " . $sql);
}
?>
