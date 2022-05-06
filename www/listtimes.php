<?php $db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming"); ?>

<?php
    $raceid = htmlspecialchars($_GET["raceid"]);
    $raceid = mysqli_real_escape_string($db, $raceid);

    if ($res = $db->query("SELECT name from races WHERE id=" . $raceid)) {
        if ($row = $res->fetch_assoc()) {
            $racename = $row["name"];
        } else {
            printf("Unable to find race with the id " + $raceid);
            exit();
        }
        $res->close();
    }
?>

<html>
 <head>
  <title>Runner times for <?= $racename ?></title>
 </head>
 <body>

<?php
    // Get the highest number of laps to format the table
    if ($res = $db->query("SELECT runnerid, COUNT(*) as laps FROM events WHERE raceid=1 GROUP BY runnerid ORDER BY laps DESC LIMIT 1")) {
        if ($row = $res->fetch_assoc())
            $maxlaps = $row["laps"] - 1;
        $res->close();
    }
?>

<?php
    // Get runner indexes for this run
    $runnerids = [];
    if ($res = $db->query("SELECT runnerid FROM events WHERE events.raceid = " . $raceid . " GROUP BY events.runnerid")) {
        while ($row = $res->fetch_assoc())
            array_push($runnerids, $row["runnerid"]);
        $res->close();
    }
?>

<h1>Times for <?=$racename ?></h1>
<h1>Number of runners <?= count($runnerids) ?></h1>
<table>
<tr><td>Number</td><td>Name</td>
<?php 
    for ($lap = 0; $lap < $maxlaps; $lap++)
        printf("<td>Lap " . ($lap + 1) . "</td>");
?>
<?php
    foreach ($runnerids as $runnerid) {
        printf("</tr>");
        if ($res = $db->query("SELECT events.timestamp as timestamp, events.msecs as msecs, events.event as event, runners.name as name, numbers.number as number FROM events JOIN runners ON runners.id = events.runnerid JOIN numbers ON numbers.raceid = events.raceid AND numbers.runnerid = events.runnerid WHERE events.raceid=" . $raceid . " AND events.runnerid=" . $runnerid . " ORDER BY number ASC, event ASC")) {
            if ($row = $res->fetch_assoc()) {
                printf("<td>" . $row["number"] . "</td><td>" . $row["name"] . "</td>");
                $timestamps = [DateTime::createFromFormat("Y-m-d H:i:s", $row["timestamp"])];
                while ($row = $res->fetch_assoc())
                    array_push($timestamps, DateTime::createFromFormat("Y-m-d H:i:s", $row["timestamp"]));

                $splits = [];
                for ($idx = 0; $idx < count($timestamps) - 1; $idx++)
                    array_push($splits, $timestamps[$idx + 1]->getTimestamp() - $timestamps[$idx]->getTimestamp());

                foreach ($splits as $split)
                    printf("<td>" . sprintf('%02d:%02d:%02d', ($split / 3600),($split / 60 % 60), $split % 60) . "</td>");
            }
            $res->close();
        }
        printf("</tr>");
    }
?>
</table>

<h2>Select other race</h2>
<form action="listtimes.php" method="get">
    <select name="raceid">
    <?php
    if ($res = $db->query("SELECT name, id from races")) {
        while ($row = $res->fetch_assoc())
            printf("<option name=" . $row["id"] . ">" . $row["name"] . "</option>");
        $res->close();
    }
    ?>
    </select>
    <input type=submit value="Show race">
</form>

</body>
</html>
