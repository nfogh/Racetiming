<?php
$config = parse_ini_file('config/config.ini', true);
mysqli_report(MYSQLI_REPORT_OFF);
$db = @new mysqli($config['database']['host'], $config['database']['user'], $config['database']['password'], $config['database']['database']);
if (mysqli_connect_errno())
    die('Unable to connect to database. Please update config.ini');
?>