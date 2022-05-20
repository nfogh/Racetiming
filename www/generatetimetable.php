<?php
    require '_init.php';

    $raceid = htmlspecialchars($_GET['raceid']);
    $raceid = mysqli_real_escape_string($db, $raceid);

    $maxLaps = 0;
    if ($res = $db->query("SELECT numbers.number, CONCAT(runners.name, ' ', runners.surname) AS name, x.events, numbers.expected_time AS expected_time FROM runners JOIN numbers ON (numbers.runnerid = runners.id) LEFT JOIN (SELECT numbers.id as numberid, GROUP_CONCAT(CONCAT(events.timestamp, '|', events.event) ORDER BY events.timestamp SEPARATOR ',') AS events FROM events JOIN numbers ON (numbers.id = events.numberid) WHERE (numbers.raceid = {$raceid}) GROUP BY numberid) x ON (x.numberid = numbers.id) WHERE (numbers.raceid = {$raceid}) ORDER BY numbers.number ASC")) {
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
                "events" => $events,
                "expected_time" => $row["expected_time"]);
        }
    } else {
        die("Unable to execute query");
    }
    $now = new DateTime();
?>
<table>
<thead>
    <tr><th>&#35;</th><th>Name</th>
    <?php
         if ($now > $start) {
             printf("<th>Total</th>");
             for ($lap = 0; $lap < $maxLaps; $lap++)
                 printf("<th>Lap " . ($lap + 1) . "</th>");
         } else {
            printf("<th>Expected laps</th><th>Expected time [min]</th>");
         }
    ?>
    </tr>
</thead>
<?php
    if ($now > $start) {
        foreach ($runners as $number => $runner) {
           printf("<tr>");
           printf("<td>{$number}</td><td>{$runner['name']}</td>");
           $events = $runner['events'];
           if (count($events) > 0) {
               $splits = Array();
               for ($idx = 0; $idx < count($events) - 1; $idx++)
                   array_push($splits, $events[$idx + 1]["timestamp"]->getTimestamp() - $events[$idx]["timestamp"]->getTimestamp());
               
               $total = end($events)["timestamp"]->getTimestamp() - $events[0]["timestamp"]->getTimestamp();
               
               if (count($splits) > 0) {
                   $kmh = number_format(count($splits)*1.75/$total*3600, 1);
                   $minkm = number_format(($total / 60) / (count($splits)*1.75), 1);
               } else {
                   $kmh = 0;
                   $minkm = 0;
               }
               printf("<td><b><span data-tooltip title='{$kmh} km/h ({$minkm} min/km)'>" . sprintf('%02d:%02d:%02d', ($total / 3600),($total / 60 % 60), $total % 60) . "</span></b></td>");
               
               foreach ($splits as $split) {
                   $kmh = number_format(1.75/$split*3600, 1);
                   $minkm = number_format(($split / 60) / 1.75, 1);
                   printf("<td><span data-tooltip title='{$kmh} km/h ({$minkm} min/km)' style='font-weight:regular'>" . sprintf('%02d:%02d:%02d', ($split / 3600),($split / 60 % 60), $split % 60) . "</span></td>");
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
        foreach ($runners as $number => $runner) {
            printf("<tr>");
            if ($res = $db->query("SELECT numbers.number as number, numbers.expected_laps as laps, numbers.expected_time as time FROM numbers WHERE numbers.raceid = {$raceid} AND numbers.runnerid={$runner['id']}")) {
                if ($row = $res->fetch_assoc()) {
                    printf("<td>{$number}</td>");
                    printf("<td>{$runner['name']} {$runner['surname']}</td>");
                    printf("<td>{$row['laps']}</td>");
                    printf("<td>{$row['time']}</td>");
                }
            }
            printf("</tr>");
        }
    }
?>
    </table>
    <div class="text-right">Updated at <?=$now->format("Y-m-d H:i:s") ?></div>
