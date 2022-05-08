<?php
    require '_init.php';

    if (!isset($_GET['raceid'])) {
        $title = 'Select race';
        $masthead_image = 'assets/races/smormosen.jpg';
        $masthead_text = 'Select race';

        require '_header.php';
        printf('<div class="callout large">Please select a race from the dropdown');
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
            die("Unable to find race with the id " . $raceid);
        }
        $res->close();
    } else {
        die("Unable to excecute query");
    }

    $title = 'Runner times for ' . $name;
    $masthead_image = 'assets/races/smormosen.jpg';
    $masthead_text = $name;

    require '_header.php';
    $now = new DateTime();
?>

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
    $runners = array();
    if ($res = $db->query('SELECT runners.id as id, numbers.number as number, runners.name as name FROM numbers JOIN runners ON (runners.id = numbers.runnerid) WHERE numbers.raceid = ' . $raceid . ' ORDER BY number ASC')) {
        while ($row = $res->fetch_assoc())
            $runners[$row["number"]] = array(
                'name' => $row['name'],
                'id' => $row['id'],
            );
        $res->close();
    } else {
        die($db->error);
    }
?>

<?php
    // Get the highest number of laps to format the table
    if ($res = $db->query('SELECT runnerid, COUNT(*) as laps FROM events WHERE raceid=' . $raceid . ' GROUP BY runnerid ORDER BY laps DESC LIMIT 1')) {
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
        ?>

        <table class="hover" width="80%">
        <thead>
            <tr><th width=100>Number</th><th>Name</th>
            <?php 
                for ($lap = 0; $lap < $maxLaps; $lap++)
                    printf("<th>Lap " . ($lap + 1) . "</th>");
            ?>
            </tr>
        </thead>
        <?php
            foreach ($runners as $number => $runner) {
                printf("</tr>");
                $numLapsCreated = 0;
                if ($res = $db->query("SELECT events.timestamp as timestamp, events.msecs as msecs, events.event as event, runners.name as name, numbers.number as number FROM events JOIN runners ON runners.id = events.runnerid JOIN numbers ON numbers.raceid = events.raceid AND numbers.runnerid = events.runnerid WHERE events.raceid=" . $raceid . " AND events.runnerid=" . $runner['id'] . " ORDER BY timestamp ASC, event ASC")) {
                    if ($row = $res->fetch_assoc()) {
                        printf("<td>" . $number . "</td><td>" . $runner['name'] . "</td>");
                        $timestamps = [DateTime::createFromFormat("Y-m-d H:i:s", $row["timestamp"])];
                        while ($row = $res->fetch_assoc())
                            array_push($timestamps, DateTime::createFromFormat("Y-m-d H:i:s", $row["timestamp"]));
                    
                        $splits = [];
                        for ($idx = 0; $idx < count($timestamps) - 1; $idx++)
                            array_push($splits, $timestamps[$idx + 1]->getTimestamp() - $timestamps[$idx]->getTimestamp());
                    
                        foreach ($splits as $split)
                            printf("<td>" . sprintf('%02d:%02d:%02d', ($split / 3600),($split / 60 % 60), $split % 60) . "</td>");
                        
                         $numLapsCreated += count($splits);
                    } else {
                        printf("<td>" . $number . "</td><td>" . $runner['name'] . "</td>");
                    }

                    for ($idx = $numLapsCreated; $idx <= $maxLaps; $idx++)
                        printf('<td></td>');
                    $res->close();
                }
                printf("</tr>");
            }
        ?>
    </table>
    <p>Updated at <?=$now->format("Y-m-d H:i:s") ?></p>
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

    <div class='cell'>
        <form action="listtimes.php" method="get">
        <h2>Select other race</h2>
        <div class="input-group">
            <span class="input-group-label">Select other race</span>
            <select class="input-group-field" type="text" name="raceid">
            <?php
              if ($res = $db->query("SELECT id, name from races")) {
                  while ($row = $res->fetch_assoc())
                      printf('<option value="' . $row['id'] . '">' . $row['name'] . "</option>");
                  $res->close();
              }
              ?>
              </select>
            <div class="input-group-button">
                <input type=submit class="button" value="Show race">
            </div>
        </div>
        </form>
    </div>
</div>

<script async
    src="https://maps.googleapis.com/maps/api/js?key=AIzaSyDACthgF53sdT7EO2ZdlBFqzdnDMK6LpAo&callback=initMap">
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

