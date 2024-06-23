<form action="deleterunners.php" method="post">
    <h1>Existing runners</h1>

    <div class="grid-container">
        <div class="grid-x grid-padding-x">
            <div class="medium-12 cell">
                <table>
                    <tr><td>ID</td><td>Name</td><td>Surname</td><td>Select</td></tr>
                        <?php
                            if ($res = $sqlite->query("SELECT * FROM runners")) {
                                while ($row = $res->fetchArray(SQLITE_ASSOC))
                                {
                                    printf('<tr>');
                                    printf('<td>' . $row['id'] . '</td>');
                                    printf('<td>' . $row['name'] . '</td>');
                                    printf('<td>' . $row['surname'] . '</td>');
                                    printf('<td><input type="checkbox" name="check[' . $row['id'] . ']"></td>');
                                    printf("</tr>");
                                }

                            }
                        ?> 
                </table>
                <div class="text-right"><input type=submit class="button" name=delete value="Delete selected"></div>
            </div>
        </div>
    </div>
</form>
