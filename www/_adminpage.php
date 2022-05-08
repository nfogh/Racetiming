<?php 
    if (!$_SESSION["loggedin"]) {
        $title = 'Login';
        $masthead_image = 'assets/images/masthead.jpg';
        $masthead_text = "You are not logged in";
        require '_init.php';
        require '_header.php';

        printf('<div class="callout large warning">You are not logged in. Please go to the <a href="/login.php">login page</a></div>');
        require '_footer.php';
        exit();
    }
?>
