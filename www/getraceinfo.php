<?php
require("_init.php");

if (!isset($_GET['apikey']))
    exit("Missing apikey parameter");
$apikey = sqlite_escape_string(htmlspecialchars($_GET["apikey"]));

if (!isset($_GET['raceid']))
    exit("Missing raceid parameter");
$raceid = sqlite_escape_string(htmlspecialchars($_GET["raceid"]));

if ($res = $sqlite->query('SELECT permissions.raceid as raceid from permissions JOIN rest_api_keys ON (rest_api_keys.adminid = permissions.adminid) WHERE rest_api_keys.api_key = "' . $apikey . '" AND raceid=' . $raceid)) {
    if (!($row = $res->fetchArray(SQLITE_ASSOC))) {
        printf('PERMISSION DENIED');
        exit();
    }
} else {
    printf("Error " . $sqlite->lastErrorMsg());
}

$sql = 'SELECT numbers.number as runnernumber, runners.name as runnername, runners.id as runnerid, runners.surname as runnersurname FROM numbers JOIN races ON (races.id = numbers.raceid) JOIN runners ON (runners.id = numbers.runnerid) WHERE races.id=' . $raceid;

$data = array();
if ($res = $sqlite->query($sql)) {
    //printf('{"runners": [');
    while ($row = $res->fetchArray(SQLITE_ASSOC)) {
        $data[] = $row;
        //printf("{\n" .
        //    '"runnernumber":' . $row['runnernumber'] . ",\n" .
        //    '"runnername":"'   . $row['runnername'] . "\",\n" .
        //    '"runnersurname":"'   . $row['runnersurname'] . "\",\n" .
        //    '"runnerid":'     . $row['runnerid'] . "\n" .
        //"},\n");
    }
    echo json_encode($data);
    //printf(']}');
} else {
    printf("Error " . $sqlite->lastErrorMsg() . ". SQL: " . $sql);
}


?>
