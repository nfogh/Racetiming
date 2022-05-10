<form action="deleteevents.php" method="post">
    <h1>Existing events</h1>
    <div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="cell">
<table>
<tr><td>Race ID</td><td>Runner name</td><td>Timestamp</td><td>Event type</td></tr>
<?php
    $eventmap = array(
        1 => "Lap/Start",
	2 => "Finish");

    if ($res = $db->query("SELECT events.id as id, events.raceid as raceid, events.timestamp as timestamp, events.msecs as msecs, events.event as event, runners.name as name, runners.surname as surname FROM events JOIN runners ON (events.runnerid = runners.id)")) {
    
        while ($row = $res->fetch_assoc())
        {
            printf("<tr>");
            printf("<td>{$row['raceid']}</td>");
            printf("<td>{$row['name']} {$row['surname']}</td>");
            printf("<td>{$row['timestamp']}.{$row['msecs']}</td>");
            printf("<td>{$eventmap[$row['event']]}</td>\n");
            printf("<td><input type=checkbox name=check[${row['id']}]></td>");
            printf("</tr>");
        }
    
        $res->close();
    } else {
      printf("<div class='callout large event'>{$db->error}</div>");
    }
?> 
</table>
<div class="text-right"><input class="button" type=submit name=delete value="Delete selected"></div>
</form>
</div>
</div>
</div>
