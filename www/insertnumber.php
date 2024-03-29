<h1>Attach number</h1>
<form action="addnumber.php" method=post>
    <div class="grid-container">
        <div class="grid-x grid-padding-x">
          <div class="medium-12 cell">
            <table>
                <tr><td>Race</td><td>Runner</td><td>Number</td><td>Number of laps</td><td>Expected time [min]</td></tr>
                <tr>
                    <td>
                        <select name="raceid">
                            <?php
                                if ($res = $db->query("SELECT id, name FROM races")) {
                                    while ($row = $res->fetch_assoc())
                                        printf("<option value='{$row['id']}'>{$row['name']}</option>");
                                    $res->close();
                                }
                            ?>
                        </select>
                    </td>
                    <td>
                        <select name="runnerid">
                            <?php
                                if ($res = $db->query("SELECT id, name, surname FROM runners")) {
                                    while ($row = $res->fetch_assoc())
                                        printf("<option value={$row['id']}>{$row['name']} {$row['surname']}</option>");
                                    $res->close();
                                }
                            ?>
                        </select>
                    </td>
                    <td><input type=number name=number value=1></td>
                    <td><input type=number name=laps value=1></td>
                    <td><input type=number name=time value=20></td>
            </tr>
        </table>
        <input type=submit class="button" name=create value=Create>
        </div>
        </div>
    </div>
</form>
