<?php
    require '_init.php';

    if (!isset($_GET['raceid'])) {
        $title = 'Select race';
        $masthead_image = 'assets/races/smormosen.jpg';
        $masthead_text = 'Select race';

        require '_header.php';
?>
<div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="cell">
        <h1>Races</h1>
        </div>
        <div class='cell'>
            <?php
                if ($res = $db->query("SELECT id, name, description, start, finish FROM races ORDER BY finish DESC")) {
                    while ($row = $res->fetch_assoc())
                    {
                        $id = $row['id'];
                        $start = $row['start'];
                        $name = $row['name'];
                        $description = $row['description'];
                        printf("<div class='callout primary large'>");
                        printf("<p>{$start}</p>");
                        printf("<h2><a href='/listtimes.php?raceid={$id}'>{$name}</a></h2>");
                        printf("<p>{$description}</p>");
printf("</div>");
                    }
                
                    $res->close();
                }
            ?>
        </div>
    </div>
</div>
<?php
        require '_footer.php';
        exit();
    }
 
    $raceid = htmlspecialchars($_GET['raceid']);
    $raceid = mysqli_real_escape_string($db, $raceid);

    if ($res = $db->query('SELECT name, description, address, start, finish, ST_X(gpscoords) as latitude, ST_Y(gpscoords) as longitude from races WHERE id=' . $raceid)) {
        if ($row = $res->fetch_assoc()) {
            $name = $row["name"];
            $description = $row["description"];
            $address = $row["address"];
            $start = new DateTime($row["start"]);
            $finish = new DateTime($row["finish"]);
            $latitude = $row["latitude"];
            $longitude = $row["longitude"];
        } else {
            die("Unable to find race with the id {$raceid}");
        }
        $res->close();
    } else {
        die("Unable to excecute query");
    }

    $title = 'Runner times for ' . $name;
    $masthead_image = 'assets/races/smormosen.jpg';
    $masthead_text = $name;
    $extraheaders = "<meta http-equiv=\"refresh\" content=\"5\">";

    require '_header.php';
    $now = new DateTime();
?>

<?php
    // Get the highest number of laps to format the table
    if ($res = $db->query("SELECT runnerid, COUNT(*) as laps FROM events JOIN numbers ON (events.numberid = numbers.id) WHERE numbers.raceid={$raceid} GROUP BY runnerid ORDER BY laps DESC LIMIT 1")) {
        if ($row = $res->fetch_assoc())
            $maxlaps = $row["laps"] - 1;
        $res->close();
    }
?>

<?php
    // Get runner indexes for this run
    $runners = array();
    if ($res = $db->query('SELECT runners.id as id, numbers.number as number, runners.name as name, runners.surname as surname, numbers.expected_time as time, numbers.expected_laps as laps FROM numbers JOIN runners ON (runners.id = numbers.runnerid) WHERE numbers.raceid = ' . $raceid . ' ORDER BY number ASC')) {
        while ($row = $res->fetch_assoc())
            $runners[$row["number"]] = array(
                'name' => $row['name'],
                'id' => $row['id'],
                'time' => $row['time'],
                'laps' => $row['laps'],
		'surname' => $row['surname']
            );
        $res->close();
    } else {
        die($db->error);
    }
?>

<?php
    // Get the highest number of laps to format the table
    if ($res = $db->query('SELECT numbers.runnerid, COUNT(*) as laps FROM events JOIN numbers ON (numbers.id = events.numberid) WHERE raceid=' . $raceid . ' GROUP BY runnerid ORDER BY laps DESC LIMIT 1')) {
        if ($row = $res->fetch_assoc())
            $maxLaps = $row["laps"] - 1;
        else
            $maxLaps = 0;
        $res->close();
    }
?>

<div class="grid-x grid-padding-x grid-padding-y">
    <div class="cell small-8">
        <?php
            if ($now > $start && $now < $finish)
                printf('<div class="callout success"><h5>This race is ongoing</h5></div>');
            else if ($now < $start)
                printf('<div class="callout warning"><h5>This race will start ' . $start->format("Y-m-d H:i:s") . ' </h5></div>');
            else
                printf('<div class="callout primary"><h5>This race finished ' . $finish->format("Y-m-d H:i:s") . ' </h5></div>');
                
        ?>

        <table class="hover" width="80%">
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
                    $numLapsCreated = 0;
                    if ($res = $db->query("SELECT events.timestamp as timestamp, events.msecs as msecs, events.event as event, runners.name as name, runners.surname as surname, numbers.number as number, numbers.expected_laps as laps, numbers.expected_time as time FROM events JOIN numbers ON events.numberid = numbers.id JOIN runners ON runners.id = numbers.runnerid WHERE numbers.raceid=" . $raceid . " AND numbers.runnerid=" . $runner['id'] . " ORDER BY timestamp ASC, event ASC")) {
                        if ($row = $res->fetch_assoc()) {
                            printf("<td>" . $number . "</td><td>" . $row['name'] . " " . $row['surname'] . "</td>");
                            $timestamps = [DateTime::createFromFormat("Y-m-d H:i:s", $row["timestamp"])];
                            while ($row = $res->fetch_assoc())
                                array_push($timestamps, DateTime::createFromFormat("Y-m-d H:i:s", $row["timestamp"]));
                            $splits = [];
                            for ($idx = 0; $idx < count($timestamps) - 1; $idx++)
                                array_push($splits, $timestamps[$idx + 1]->getTimestamp() - $timestamps[$idx]->getTimestamp());

                            $total = end($timestamps)->getTimestamp() - $timestamps[0]->getTimestamp();

                            $kmh = number_format(count($splits)*1.75/$total*3600, 1);
                            $minkm = number_format(($total / 60) / (count($splits)*1.75), 1);
                            
                            printf("<td><b><span data-tooltip title='{$kmh} km/h ({$minkm} min/km)'>" . sprintf('%02d:%02d:%02d', ($total / 3600),($total / 60 % 60), $total % 60) . "</span></b></td>");
                            
                            foreach ($splits as $split) {
                                $kmh = number_format(1.75/$split*3600, 1);
                                $minkm = number_format(($split / 60) / 1.75, 1);
                                printf("<td><span data-tooltip title='{$kmh} km/h ({$minkm} min/km)' style='font-weight:regular'>" . sprintf('%02d:%02d:%02d', ($split / 3600),($split / 60 % 60), $split % 60) . "</span></td>");
                            }
                            
                             $numLapsCreated += count($splits);
                        } else {
                            printf("<td>{$number}</td><td>{$runner['name']} {$runner['surname']}</td><td><div style='color:gray'>00:{$runner['time']}:00</div></td>");
                            for ($idx = 0; $idx < $runner['laps']; $idx++)
                                printf("<td class='text-center'><div style='color:gray'>-</div></td>");
                            $numLapsCreated += $runner['laps'];
                        }

                        for ($idx = $numLapsCreated; $idx <= $maxLaps; $idx++)
                            printf('<td></td>');
                        $res->close();
                    }
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

<?php
                    if (is_dir("./assets/races/{$raceid}")) {
$images = array_filter(scandir("./assets/races/{$raceid}"), function($item) {
    return !is_dir("./assets/races/{$raceid}/" . $item) && pathinfo($item, PATHINFO_EXTENSION) == "JPG";
});
                        }
if ($images) {
?>
<div class="grid-x">
    <div class="cell small-1 large-3"></div>
    <div class="cell small-10 large-6">
    <h4>Images from event</h4>
<div class="orbit" role="region" aria-label="Pictures from BKL" data-orbit width=50%>
  <div class="orbit-wrapper">
    <div class="orbit-controls">
      <button class="orbit-previous"><span class="show-for-sr">Previous Slide</span>&#9664;&#xFE0E;</button>
      <button class="orbit-next"><span class="show-for-sr">Next Slide</span>&#9654;&#xFE0E;</button>
    </div>
    <ul class="orbit-container">
<?php
    $isactive = "is-active";
    foreach ($images as $image) {
        printf("<li class='{$isactive} orbit-slide'>");
        printf("<figure class='orbit-figure'>");
        printf("  <img class='orbit-image' src='/assets/races/{$raceid}/{$image}'>");
        printf("  <figcaption class='orbit-caption'>");
        if (file_exists("./assets/races/{$raceid}/{$image}.txt"))
            readfile("./assets/races/{$raceid}/{$image}.txt");
        printf("</figcaption>");
        printf("</figure>");
        printf("</li>");
        $isactive = "";
    }
?>
    </ul>
  </div>
</div>
    </div>
    <div class="cell small-1 large-3"></div>
    </div>
<?php } ?>
    </div>

    <div class="small-4">
        <div class="panel callout radius">
            <?= $description ?>
            <div class="grid-x">
                <div class="cell small-4"><h5>Start</h5></div>
                <div class="cell small-8"><?= $start->format("Y-m-d H:i:s") ?></div>
                <div class="cell small-4"><h5>Finish</h5></div>
                <div class="cell small-8"><?= $finish->format("Y-m-d H:i:s") ?></div>
                <div class="cell small-4"><h5>Address</h5></div>
                <div class="cell small-8"><?= $address ?></div>
                <div class="cell small-4"><h5>GPS coordinates</h5></div>
                <div class="cell small-8"><?= $latitude ?>, <?= $longitude ?></div>
                <div class="cell"><div id="map" style="width:100%; height:400px"></div></div>
            </div>
        </div>
    </div>
</div>

<script async
    src="https://maps.googleapis.com/maps/api/js?key=<?= $config['apikeys']['googlemaps'] ?>&callback=initMap">
</script>

<script>
function initMap() {
  const race = { lat: <?= $latitude ?>, lng: <?= $longitude ?> };
  const map = new google.maps.Map(document.getElementById("map"), {
    zoom: 15,
    center: race,
  });

  const marker = new google.maps.Marker({
    position: race,
    map: map,
  });
}

window.initMap = initMap;
</script>

<?php require '_footer.php' ?>

