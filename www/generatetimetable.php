<?php
    require '_init.php';

    $raceid = $_GET['raceid'];

    $stmt = $db->prepare("
        SELECT
            lap_length,
            start
        FROM
            races
        WHERE
            races.id=?");
    $stmt->bind_param("i", $raceid);
    if ($stmt->execute()) {
        $res = $stmt->get_result();
        if ($row = $res->fetch_assoc()) {
            $lap_length = $row['lap_length'];
            $start = DateTime::createFromFormat("Y-m-d H:i:s", $row['start']);
        }
    }
    $stmt->close();

    $stmt = $db->prepare("
        SELECT
            numbers.number,
            CONCAT(runners.name, ' ', runners.surname) AS name,
            eventsx.events,
            numbers.expected_time AS expected_time,
            numbers.expected_laps AS expected_laps,
            teams.name as teamname
        FROM
            runners 
        JOIN
            numbers ON (numbers.runnerid = runners.id)
        LEFT JOIN
            (
                SELECT 
                    numbers.id as numberid,
                    GROUP_CONCAT(CONCAT(events.timestamp, '|', events.event) ORDER BY events.timestamp SEPARATOR ',') AS events
                FROM
                    events
                JOIN
                    numbers ON (numbers.id = events.numberid)
                WHERE
                    numbers.raceid = ?
                GROUP BY
                    numberid
            ) eventsx
        ON
            eventsx.numberid = numbers.id
        LEFT JOIN
            teams ON teams.id=numbers.teamid
        WHERE
            numbers.raceid = ?
        ORDER BY
            teams.id DESC,numbers.number ASC");

    if (!$stmt)
        die("Unable to execute statement " . $db->error);

    $maxLaps = 0;
    $stmt->bind_param("ii", $raceid, $raceid);
    if ($stmt->execute()) {
        $res = $stmt->get_result();
        $runners = Array();
        while ($row = $res->fetch_assoc()) {
            $number = $row["number"];
            $events = Array();

            if (!empty($row['events'])) {
            $eventsList = explode(",", $row['events']);
            
            foreach ($eventsList as $event) {
                list($timestamp, $type) = explode("|", $event);
                $events[] = Array(
                    "timestamp" => DateTime::createFromFormat("Y-m-d H:i:s", $timestamp),
                    "type" => $type);
            }
            if (count($events) - 1 > $maxLaps)
                $maxLaps = count($events) - 1;
            }

            $runners[$number] = Array(
                "name" => $row["name"],
                "teamname" => $row["teamname"],
                "events" => $events,
                "expected_time" => $row["expected_time"],
                "expected_laps" => $row["expected_laps"]);
         }
    } else {
        die("Unable to execute query");
    }
    $stmt->close();
    $now = new DateTime();
?>

<table>
<thead>
    <tr><th>&#35;</th><th>Name</th><th>Team</th>
<?php
    if ($now > $start) {
        printf("<th>Total</th>");
        for ($lap = 0; $lap < $maxLaps; $lap++)
            printf("<th>Lap " . ($lap + 1) . "</th>");
        printf("</tr></thead>");
        foreach ($runners as $number => $runner) {
            printf("<tr>");
            printf("<td>{$number}</td><td>{$runner['name']}</td><td>{$runner['teamname']}</td>");
            $events = $runner['events'];
            if (count($events) > 0) {
                $splits = Array();
                for ($idx = 0; $idx < count($events) - 1; $idx++)
                    array_push($splits, $events[$idx + 1]["timestamp"]->getTimestamp() - $events[$idx]["timestamp"]->getTimestamp());
                
                $total = end($events)["timestamp"]->getTimestamp() - $events[0]["timestamp"]->getTimestamp();
                
                if (!is_null($lap_length)) {
                    if (count($splits) > 0) {
                        $kmh = number_format(count($splits)*$lap_length/$total*3600, 1);
                        $minkm = number_format(($total / 60) / (count($splits)*$lap_length), 1);
                    } else {
                        $kmh = 0;
                        $minkm = 0;
                    }
                    printf("<td><b><span data-tooltip title='{$kmh} km/h ({$minkm} min/km)'>" . sprintf('%02d:%02d:%02d', ($total / 3600),($total / 60 % 60), $total % 60) . "</span></b></td>");
                    
                    foreach ($splits as $split) {
                        $kmh = number_format($lap_length/$split*3600, 1);
                        $minkm = number_format(($split / 60) / $lap_length, 1);
                        printf("<td><span data-tooltip title='{$kmh} km/h ({$minkm} min/km)' style='font-weight:regular'>" . sprintf('%02d:%02d:%02d', ($split / 3600),($split / 60 % 60), $split % 60) . "</span></td>");
                    }
                }
                
                $numLapsCreated += count($splits);
            } else {
                printf("<td><div style='color:gray'><b>00:{$runner['expected_time']}:00</b></div></td>");
                $numLapsCreated += 1;
            }
            for ($idx = $numLapsCreated; $idx <= $maxLaps; $idx++)
                printf('<td></td>');
                                                                          
            printf("</tr>\n");
        }
    } else {
        printf("<th>Expected laps</th><th>Expected time [min]</th></tr></thead>");
        foreach ($runners as $number => $runner) {
            printf("<tr>");
            printf("<td>{$number}</td>");
            printf("<td>{$runner['name']} {$runner['surname']}</td>");
            printf("<td><font color={$runner['teamname']}>{$runner['teamname']}</color></td>");
            printf("<td>{$runner['expected_laps']}</td>");
            printf("<td>{$runner['expected_time']}</td>");
            printf("</tr>");
        }
    }
?>
</table>
<div>Number of runners <b><?= count($runners) ?></b></div>
<div class="text-right">Updated at <?=$now->format("Y-m-d H:i:s") ?></div>
