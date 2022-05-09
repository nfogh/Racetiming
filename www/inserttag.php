<h1>Attach RFID tag</h1>
<div class="grid-container">
    <div class="grid-x grid-padding-x">
        <div class="medium-6 cell">
            <form action="addtag.php" method=post>
                <table>
                    <tr><td>Runner</td><td>RFID TID</td></tr>
                    <tr><td>
                        <select name="runnerid">
                        <?php
                            if ($res = $db->query("SELECT * FROM runners")) {
                                while ($row = $res->fetch_row())
                                    printf("<option value=\"%s\">%s</option>", $row[0], $row[1]);
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
