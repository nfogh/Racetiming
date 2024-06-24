<h1>Insert race</h1>
<form action="addrace.php" method=post>
<div class="grid-container">
    <div class="grid-x grid-padding-x">
      <div class="medium-12 cell">
        <label>Name
          <input type="text" name="name" placeholder="Race name">
        </label>
      </div>
      <div class="medium-12 cell">
        <label>Desciption
          <textarea name="description" placeholder="Race description"></textarea>
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Start date/time
          <input type="datetime" name="start" id="start">
          <script>
            $(function(){
                $('#start').fdatepicker({
		            format: 'yyyy-mm-dd hh:ii:ss',
		            disableDblClickSelection: true,
		            pickTime: true}); });
          </script>
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Finish date/time
        <input type="datetime" name="finish" id="finish">
          <script>
            $(function(){
                $('#finish').fdatepicker({
		            format: 'yyyy-mm-dd hh:ii:ss',
		            disableDblClickSelection: true,
		            pickTime: true}); });
          </script>
        </label>
      </div>
      <div class="medium-12 cell">
        <label>Address
          <textarea name="address" placeholder="Enter address"></textarea>
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Latitude
        <input type="text" name="latitude" placeholder="Enter latitude">
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Longitude
        <input type="text" name="longitude" placeholder="Enter longitude">
        </label>
      </div>
      <div class="medium-6 cell">
        <label>Length
          <input type="text" name="lap_length" value="<?=$lap_length ?>">
        </label>
      </div>
      <div class="medium-12 cell">
        <div class="text-right">
          <input type="submit" class="button" name="create" value="Create new race">
        </div>
      </div>
    </div>
  </div>
</form>
