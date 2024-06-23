<form action="deleterace.php" method="post">
    <h1>Existing races</h1>
<div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="cell">
            <table>
            <tr><td>ID</td><td>Name</td><td>Select</td></tr>
            <?php
                if ($res = $sqlite->query("SELECT id, name FROM races")) {
                    while ($row = $res->fetchArray(SQLITE_ASSOC))
                    {
                        $id = $row['id'];
                        $name = $row['name'];
                        printf("<tr>");
                        printf("<td>{$id}</td> <td><a href='/editrace.php?id={$id}'>{$name}</a></td>\n");
                        printf("<td><input type=checkbox name=check[{$id}]></td>");
                        printf("</tr>");
                    }

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
