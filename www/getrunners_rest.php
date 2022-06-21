<?php
$db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming");

if (!isset($_GET['apikey']))
    exit("Missing apikey parameter");
$apikey = $_GET["apikey"];

if (!isset($_GET['raceid']))
    exit("Missing raceid parameter");
$raceid = $_GET["raceid"];

$stmt = $db->prepare("
    SELECT
        permissions.raceid
    FROM
        permissions
    JOIN
        rest_api_keys ON (rest_api_keys.adminid = permissions.adminid)
    WHERE
        rest_api_keys.api_key=? AND raceid=?");
$stmt->bind_param("si", $apikey, $raceid);
if ($stmt->execute()) {
    $res = $stmt->get_result();
    if (!$res->fetch_assoc())
        exit("{ \"addevent\": \"error permission denied\" }");
} else {
    exit("{ \"addevent\": \"error {$db->error}\" }");
}
$stmt->close();

$stmt = $db->prepare("
    SELECT
        numbers.id as numberid,
        numbers.number as number,
        runners.name as name,
        runners.surname as surname,
        runners.id as runnerid,
        GROUP_CONCAT(tags.tid) as tids
    FROM
        numbers
    JOIN
        races ON (races.id = numbers.raceid)
    JOIN
        runners ON (runners.id = numbers.runnerid)
    LEFT JOIN
        tags ON (tags.numberid = numbers.id)
    WHERE 
        races.id=?
    GROUP BY
        numbers.id
    ");
if (!$stmt) {
        die("Unable to prepare statement " . $db->error);
    }
$stmt->bind_param("i", $raceid);

$data = array();
if ($stmt->execute()) {
    $res = $stmt->get_result();
    while ($row = $res->fetch_assoc()) {
        if (!is_null($row["tids"]))
            $row["tids"] = explode(",", $row["tids"]);
        $data[] = $row;
    }
    $res->close();
    echo "{ \"runners\": " . json_encode($data) . " }";
} else {
    printf("Error " . $db->error . ". SQL: " . $sql);
}


?>
