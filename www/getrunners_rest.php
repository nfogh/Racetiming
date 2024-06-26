<?php
insert("_init.php");

if (!isset($_GET['apikey']))
    exit("Missing apikey parameter");
$apikey = $_GET["apikey"];

if (!isset($_GET['raceid']))
    exit("Missing raceid parameter");
$raceid = $_GET["raceid"];

$stmt = $sqlite->prepare("
    SELECT
        permissions.raceid
    FROM
        permissions
    JOIN
        rest_api_keys ON (rest_api_keys.adminid = permissions.adminid)
    WHERE
        rest_api_keys.api_key=:apikey AND raceid=:raceid");
$stmt->bindValue("apikey", $apikey);
$stmt->bindValue("raceid", $raceid);
if ($res = $stmt->execute()) {
    if (!$res->fetchArray())
        exit("{ \"addevent\": \"error permission denied\" }");
} else {
    exit("{ \"addevent\": \"error {$sqlite->lastErrorMsg()}\" }");
}

$stmt = $sqlite->prepare("
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
        races.id=:raceid
    GROUP BY
        numbers.id
    ");
if (!$stmt) {
    die("Unable to prepare statement " . $sqlite->lastErrorMsg());
    }
$stmt->bindValue("raceid", $raceid);

$data = array();
if ($res = $stmt->execute()) {
    while ($row = $res->fetchArray()) {
        if (!is_null($row["tids"]))
            $row["tids"] = explode(",", $row["tids"]);
        $data[] = $row;
    }
    $res->close();
    echo "{ \"runners\": " . json_encode($data) . " }";
} else {
    printf("Error " . $sqlite->lastErrorMsg() . ". SQL: " . $sql);
}


?>
