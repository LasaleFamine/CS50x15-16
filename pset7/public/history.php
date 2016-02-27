<?php

    // configuration
    require("../includes/config.php");
    
    
    $userId = $_SESSION['id'];
    
    // query database for user portfolios
    $rows = CS50::query("SELECT * FROM history WHERE user_id = ?", $userId);
    
    $id = 1;
    foreach ($rows as $row)
    {
        
        $history [] = [
            "id" => $id,
            "transaction" => $row["transaction"],
            "symbol" => $row["symbol"],
            "shares" => $row["shares"],
            "price" => "$" . number_format($row["share_price"], 2),
            "Date/Time" => date("d/m/Y H:i", strtotime($row["timestamp"]))
        ];
        
        $id++;
       
    }
    
    // render portfolio
    render("history.php", ["history" => $history, "title" => "History"]);
    
?>