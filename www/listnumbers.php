<form action="deletenumbers.php" method="post">
    <h1>Existing numbers</h1>
<table>
<tr><td>Race [ID]</td><td>Runner [ID]</td><td>Number</td></tr>
<?php
    if ($res = $db->query("SELECT numbers.raceid as raceid, numbers.runnerid as runnerid, numbers.number as number, races.name as racename, runners.name as runnername FROM numbers JOIN races ON races.id = numbers.raceid JOIN runners ON runners.id = numbers.runnerid")) {
    
        while ($row = $res->fetch_assoc())
        {
            printf("<tr>");
            printf("<td>" . $row["racename"] . " [" . $row["raceid"] . "]</td>");
            printf("<td>" . $row["runnername"] . " [" . $row["runnerid"] . "]</td>");
            printf("<td>" . $row["number"] . "</td>\n");
            printf("<td><input type=checkbox name=check[%s]></td>", $row[0]);
            printf("</tr>");
        }
    
        $res->close();
    }
?> 
</table>
<input type=submit name=delete value="Delete selected">
</form>
