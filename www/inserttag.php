<h1>Attach RFID tag</h1>
<div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="medium-6 cell">
            <form action="addtag.php" method=post>
                <table>
                    <tr><td>Number</td><td>RFID TID</td></tr>
                    <tr><td>
                        <select name="numberid">
                        <?php
                            if ($res = $db->query("SELECT numbers.id as id, numbers.number as number, races.name as racename, runners.name as runnername, runners.surname as runnersurname FROM numbers JOIN races ON (numbers.raceid = races.id) JOIN runners ON (runners.id = numbers.runnerid)")) {
                                while ($row = $res->fetch_assoc())
                                    printf("<option value='{$row['id']}'>{$row['racename']} [{$row['number']}] {$row['runnername']} {$row['runnersurname']}</option>");
                                $res->close();
                            }
                        ?>
                        </select>
                    </td>
                    <td><input type=text name=tid></td>
                    </tr>
                </table>
                <div class="text-right"><input type=submit class="button" name=create value=Create></div>
            </form>
        </div>
    </div>
</div>
