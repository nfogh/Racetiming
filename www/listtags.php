<form action="deletetags.php" method="post">
    <h1>Existing RFID tags</h1>
    <div class="grid-container">
        <div class="grid-x grid-padding-x">
            <div class="medium-6 cell">
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
                <div class="text-right"><input type=submit class="button" name=delete value="Delete selected"></div>
            </div>
        </div>
    </div>
</form>
