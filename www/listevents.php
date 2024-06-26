<form action="deleteevents.php" method="post">
    <h1>Existing events</h1>
    <div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="cell">
<table>
<tr><td>Race name</td><td>Runner name</td><td>Timestamp</td><td>Event type</td></tr>
<?php
    $eventmap = array(
        1 => "Lap/Start",
	    2 => "Finish");

    $sql = "
        SELECT 
            events.id as id, 
            events.timestamp as timestamp, 
            events.msecs as msecs, 
            events.event as event, 
            runners.name as name, 
            runners.surname as surname, 
            races.name as racename 
        FROM events 
            JOIN numbers ON (events.numberid  = numbers.id) 
            JOIN runners ON (numbers.runnerid = runners.id)
            JOIN races   ON (numbers.raceid   = races.id)";
    if ($res = $sqlite->query($sql)) {
    
        while ($row = $res->fetchArray(SQLITE3_ASSOC))
        {
            printf("<tr>");
            printf("<td>{$row['racename']}</td>");
            printf("<td>{$row['name']} {$row['surname']}</td>");
            printf("<td>{$row['timestamp']}.{$row['msecs']}</td>");
            printf("<td>{$eventmap[$row['event']]}</td>\n");
            printf("<td><input type=checkbox name=check[${row['id']}]></td>");
            printf("</tr>");
        }
    
    } else {
      printf("<div class='callout large event'>{$sqlite->lastErrorMsg()}</div>");
    }
?> 
</table>
<div class="text-right"><input class="button" type=submit name=delete value="Delete selected"></div>
</form>
</div>
</div>
</div>
