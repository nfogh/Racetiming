<?php
$title = 'Admin interface';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "BK Health";
require '_init.php';
require '_adminpage.php';
require '_header.php';

if($_SERVER["REQUEST_METHOD"] == "POST") {
    if (empty($_POST['id']))
        $errorstring  = 'The id of the race was not set';
    $id = mysqli_real_escape_string($db, htmlspecialchars($_POST["id"]));

    if (empty($_POST['name']))
        $errorstring  = 'The name of the race was not set';
    $name = mysqli_real_escape_string($db, htmlspecialchars($_POST["name"]));

    if (empty($_POST['description']))
        $errorstring = 'The description of the race was not set';
    $description = mysqli_real_escape_string($db, htmlspecialchars($_POST["description"]));

    if (empty($_POST['start']))
        $errorstring = 'The start of the race was not set';
    $start = mysqli_real_escape_string($db, htmlspecialchars($_POST["start"]));

    if (empty($_POST['finish']))
        $errorstring = 'The finish of the race was not set';
    $finish = mysqli_real_escape_string($db, htmlspecialchars($_POST["finish"]));

    if (empty($_POST['address']))
        $errorstring = 'The address of the race was not set';
    $address = mysqli_real_escape_string($db, htmlspecialchars($_POST["address"]));

    if (empty($_POST['latitude']))
        $errorstring = 'The latitude of the race was not set';
    $latitude = mysqli_real_escape_string($db, htmlspecialchars($_POST["latitude"]));

    if (empty($_POST['longitude']))
        $errorstring  = 'The longitude of the race was not set';
    $longitude = mysqli_real_escape_string($db, htmlspecialchars($_POST['longitude']));

    if (!isset($errorstring)) {
        $sql = "UPDATE races SET name='{$name}', description='{$description}', start='${start}', finish='{$finish}', address='{$address}', gpscoords=POINT({$latitude},{$longitude}) WHERE id={$id}";

        if (!$db->query($sql))
            $errorstring = '<div class="callout large alert"><h5>Unable to update race. ' . $db->error . "</h5></div>";
        else
            $successstring = '<div class="callout large success"><h5>Updated race ' . $name . '</h5></div>';
    } else {
        $errorstring = '<div class="callout large alert"><h5>Error: ' . $errorstring . '</h5></div>';
    }
} else {
    if (empty($_GET['id']))
        $errorstring  = 'The ID of the race was not specified';
    $id = mysqli_real_escape_string($db, htmlspecialchars($_GET['id']));
}

if (!$errorstring) {
    $sql = "SELECT name, description, start, finish, address, ST_X(gpscoords) as latitude, ST_Y(gpscoords) as longitude FROM races WHERE id=${id}";
    if ($res = $db->query($sql)) {
        if ($row = $res->fetch_assoc())
        {
            $name = $row['name'];
            $description = $row['description'];
            $start = $row['start'];
            $finish = $row['finish'];
            $address = $row['address'];
            $latitude = $row['latitude'];
            $longitude = $row['longitude'];
        } else {
            $errorstring = "Unable to get race information";
        }
        $res->close();
    } else {
        $errorstring = "Unable to get race information. " . $db->error;
    }
}

if (isset($errorstring)) {
    printf("<div class=\"callout large alert\">{$errorstring}</div>");
    require '_footer.php';
    exit();
}

if (isset($successstring))
    printf("<div class=\"callout large success\">{$successstring}</div>");
?>


<h1>Edit race</h1>
<form action="editrace.php" method=post>
    <input type="hidden" name="id" value="<?=$id?>">
<div class="grid-container">
    <div class="grid-x grid-padding-x">
      <div class="medium-12 cell">
        <label>Name
          <input type="text" name="name" value="<?=$name ?>">
        </label>
      </div>
      <div class="medium-12 cell">
        <label>Desciption
          <textarea name="description"><?=$description ?></textarea>
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Start date/time
          <input type="datetime" name="start" id="start" value="<?=$start ?>">
          <script>
            $(function(){
                $('#start').fdatepicker({
		            format: 'yyyy-mm-dd hh:ii:ss',
		            disableDblClickSelection: true,
		            pickTime: true}); });
          </script>
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Finish date/time
        <input type="datetime" name="finish" id="finish" value="<?=$finish ?>">
          <script>
            $(function(){
                $('#finish').fdatepicker({
		            format: 'yyyy-mm-dd hh:ii:ss',
		            disableDblClickSelection: true,
		            pickTime: true}); });
          </script>
        </label>
      </div>
      <div class="medium-12 cell">
        <label>Address
          <textarea name="address"><?=$address ?></textarea>
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Latitude
        <input type="text" name="latitude" value="<?=$latitude ?>">
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Longitude
        <input type="text" name="longitude" value="<?=$longitude ?>">
        </label>
      </div>
      <div class="medium-12 cell">
        <div class="text-right">
          <input type="submit" class="button" name="edit" value="Modify">
        </div>
      </div>
    </div>
  </div>
</form>

<?php require '_footer.php' ?>