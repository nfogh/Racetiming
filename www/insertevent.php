<h1>Add event</h1>
<form action="addevent.php" method=post>
    <div class="grid-container">
        <div class="grid-x grid-padding-x">
          <div class="medium-12 cell">
            <table>
                <tr><td>Number</td><td>Timestamp</td><td>Milliseconds</td><td>Event type</td></tr>
                <tr>
                    <td>
                        <select name="numberid">
                            <?php
                                if ($res = $db->query("SELECT numbers.id as numberid, numbers.number as number, runners.name as runnername, runners.surname as runnersurname, races.name as racename FROM numbers JOIN runners ON (numbers.runnerid = runners.id) JOIN races ON (numbers.raceid = races.id)")) {
                                    while ($row = $res->fetch_assoc())
                                        printf("<option value='{$row['numberid']}'>{$row['racename']} - {$row['number']} - {$row['runnername']} {$row['runnersurname']}</option>");
                                    $res->close();
                                }
                            ?>
                        </select>
                    </td>
                    <td><input type="datetime" name="timestamp" id="timestamp"></td>
                      <script>
                        $(function(){
                            $('#timestamp').fdatepicker({
            		            format: 'yyyy-mm-dd hh:ii:ss',
            		            disableDblClickSelection: true,
            		            pickTime: true}); });
                      </script>
                    <td><input type=number name=msecs value=0></td>
                    <td>
		        <select name=event>
			  <option value=1>Lap/Start</option>
			  <option value=2>Finish</option>
			</select>
                    </td>
            </tr>
        </table>
        <input type=submit class="button" name=create value=Create>
        </div>
        </div>
    </div>
</form>
