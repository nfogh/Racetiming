<form action="deleteadmins.php" method="post">
    <h1>Existing admins</h1>
<table>
<tr><td>Username</td><td>Creation date</td><td>Password</td></tr>
<?php
    if ($res = $sqlite->query("SELECT numbers.id as id, numbers.raceid as raceid, numbers.runnerid as runnerid, numbers.number as number, races.name as racename, runners.name as runnername FROM numbers JOIN races ON races.id = numbers.raceid JOIN runners ON runners.id = numbers.runnerid")) {
    
        while ($row = $res->fetchArray())
        {
            printf("<tr>");
            printf("<td>" . $row["racename"] . " [" . $row["raceid"] . "]</td>");
            printf("<td>" . $row["runnername"] . " [" . $row["runnerid"] . "]</td>");
            printf("<td>" . $row["number"] . "</td>\n");
            printf("<td><input type=checkbox name=check[%s]></td>", $row['id']);
            printf("</tr>");
        }
    
    }
?> 
</table>
<input type=submit name=delete value="Delete selected">
</form>
