<html>
<?php $db = new mysqli("db", "root", "e9w86036f78sd9", "racetiming"); ?>

<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/foundation-sites@6.7.4/dist/css/foundation.min.css" crossorigin="anonymous">
<script src="https://cdn.jsdelivr.net/npm/foundation-sites@6.7.4/dist/js/foundation.min.js" crossorigin="anonymous"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-backstretch/2.1.18/jquery.backstretch.min.js" crossorigin="anonymous"></script>
<link href="//cdnjs.cloudflare.com/ajax/libs/foundicons/3.0.0/foundation-icons.css" rel="stylesheet">
<script src="https://cdnjs.cloudflare.com/ajax/libs/foundation-datepicker/1.5.6/js/foundation-datepicker.min.js" crossorigin="anonymous"></script>
<link href="https://cdnjs.cloudflare.com/ajax/libs/foundation-datepicker/1.5.6/css/foundation-datepicker.min.css" rel="stylesheet">
 <head>
  <title>PHP Test</title>
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
    </ul>
  </div>
</div>

 <div id="masthead">
	<div class="row">
		<div class="small-12 columns">
			<a id="logo" href="/" title="BK Racetiming">
				<img src="/assets/images/logo.png" alt="BK Racetiming">
			</a>
		</div><!-- /.small-12.columns -->
	</div><!-- /.row -->
</div><!-- /#masthead -->

<hr>
 <h1>Races</h1>
<?php require "listraces.php" ?>
<?php require "insertrace.php" ?>

<hr>
<h1>Runners</h1>
<?php require "listrunners.php" ?>
<?php require "insertrunner.php" ?>

<hr>
<h1>RFID tags</h1>
<?php require "listtags.php" ?>
<?php require "inserttag.php" ?>

<hr>
<h1>Manage numbers</h1>
<?php require "listnumbers.php" ?>
<?php require "insertnumber.php" ?>

</body>

<script>
    $("#masthead").backstretch("/assets/images/masthead.jpg", {fade: 700});
    $(document).foundation();
</script>

</html>