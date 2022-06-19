<?php
require "_init.php";

if (!isset($_POST['apikey']))
    exit("{ \"addtag\": \"error: Missing apikey parameter\" }");
$apikey = $_POST["apikey"];

if (!isset($_POST['numberid']))
    exit("{ \"addtag\": \"error: Missing numberid parameter\" }");
$numberid = $_POST["numberid"];

$stmt = $db->prepare("
    SELECT 
        numbers.id
    FROM numbers
    JOIN permissions ON (numbers.raceid = permissions.raceid)
    JOIN rest_api_keys ON (rest_api_keys.adminid = permissions.adminid) 
    WHERE rest_api_keys.api_key=? AND numbers.id=?");
print("apikey={$apikey}<br>");
print("numberid={$numberid}<br>");

$stmt->bind_param("si", $apikey, $numberid);
if ($stmt->execute()) {
    $res = $stmt->get_result();
    if (!$res->fetch_assoc()) {
        die('PERMISSION DENIED. ' . $db->error);
    }
} else {
    exit("{ \"addevent\": \"error {$db->error}\" }");
}

if (!isset($_POST['tid']))
    exit("{ \"addtag\": \"error: Missing tid parameter\" }");
$tid = $_POST["tid"];

$stmt = $db->prepare("INSERT INTO tags (numberid, tid) VALUES(?, ?)");
$stmt->bind_param("is", $numberid, $tid);
if ($stmt->execute()) {
    exit("{ \"addtag\": \"success\" }");
} else {
    exit("{ \"addtag\": \"error: {$db->error}\" }");
}
?>
