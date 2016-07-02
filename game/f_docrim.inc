<?php

/**Превращает персонажа в преступника
 *
 * @param string $loc   локация
 * @param string $login логин
 * @param string $title "звание"
 */
function docrim($loc, $login, $title = "преступник") {

    global $game, $loc_i, $g_crim;
    if (substr($loc, 0, 2) == "c." && substr($loc, 3) != ".in") {
        return;
    }
    if (isset($loc_i[$loc][$login]) && substr($login, 0, 2) == "u." && $loc != "arena") {
        $char = explode("|", $loc_i[$loc][$login]["char"]);
        $char[9] = $title;
        $char[10] = time() + $g_crim;
        $loc_i[$loc][$login]["char"] = implode("|", $char);
    }
}