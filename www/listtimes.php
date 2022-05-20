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

    require '_header.php';
    $now = new DateTime();

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

<div id="times">
</div>

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

<script async type="text/javascript"
    src="https://maps.googleapis.com/maps/api/js?key=<?= $config['apikeys']['googlemaps'] ?>&callback=initMap">
</script>

<script type="text/javascript">
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

<script type="text/javascript">
  $.ajax({url: "/generatetimetable.php?raceid=<?= $raceid ?>", success: function(result) {
    $("#times").html(result);
  }});

<?php if (($now > $start) && ($now < $end)) { ?>
  const interval = setInterval(function() {
      $.ajax({url: "/generatetimetable.php?raceid=<?= $raceid ?>", success: function(result){
          $("#times").html(result);
      }
       })}, 10000);
<?php } ?>
</script>

<?php require '_footer.php' ?>

