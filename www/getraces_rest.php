<?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");

if (!isset($_GET['apikey']))
    exit("Missing apikey parameter");
$apikey = htmlspecialchars($_GET["apikey"]);

$sql = 'SELECT races.name as name, races.id as id from races JOIN permissions ON (permissions.raceid = races.id) JOIN rest_api_keys ON (rest_api_keys.adminid = permissions.adminid) WHERE rest_api_keys.api_key = "' . $apikey . '"';
if ($res = $db->query($sql)) {
    while ($row = $res->fetch_assoc())
        $data[] = $row;
    $res->close();
    echo "{ \"races\": " . json_encode($data) . " }";
} else {
    printf("Error " . $db->error . ". SQL: " . $sql);
}

?>
