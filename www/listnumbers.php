<form action="deletenumbers.php" method="post">
    <h1>Existing numbers</h1>
    <div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="cell">
<table>
<tr><td>Race [ID]</td><td>Runner [ID]</td><td>Number</td><td>Expected laps</td><td>Expected time [min]</td></tr>
<?php
    if ($res = $db->query("SELECT numbers.id as id, numbers.raceid as raceid, numbers.runnerid as runnerid, numbers.number as number, numbers.expected_laps as laps, numbers.expected_time as time, races.name as racename, runners.name as runnername, runners.surname as runnersurname FROM numbers JOIN races ON races.id = numbers.raceid JOIN runners ON runners.id = numbers.runnerid ORDER BY raceid, number")) {
    
        while ($row = $res->fetch_assoc())
        {
            printf("<tr>");
            printf("<td>{$row['racename']} [{$row['raceid']}]</td>");
            printf("<td>{$row['runnername']} {$row['runnersurname']} [{$row['runnerid']}]</td>");
            printf("<td>{$row['number']}</td>\n");
            printf("<td>{$row['laps']}</td>\n");
            printf("<td>{$row['time']}</td>\n");
            printf("<td><input type=checkbox name=check[${row['id']}]></td>");
            printf("</tr>");
        }
    
        $res->close();
    }
?> 
</table>
<div class="text-right"><input class="button" type=submit name=delete value="Delete selected"></div>
</form>
</div>
</div>
</div>
