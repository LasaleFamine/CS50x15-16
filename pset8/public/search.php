<?php

    require(__DIR__ . "/../includes/config.php");

    // numerically indexed array of places
    $places = [];
    
    if (!isset($_GET['geo']))
    {
        header("Location: /");
    }

    // TODO: search database for places matching $_GET["geo"], store in $places
    $rows = CS50::query("SELECT * FROM places WHERE MATCH(postal_code, place_name, admin_name1) AGAINST (?) limit 50", $_GET["geo"]);
    if ($rows < 1)
    {
        $places = "Nothing";
    }
    else 
    {
        foreach($rows as $row)
        {
            $places [] = $row;
        }
    }
    
    

    // output places as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($places, JSON_PRETTY_PRINT));

?>