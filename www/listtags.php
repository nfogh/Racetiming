<form action="deletetags.php" method="post">
    <h1>Existing RFID tags</h1>
<table>
    <tr><td>Runner ID</td><td>Name</td><td>TID</td><td>Select</td></tr>
<?php
    if ($res = $db->query("SELECT rfidtags.runnerid, rfidtags.tid, runners.name FROM rfidtags JOIN runners ON runners.id = rfidtags.runnerid")) {

        while ($row = $res->fetch_row()) {
            printf("<tr>");
            printf("<td>%s</td><td>%s</td><td>0x%s</td>\n", $row[0], $row[2], strtoupper(bin2hex($row[1])));
            printf("<td><input type=checkbox name=check[%s]></td>", bin2hex($row[1]));
            printf("</tr>");
        }
    
        $res->close();
    }
?> 
</table>
<input type=submit name=delete value="Delete selected">
</form>
