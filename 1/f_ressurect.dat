<?php
global $loc_i;

if (isset($loc_i[$loc][$to]) && substr($to, 0, 2) == "u.") {
    $char = explode("|", $loc_i[$loc][$to]["char"]);
    if ($char[8]) {
        $char[8] = 0;
        $char[5] = time();
        $loc_i[$loc][$to]["char"] = implode("|", $char);
        addjournal($loc, "all", $char[0] . " воскрес!", $to);
        addjournal($loc, $to, "Вы воскресли!");
    } else addjournal($loc, $to, "Вы не призрак");
} 
