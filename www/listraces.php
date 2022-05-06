<form action="deleterace.php" method="post">
    <h1>Existing races</h1>
<table>
<tr><td>ID</td><td>Name</td><td>Select</td></tr>
<?php
    if ($res = $db->query("SELECT id, name FROM races")) {

    
        while ($row = $res->fetch_row())
        {
            printf("<tr>");
            printf("<td>%s</td> <td>%s</td>\n", $row[0], $row[1]);
            printf("<td><input type=checkbox name=check[%s]></td>", $row[0]);
            printf("</tr>");
        }
    
        $res->close();
    }
?> 
</table>
<input type=submit name=delete value="Delete selected">
</form>
