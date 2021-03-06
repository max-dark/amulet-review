<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */

if (substr($to, 0, 5) == 'i.rr.' &&
    (strpos($loc_i[$loc][$login]["items"], $to . ":") !== false || isset($loc_i[$loc][$to]))
) {
    if (substr($to, 0, 10) != 'i.rr.empty') {
        $tl = substr($to, 5);
        if ($tl == "arena") {
            msg("В арену нельзя телепортироваться, переназначьте руну в другое место");
        }
        $tc = explode("x", $tl);
        if ($tc[2] >= 1099) {
            msg("На Волчьем острове магия рун перемещения не работает из-за влияния магических самоцветов в горной породе");
        }
        if ($tl == "x1087x543" || $tl == "x1080x539" || $tl == "x1087x528" || $tl == "x1080x532") {
            msg("В таверну нельзя телепортироваться, переназначьте руну в другое место");
        }

        $loc_i[$loc]["i.s.portal_" . $tl] = "портал телепортации|0|" . (time() + 3 * 60);    // 3 минуты
        addjournal($loc, "all", $char[0] . " создал портал телепортации");
    } else {
        addjournal($loc, $login, "Руна не помечена ни в одно место");
    }
} else {
    addjournal($loc, $login, "Заклинание можно использовать только на руну телепортации");
}
