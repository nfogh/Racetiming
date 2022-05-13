<?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");

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

$sql = 'SELECT numbers.number as number, runners.name as name, runners.id as id, runners.surname as surname FROM numbers JOIN races ON (races.id = numbers.raceid) JOIN runners ON (runners.id = numbers.runnerid) WHERE races.id=' . $raceid;

$data = array();
if ($res = $db->query($sql)) {
    while ($row = $res->fetch_assoc()) {
        $data[] = $row;
    }
    $res->close();
    echo "{ \"runners\": " . json_encode($data) . " }";
} else {
    printf("Error " . $db->error . ". SQL: " . $sql);
}


?>
