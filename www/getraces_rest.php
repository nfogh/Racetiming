<?php
    require '_init.php';

if (!isset($_GET['apikey']))
    exit("Missing apikey parameter");
$apikey = SQLite3::escapeString(htmlspecialchars($_GET["apikey"]));

$sql = 'SELECT races.name as name, races.id as id from races JOIN permissions ON (permissions.raceid = races.id) JOIN rest_api_keys ON (rest_api_keys.adminid = permissions.adminid) WHERE rest_api_keys.api_key = "' . $apikey . '"';
$data = array();
if ($res = $sqlite->query($sql)) {
    while ($row = $res->fetchArray())
        $data[] = $row;
    $res->close();
    echo "{ \"races\": " . json_encode($data) . " }";
} else {
    printf("Error " . $sqlite->lastErrorMsg() . ". SQL: " . $sql);
}

?>
