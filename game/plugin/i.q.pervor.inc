<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global to
 * @global char
 */
// выводит список предметов цели

if ($to) {
    if ( ! isset($loc_i[$loc][$to])) {
        msg("Нет цели");
    }
    if ($to == $login || (substr($to, 0, 2) != "n." && substr($to, 0, 2) != "u.")) {
        msg("Подглядывать можно только в рюкзаки NPC и игроков");
    }
    $tchar = explode("|", $loc_i[$loc][$to]["char"]);

    if ( ! $loc_i[$loc][$to]["items"]) {
        msg("<p>У " . $tchar[0] . " нет ни одного предмета.");
    }
    $stmp  = "<p>Предметы:";
    $items = explode("|", $loc_i[$loc][$to]["items"]);
    foreach (array_keys($items) as $i) {
        if ($items[$i]) {
            $k = explode(":", $items[$i]);
            $title = findItemByFullId($k[0]);
            $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&use=skill.steal&to=" . $to . "&id=" . $k[0] . "\">";
            if (strpos($k[0], "..") !== false) {
                $title[0] .= " *";
            }
            if ($k[1] > 1) {
                $stmp .= $title[0] . " (" . $k[1] . ")";
            } else {
                $stmp .= $title[0];
            }
            $stmp .= "</a>";
        }
    }
    manageItems($loc, $login, "", "i.q.pervor", 1, "items", "items", 1, 0, 1);
    msg($stmp, $char[0], 0);

} else {
    $list = 'all';
}    // выведем ниже список целей