<h1>Attach number</h1>
<form action="addnumber.php" method=post>

    <table>
        <tr><td>Race</td><td>Runner</td><td>Number</td></tr>
        <tr>
            <td>
                <select name="raceid">
<?php
    if ($res = $db->query("SELECT id, name FROM races")) {
        while ($row = $res->fetch_row())
            printf("<option value=\"%s\">[%s] %s</option>", $row[0], $row[0], $row[1]);
        $res->close();
    }
?>
                </select>
            </td>
            <td>
                <select name="runnerid">
<?php
    if ($res = $db->query("SELECT id, name FROM runners")) {
        while ($row = $res->fetch_row())
            printf("<option value=\"%s\">[%s] %s</option>", $row[0], $row[0], $row[1]);
        $res->close();
    }
?>
                </select>
            </td>
            <td><input type=text name=number></td>
    </tr>
</table>
<input type=submit class="button" name=create value=Create>
</form>
