<?php
$title = 'Randomize tagids';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Race admin";
require '_init.php';
require '_adminpage.php';
require '_header.php';
?>

<?php
if (empty($_GET['raceid']))
    $errorstring  = 'The ID the race was not set';
$raceid = sqlite_escape_string(htmlspecialchars($_GET['raceid']));

$sql = "SELECT id from numbers WHERE numbers.raceid={$raceid}";
if ($res = $sqlite->query($sql)) {
    $numbers = array();
    while ($row = $res->fetchArray(SQLITE3_ASSOC))
        $numbers[] = $row['id'];
}
else {
    die("<h2>Unable to get numbers. " . $sqlite->lastErrorMsg() . "</h2>");
}

$sql = "SELECT tags.numberid FROM tags JOIN numbers ON (tags.numberid = numbers.id) WHERE numbers.raceid={$raceid}";
if ($res = $sqlite->query($sql)) {
    $existingtags = array();
    while ($row = $res->fetchArray(SQLITE3_ASSOC))
        $existingtags[] = $row['numberid'];
}
else {
    die("<h2>Unable to get tags. " . $sqlite->lastErrorMsg() . "</h2>");
}

$missingnumbers = array_diff($numbers, $existingtags);

if (count($missingnumbers) > 0) {

$sql = "INSERT INTO tags (numberid, tid) VALUES";
$first = True;
foreach ($missingnumbers as $missingnumber) {
    if (!$first) {
        $sql = $sql . ",";
    } else {
        $first = False;
    }
    
    $sql = $sql . "({$missingnumber}, MD5(RAND()))";

}

if (!$sqlite->query($sql)) {
    die("<h2>Unable to get tags. " . $sqlite->lastErrorMsg() . "</h2>");
}
else {
    printf("Generated tags");
}
}
?>

<?php require '_footer.php' ?>
