<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 */
// действует на цель как оружие

if ($to) {    // атакуем как магией
    if ( ! isset($loc_i[$loc][$to])) {
        msg("Нет цели");
    }
    $war     = explode("|", $loc_i[$loc][$login]["war"]);
    $war[0]  = 100;
    $war[1]  = 1;
    $war[2]  = 28;
    $war[3]  = 8;
    $war[4]  = 0;
    $war[12] = "ядом";
    $war[14] = "";
    $war     = implode("|", $war);

    $char[6]                     = time() + 8;
    $loc_i[$loc][$login]["char"] = implode("|", $char);

    manageItems($loc, $login, "", "i.b.jad", 1, "items", "items", 1, 0, 1);
    attack($loc, $login, $to, $war, 0, 1);
} else {
    $list = 'all';
}    // выведем ниже список целей
