<?php

    // configuration
    require("../includes/config.php");
    
    $userId = $_SESSION["id"];
    
    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // find current cash for user
        $rows = CS50::query("SELECT cash FROM users WHERE id = ?", $userId);
        if (count($rows) < 1)
        {
            apologize("Error, retry");
        }
        
        $cash = number_format($rows[0]["cash"], 2);
        
        // else render form
        render("buy_form.php", ["cash" => $cash, "title" => "Buy shares"]);
    }
    
     // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (!empty($_POST["symbol"]) && !empty($_POST["shares"]))
        {
            if(!preg_match("/^\d+$/", $_POST["shares"]))
            {
                apologize("Error, shares must be integer, no negative");
            }
            
            $stock = lookup($_POST["symbol"]);
            if (!$stock)
            {
                apologize("Error, symbol not found");
            }
            $stock["price"] = number_format($stock["price"], 2);
            
             // find current cash for user
            $rows = CS50::query("SELECT cash FROM users WHERE id = ?", $userId);
            if (count($rows) < 1)
            {
                apologize("Error, retry");
            }
            
            $cashLoose = $_POST["shares"] * $stock["price"];
            
            if ($rows[0]["cash"] < $cashLoose)
            {
                apologize("You don't have too much money to buy {$_POST['shares']} shares of {$stock['name']}");
            }
            
            $rowsIns = CS50::query("INSERT INTO portfolios (user_id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)", $userId, strtoupper($stock["symbol"]), $_POST["shares"]);
            if ($rowsIns != 0)
            {
                $updateCash = CS50::query("UPDATE users SET cash = cash - ? WHERE id = ?", $cashLoose, $userId);
                
                $transactionDetails[] = [
                    "userId" => $userId,
                    "transaction" => "BUY",
                    "symbol" => $stock["symbol"],
                    "shares" => $_POST["shares"],
                    "share_price" => $stock["price"]
                ];
            
                $transaction = updateHistory($transactionDetails);
                if($transaction === false)
                {
                    apologize("Error on update history");
                }
                
                redirect("/");
            }
            
            apologize("Error, retry");
        }
        
        apologize("You must fill both the fields");
        
        
    }