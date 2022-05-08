<form action="deleterace.php" method="post">
    <h1>Existing races</h1>
<div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="cell">
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
            <div class="text-right">
                <input type=submit class="button" name=delete value="Delete selected">
            </div>
        </div>
    </div>
</div>
</form>
