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
 * @global id
 * @global locai
 * @global tchar
 * @global g_list
 * @global trader
 * @global up
 * @global down
 * @global to
 * @global num
 */

// $to из банка
$item = findItemByFullId($to);

$count = intval(preg_replace('/.*' . $to . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["bank"]));
if ($count == 0) {
    msg("В банке нет " . $item[0], $tchar[0]);
}
if ($count > 1 && ! $num) {        // кол-во
    $stmp = "<p>Укажите количество:\n<br/><input name=\"num\" format=\"*N\" maxlength=\"7\" value=\"" . $count .
            "\"/>\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start&to=$to&num=$(num)\">Отправить</a>";
    msg($stmp, $tchar[0], 1, "", "num", $count);
}
if ( ! $num || $num < 1) {
    $num = 1;
}
if ($num > $count) {
    $num = $count;
}
additem($loc, $login, $login, $to, $num, "bank", "items", 0, -1, 1);
msg("Вы забрали из банка " . $num . " " . $item[0] .
    "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start\">Продолжить</a>");


