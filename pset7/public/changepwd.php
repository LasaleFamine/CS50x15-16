<?php

     // configuration
    require("../includes/config.php");
    
    $userId = $_SESSION["id"];

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("changepwd_form.php", ["title" => "Change password"]);
    }
    
     // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if(empty($_POST["current"]) || empty($_POST["password"]) || empty($_POST["confirmation"]))
        {
            apologize("You must fill all the fields");
        }
        
        // find current password for user
        $rows = CS50::query("SELECT hash FROM users WHERE id = ?", $userId);
        
        // compare hash of user's input against hash that's in database
        if (!password_verify($_POST["current"], $rows[0]["hash"]))
        {
           apologize("Wrong current password");
        }
        
        if($_POST["password"] != $_POST["confirmation"])
        {
            apologize("Passowrd and confirmation must be identical");
        }
        
        // Update password
        $update = CS50::query("UPDATE users SET hash = ? WHERE id = ?",  password_hash($_POST["password"], PASSWORD_DEFAULT), $userId);
        if (!$update)
        {
            apologize("Error, retry");
        }
        
        redirect("/");
        
    }
?>