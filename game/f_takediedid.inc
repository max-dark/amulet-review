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
 * @global num
 * @global id
 */

use MaxDark\Amulet\OldCode\PageType;

$died = explode("|", $loc_i[$loc][$take]);
$tcount = intval(preg_replace('/.*' . $id . ':(\d+).*/', "\\1", $died[3]));
if ($tcount) {
    // проверим кол-во
    if ($all) {
        $num = $tcount;
    }
    if ($tcount > 1 && !$num) {
        $stmp = "<p>Укажите количество:\n<br/><form action=\"$PHP_SELF\" method=\"get\">" .
            "<input name=\"sid\" value=\"" . $sid . "\" type=\"hidden\"/>" .
            "<input name=\"take\" value=\"" . $take . "\" type=\"hidden\"/>" .
            "<input name=\"id\" value=\"" . $id . "\" type=\"hidden\"/>" .
            "<input class=\"form-control\" name=\"num\" format=\"*N\" maxlength=\"7\" value=\"" . $item[1] .
            "\"/>\n<br/><button class=\"btn btn-outline-primary\" type=\"submit\">Отправить</button></form></p>";
        msg($stmp, "Взять", 1, PageType::MSG, "num", $tcount);
    }
    if (!$num || $num < 1) {
        $num = 1;
    }
    if ($num > $tcount) {
        $num = $tcount;
    }
    manageItems($loc, $take, $login, $id, $num, "items", "items", 1, -1, 1);
    $char = explode("|", $loc_i[$loc][$login]["char"]);
    $char[6] = time() + 2;
    $loc_i[$loc][$login]["char"] = implode("|", $char);
    //$id=0;	// чтобы вывести список оставшихся
} else {
    addjournal($loc, $login, "Нет такого предмета");
}