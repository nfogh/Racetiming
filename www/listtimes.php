<?php
    $db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming"); 
    $now = new DateTime();
?>

<?php
    $raceid = htmlspecialchars($_GET["raceid"]);
    $raceid = mysqli_real_escape_string($db, $raceid);

    if ($res = $db->query("SELECT name, description, address, start, finish, gpscoords from races WHERE id=" . $raceid)) {
        if ($row = $res->fetch_assoc()) {
            $name = $row["name"];
            $description = $row["description"];
            $address = $row["address"];
            $start = new DateTime($row["start"]);
            $finish = new DateTime($row["finish"]);
            $gpscoords = $row["gpscoords"];
        } else {
            printf("Unable to find race with the id " + $raceid);
            exit();
        }
        $res->close();
    }
?>

<html>
<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/foundation-sites@6.7.4/dist/css/foundation.min.css" crossorigin="anonymous">
<script src="https://cdn.jsdelivr.net/npm/foundation-sites@6.7.4/dist/js/foundation.min.js" crossorigin="anonymous"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-backstretch/2.1.18/jquery.backstretch.min.js" crossorigin="anonymous"></script>
<script src="https://polyfill.io/v3/polyfill.min.js?features=default" crossorigin="anonymous"></script>



<script>
// Initialize and add the map
function initMap() {
  // The location of Uluru
  const uluru = { lat: -25.344, lng: 131.031 };
  // The map, centered at Uluru
  const map = new google.maps.Map(document.getElementById("map"), {
    zoom: 4,
    center: uluru,
  });
  // The marker, positioned at Uluru
  const marker = new google.maps.Marker({
    position: uluru,
    map: map,
  });
}

window.initMap = initMap;    
</script>

 <head>
  <title>Runner times for <?= $racename ?></title>
 </head>
 <body>

 
<div class="top-bar">
  <div class="top-bar-left">
    <ul class="dropdown menu" data-dropdown-menu>
      <li class="menu-text">BK Health timing</li>
      <li>
        <a href="#">Races</a>
        <ul class="menu vertical">
<?php
    if ($res = $db->query('SELECT name, id from races ORDER BY start DESC LIMIT 10')) {
        while ($row = $res->fetch_assoc())
            printf('<li><a href="/listtimes.php?raceid=' . $row['id'] . '">' . $row['name'] . '</a></li>');
        $res->close();
    }
?>
        </ul>
      </li>
    </ul>
  </div>
  <div class="top-bar-right">
    <ul class="menu">
      <li><a href="#">Info</a></li>
      <li><a href="/admin.php">Admin</a></li>
    </ul>
  </div>
</div>

 <div id="masthead">
	<div class="row">
		<div class="small-12 columns">
            <div style="height: 200px; font-size: 100px;
  padding-top: 25px; padding-left:100px; text-shadow: 3px 3px #000000;
  color: #008700;"><?= $name ?></div>
		</div><!-- /.small-12.columns -->
	</div><!-- /.row -->
</div><!-- /#masthead -->

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
    if ($res = $db->query("SELECT runners.id as id, numbers.number as number, runners.name as name FROM numbers JOIN runners ON (runners.id = numbers.runnerid) WHERE numbers.raceid = " . $raceid . " ORDER BY number ASC")) {
        while ($row = $res->fetch_assoc())
            $runners[$row["number"]] = array(
                'name' => $row['name'],
                'id' => $row['id'],
            );
        $res->close();
    } else {
        printf($db->error());
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
                <div class='cell'>Start <?= $start->format("Y-m-d H:i:s") ?></div>
                <div class='cell'>Finish <?= $finish->format("Y-m-d H:i:s") ?></div>
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

<script>
    $("#masthead").backstretch("/assets/races/smormosen.jpg", {fade: 700});
    $(document).foundation();
</script>

</body>

</html>
