<?php
require "_init.php";

if (!isset($_POST['apikey']))
    exit("{ \"addtag\": \"error: Missing apikey parameter\" }");
$apikey = $_POST["apikey"];

if (!isset($_POST['numberid']))
    exit("{ \"addtag\": \"error: Missing numberid parameter\" }");
$numberid = $_POST["numberid"];

$stmt = $sqlite->prepare("
    SELECT 
        numbers.id
    FROM numbers
    JOIN permissions ON (numbers.raceid = permissions.raceid)
    JOIN rest_api_keys ON (rest_api_keys.adminid = permissions.adminid) 
    WHERE rest_api_keys.api_key=:apikey AND numbers.id=:numberid");
print("apikey={$apikey}<br>");
print("numberid={$numberid}<br>");

$stmt->bindValue("apikey", $apikey);
$stmt->bindValue("numberid", $numberid);
if ($res = $stmt->execute()) {
    if (!$res->fetchArray(SQLITE3_ASSOC)) {
        die('PERMISSION DENIED. ' . $sqlite->lastErrorMsg());
    }
} else {
    exit("{ \"addevent\": \"error {$sqlite->lastErrorMsg()}\" }");
}

if (!isset($_POST['tid']))
    exit("{ \"addtag\": \"error: Missing tid parameter\" }");
$tid = $_POST["tid"];

$stmt = $sqlite->prepare("INSERT INTO tags (numberid, tid) VALUES(:numberid, :tid)");
$stmt->bindValue("numberid", $numberid);
$stmt->bindValue("tid", $tid);
if ($stmt->execute()) {
    exit("{ \"addtag\": \"success\" }");
} else {
    exit("{ \"addtag\": \"error: {$sqlite->lastErrorMsg()}\" }");
}
?>
