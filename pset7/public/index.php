<?php

    // configuration
    require("../includes/config.php");
    
    
    $userId = $_SESSION['id'];
    
    // query database for user portfolios
    $rows = CS50::query("SELECT * FROM portfolios WHERE user_id = ?", $userId);
    
    // find current cash for user
    $rowsCash = CS50::query("SELECT cash FROM users WHERE id = ?", $userId);
    $cash = "$" . number_format($rowsCash[0]["cash"], 2);
    
     // if we found something update our array
    $userProp = [];
    $tableHeaders = [];
    
    if (count($rows) < 1)
    {
        apologize("Nothing found :(. Start buy to see some results");
    }
    
    $id = 1;
    foreach ($rows as $row)
    {
        $stock = lookup($row["symbol"]);
        if ($stock !== false)
        {
            $userProp[] = [
                "id" => $id,
                "symbol" => $row["symbol"],
                "price" => $stock["price"],
                "name" => $stock["name"],
                "shares" => $row["shares"],
                "TOTAL" => "$" . number_format((float)$row["shares"] * (float)$stock["price"], 2)
            ];
        }
        $id++;
    }
    
    
    
    foreach ($userProp[0] as $key => $val)
    {
        array_push($tableHeaders, $key);
    }


    // render portfolio
    render("portfolio.php", ["userProp" => $userProp, "tableHeaders" => $tableHeaders, "cash" => $cash, "title" => "Portfolio"]);

?>
