<?php
    $title = 'Login';
    $masthead_image = 'assets/images/masthead.jpg';
    $masthead_text = "Login";
    require '_init.php';

    // Initialize the session
 
// Check if the user is already logged in, if yes then redirect him to welcome page
if(isset($_SESSION["loggedin"]) && $_SESSION["loggedin"] === true){
    header("location: admin.php");
    exit;
}

// Define variables and initialize with empty values
$username = $password = "";
$username_err = $password_err = $login_err = "";
 
// Processing form data when form is submitted
if($_SERVER["REQUEST_METHOD"] == "POST"){
    // Check if username is empty
    if (empty(trim($_POST["username"]))) {
        $username_err = "Please enter username.";
    } else{
        $username = sqlite_escape_string(trim($_POST["username"]));
    }
    
    // Check if password is empty
    if(empty(trim($_POST["password"]))){
        $password_err = "Please enter your password.";
    } else{
        $password = sqlite_escape_string(trim($_POST["password"]));
    }
    
    // Validate credentials
    if(empty($username_err) && empty($password_err)) {
        if (!empty($config['admin']['name']) && !empty($config['admin']['password']) &&
            ($config['admin']['name'] == $username) && password_verify($password, $config['admin']['password'])) {
                     $_SESSION["loggedin"] = true;
                     $_SESSION["id"] = -1;
                     $_SESSION["username"] = $username;                            
                     
                     header("location: admin.php");
        }

        $sql = 'SELECT name, password, id FROM admins WHERE name = "' . $username . '"';
        if ($res = $sqlite->query($sql)) {
            if ($row = $res->fetchArray(SQLITE_ASSOC)) {
                if (password_verify($password, $row['password'])) {
                     $_SESSION["loggedin"] = true;
                     $_SESSION["id"] = $id;
                     $_SESSION["username"] = $username;                            
                     
                     header("location: admin.php");
                } else {
                     $login_err = "Invalid username or password.";
                }
            } else {
                $login_err = $sqlite->lastErrorMsg();
            }
        } else {
            $login_err = $sqlite->lastErrorStr();
        }
    }
}
?>
 
<?php require '_header.php'; ?>

    <div class="xgrid">
        <h2>Login</h2>
        <p>Please fill in your credentials to login.</p>

        <?php 
        if(!empty($login_err)){
            echo '<div class="callout large alert">' . $login_err . '</div>';
        }        
        ?>

        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
            <div class="form-group">
                <label>Username</label>
                <input type="text" name="username" class="form-control <?php echo (!empty($username_err)) ? 'is-invalid' : ''; ?>" value="<?php echo $username; ?>">
                <span class="invalid-feedback"><?php echo $username_err; ?></span>
            </div>    
            <div class="form-group">
                <label>Password</label>
                <input type="password" name="password" class="form-control <?php echo (!empty($password_err)) ? 'is-invalid' : ''; ?>">
                <span class="invalid-feedback"><?php echo $password_err; ?></span>
            </div>
            <div class="form-group">
                <input type="submit" class="button" value="Login">
            </div>
        </form>
    </div>
<?php require '_footer.php' ?>
