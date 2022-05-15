<form action="deletetags.php" method="post">
    <h1>Existing RFID tags</h1>
    <div class="grid-container">
        <div class="grid-x grid-padding-x">
            <div class="medium-6 cell">
                <table>
                    <tr><td>Race</td><td>Number</td><td>Name</td><td>TID</td><td>Select</td></tr>
                    <?php
                        if ($res = $db->query("SELECT rfidtags.numberid as numberid, rfidtags.tid as tid, rfidtags.id as id, numbers.number as number, numbers.raceid as raceid, numbers.runnerid as runnerid, runners.name as runnername, runners.surname as runnersurname, races.name as racename FROM rfidtags JOIN numbers ON (numbers.id = rfidtags.numberid) JOIN runners ON (runners.id = numbers.runnerid) JOIN races ON (races.id = numbers.raceid)")) {
                        
                            while ($row = $res->fetch_assoc()) {
                                printf("<tr>");
                                printf("<td>{$row['racename']}</td><td>{$row['number']}</td><td>{$row['runnername']} {$row['runnersurname']}</td><td>{$row['tid']}</td>\n");
                                printf("<td><input type=checkbox name=check[{$row['id']}]></td>");
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
