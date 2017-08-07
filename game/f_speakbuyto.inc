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
 * @global to
 */

use MaxDark\Amulet\OldCode\PageType;

$item = findItemByFullId($to);

if (strpos($loc_i[$loc][$speak]["bank"], $to . ":") === false) {
    msg("У меня нет " . $item[0]);
}
$count = intval(preg_replace('/.*' . $to . ':(\d+).*/', "\\1", $loc_i[$loc][$speak]["bank"]));
if ($count == 0) {
    msg($item[0] . " у меня нет, попробуйте заглянуть в другой раз.", $tchar[0]);
}
if ($count > 1 && ! $num) {        // кол-во
    $stmp = "<p>Укажите количество:\n<br/><input name=\"num\" format=\"*N\" maxlength=\"7\" value=\"" . $count .
            "\"/>\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&to=$to&start=$start&num=$(num)\">Отправить</a>";
    msg($stmp, $tchar[0], 1, PageType::MSG, "num", $count);
}
if ( ! $num || $num < 1) {
    $num = 1;
}
if ($num > $count) {
    $num = $count;
}
// стоимость
if ($item[1] <= 1) {
    $trader[0] = 1;
}
$price = round($item[1] * $num * $trader[0]);
$money = intval(preg_replace('/.*i\.money:(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
if ($money < $price) {
    msg("У вас недостаточно денег (надо " . $price . " монет)");
}
manageItems($loc, $login, "", "i.money", $price, "items", "items", 0, -1, 1);
manageItems($loc, $speak, $login, $to, $num, "bank", "items", 0, -1, 1);
msg("Вы купили " . $num . " " . $item[0] . " за " . $price .
    " монет<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start\">Продолжить</a>");