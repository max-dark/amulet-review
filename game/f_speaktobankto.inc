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
 * @global id
 * @global to
 * @global num
 */
// $to в банк
use MaxDark\Amulet\OldCode\PageType;

$item = findItemByFullId($to);
$count = intval(preg_replace('/.*' . $to . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
if ($count == 0) {
    msg("У вас нет " . $item[0], $tchar[0]);
}
if ($count > 1 && ! $num) {        // кол-во
    $stmp = "<p>Укажите количество:\n<br/><input name=\"num\" format=\"*N\" maxlength=\"7\" value=\"" . $count .
            "\"/>\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start&to=$to&num=$(num)\">Отправить</a>";
    msg($stmp, $tchar[0], 1, PageType::MSG, "num", $count);
}
if ( ! $num || $num < 1) {
    $num = 1;
}
if ($num > $count) {
    $num = $count;
}
if ($to == "i.money" &&
    intval(preg_replace('/.*i.money:(\d+).*/', "\\1", $loc_i[$loc][$login]["bank"])) + $num > 70000
) {
    msg("Извините, в банке можно хранить не более 70000 монет");
}
if ($speak == "n.t.bankir") {
    manageItems($loc, $login, "", "i.money", "20", "items", "items", 1, 0, 1);
}
if ($speak == "n.p.bankir") {
    manageItems($loc, $login, "", "i.money", "200", "items", "items", 1, 0, 1);
}
manageItems($loc, $login, $login, $to, $num, "items", "bank", 0, -1, 1);
msg("Вы положили в банк " . $num . " " . $item[0] .
    "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start\">Продолжить</a>");
