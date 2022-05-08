<?php
// Initialize the session.
// If you are using session_name("something"), don't forget it now!
session_start();

// Unset all of the session variables.
$_SESSION = array();

// If it's desired to kill the session, also delete the session cookie.
// Note: This will destroy the session, and not just the session data!
if (ini_get("session.use_cookies")) {
    $params = session_get_cookie_params();
    setcookie(session_name(), '', time() - 42000,
        $params["path"], $params["domain"],
        $params["secure"], $params["httponly"]
    );
}

// Finally, destroy the session.
session_destroy();

$title = 'Logout';
$masthead_image = 'assets/images/masthead.jpg';
$masthead_text = "Logged out";

require '_init.php';
require '_header.php';
?>

<div class="callout large success">You are now logged out</div>

<?php
require '_footer.php';
?>
