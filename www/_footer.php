<script>
    <?php
    if (isset($masthead_image))
        print('$("#masthead").backstretch("' . $masthead_image . '", {fade: 700})');
    ?>

    $(document).foundation();
</script>

</body>

</html>
