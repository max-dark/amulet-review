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
 * @global g_list
 * @global id
 */
// если не указан предмет, то список
$died = explode("|", $loc_i[$loc][$take]);
if ($died[3]) {
    $items = explode(",", $died[3]);
    if (count($items) > 0) {
        if (strpos($died[0], "*") === false) {
            $clan = "";
        } else {
            $clan = substr($died[0], strpos($died[0], "*") + 1, strrpos($died[0], "*") - strpos($died[0], "*") - 1);
        }
        $tc = explode("|", $loc_i[$loc][$login]["char"]);
        if (strpos($tc[0], "*") === false) {
            $clan1 = "";
        } else {
            $clan1 = substr($tc[0], strpos($tc[0], "*") + 1, strrpos($tc[0], "*") - strpos($tc[0], "*") - 1);
        }
        if ($char[13]) {
            $wife = substr($take, 6, strrpos($take, ".") - 6) ==
                    substr($char[13], 0, strlen(substr($take, 6, strrpos($take, ".") - 6)));
        } else {
            $wife = 0;
        }
        if ( ! $died[1] && substr($take, 0, strlen("i.s.d." . $login)) != "i.s.d." . $login &&
             ( ! $clan1 || ($clan1 && $clan1 != $clan)) && ! $wife && substr($take, 0, 11) != "i.s.d.u.qv."
        ) {
            $stmp = "<p>Брать отсюда предметы будет преступлением!";
        } else {
            $stmp = "<p>Предметы:";
        }
        if ( ! $start) {
            $start = 0;
        }
        $keys = array_keys($items);
        for ($i = $start; $i < $start + $g_list && $i < count($keys); $i++) {
            if ($items[$keys[$i]]) {
                $k = explode(":", $items[$keys[$i]]);
                if (strpos($k[0], "_") !== false) {
                    $ti = substr($k[0], 0, strpos($k[0], "_"));
                } else {
                    $ti = $k[0];
                }
                if (substr($k[0], 0, 5) == "i.rr.") {
                    $item = explode("|", "руна|50");
                } else {
                    if (file_exists("items/" . $ti)) {
                        $item = explode("|", (file_get_contents("items/" . $ti)));
                    } else {
                        die("err: no items/" . $k[0]);
                    }
                }
                $item = $item[0];
                if (strpos($k[0], "..") !== false) {
                    $item .= " *";
                }
                if ($k[1] > 1) {
                    $item .= " (" . $k[1] . ")";
                }
                $stmp .= "\n<br/><anchor>" . $item . "<go href=\"#menu\"><setvar name=\"to\" value=\"" . $k[0] .
                         "\"/></go></anchor>";
            }
        }
        if ($start) {
            $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&take=$take\">^ </a>";
        }
        if ($start + $g_list < count($keys) - 1) {
            if ( ! $start) {
                $stmp .= "\n<br/>";
            }
            $stmp .= "<a href=\"$PHP_SELF?sid=$sid&take=$take&start=" . ($start + $g_list) . "\">+ (" .
                     (count($keys) - $start - $g_list) . ")</a>";
        }
        $stmp .= "\n</p>\n</card>\n<card id=\"menu\" title=\"Меню\">\n<p>\n<a href=\"$PHP_SELF?sid=$sid&take=$take&id=$(to)\">Взять</a>\n<br/><a href=\"$PHP_SELF?sid=$sid&take=$take&id=$(to)&all=1\">Взять все</a>\n<br/><a href=\"$PHP_SELF?sid=$sid&from=$take&look=$(to)\">Инфо</a>";
        msg($stmp, $died[0]);
    }
} else {
    addjournal($loc, $login, "У " . $died[0] . " нет ни одного предмета");
}