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
$raceid = mysqli_real_escape_string($db, htmlspecialchars($_GET['raceid']));

$sql = "SELECT id from numbers WHERE numbers.raceid={$raceid}";
if ($res = $db->query($sql)) {
    $numbers = array();
    while ($row = $res->fetch_assoc())
        $numbers[] = $row['id'];

    $res->close();
}
else {
    die("<h2>Unable to get numbers. " . $db->error . "</h2>");
}

$sql = "SELECT tags.numberid FROM tags JOIN numbers ON (tags.numberid = numbers.id) WHERE numbers.raceid={$raceid}";
if ($res = $db->query($sql)) {
    $existingtags = array();
    while ($row = $res->fetch_assoc())
        $existingtags[] = $row['numberid'];

    $res->close();
}
else {
    die("<h2>Unable to get tags. " . $db->error . "</h2>");
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

if (!$db->query($sql)) {
    die("<h2>Unable to get tags. " . $db->error . "</h2>");
}
else {
    printf("Generated tags");
}
}
?>

<?php require '_footer.php' ?>
