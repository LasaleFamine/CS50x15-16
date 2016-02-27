<?php

    // configuration
    require("../includes/config.php");
    
    $userId = $_SESSION["id"];
    
    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        
        // find current cash for user
        $rows = CS50::query("SELECT symbol, shares FROM portfolios WHERE user_id = ?", $userId);
        if (count($rows) < 1)
        {
            apologize("You don't have shares to sell");
        }
        
        // else render form
        render("sell_form.php", ["shares" => $rows, "title" => "Sell your shares"]);
    }
    
     // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (!empty($_POST["symbol"]) && !empty($_POST["sharesN"]))
        {
            
            // find current shares for user
            $rows = CS50::query("SELECT shares FROM portfolios WHERE user_id = ? AND symbol = ?", $userId, $_POST["symbol"]);
            if (count($rows) < 1)
            {
                apologize("Error, retry");
            }
            
            if(!preg_match("/^\d+$/", $_POST["sharesN"]))
            {
                apologize("Error, shares must be integer, no negative");
            }
            
            $stock = lookup($_POST["symbol"]);
            if (!$stock)
            {
                apologize("Error, retry");
            }
            $stock["price"] = number_format($stock["price"], 2);
            
            if($rows[0]["shares"] < $_POST["sharesN"])
            {
                apologize("Error, you are trying to sell to much shares");
            } 
            else if($rows[0]["shares"] == $_POST["sharesN"])
            {
                $deleteShare = CS50::query("DELETE FROM portfolios WHERE user_id = ? AND symbol = ?", $userId, $_POST["symbol"]);
            }
            else 
            {
                $updateShare = CS50::query("UPDATE portfolios SET shares = shares - ? WHERE user_id = ? AND symbol = ?", $_POST["sharesN"], $userId,  $_POST["symbol"]);
            }
            
            $cashEarn = $stock["price"] * $_POST["sharesN"];
            $updateCash = CS50::query("UPDATE users SET cash = cash + ? WHERE id = ?", $cashEarn, $userId);
            
            $transactionDetails[] = [
                "userId" => $userId,
                "transaction" => "SELL",
                "symbol" => $_POST["symbol"],
                "shares" => $_POST["sharesN"],
                "share_price" => $stock["price"]
            ];
            
            $transaction = updateHistory($transactionDetails);
            if($transaction === false)
            {
                apologize("Error on update history");
            }
            
            redirect("/");
            
        }
        
        apologize("You must submit a symbol and number of shares to sell");
        
    }
    
?>