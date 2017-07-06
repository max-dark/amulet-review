<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global tchar
 * @global use
 * @global id
 */
if ($use) {
    if (time() > $char[6]) {
        // если $list='all', то выведется список целей
        if ($char[8]) {
            msg("<p>Вы призрак и поэтому не можете ничего использовать, найдите лекаря или камень воскрешения");
        }
        if (substr($use, 0, 6) == 'i.note' && substr($use, 0, 6) == 'i.book') {
            $look = $use;
        } // на книги смотрим
        else {
            $scroll = 0;    // со свитка?
            if (substr($use, 0, 2) == "i.") {
                include "f_useitem.inc";
            }
            if (substr($use, 0, 2) == "m.") {
                include "f_usemagic.inc";
            }
            if (substr($use, 0, 6) == "skill.") {
                include "f_useskill.inc";
            }
        }
    } else {
        addjournal($loc, $login, "Вы должны отдохнуть " . round($char[6] - time() + 1) . " сек");
    }
}

