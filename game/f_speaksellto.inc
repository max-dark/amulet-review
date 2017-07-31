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
 * @global num
 * @global to
 */

// продаем $to
$item = findItemByFullId($to);

// фильтр
if ($trader[1]) {
    $filter = explode(":", $trader[1]);
} else {
    $filter = "";
}
// удалим что не в фильтре
if ($filter) {
    $b = 0;
    foreach ($filter as $j) {
        if (strpos($to, $j) !== false) {
            $b = 1;
            break;
        }
    }
    if ( ! $b) {
        msg("Извини, меня не интересует " . $item[0], $tchar[0]);
    }
}

$count = intval(preg_replace('/.*' . $to . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
if ($count == 0) {
    msg("У вас нет " . $item[0] . ", возвращайтесь когда будет.", $tchar[0]);
}
if ($count > 1 && ! $num) {        // кол-во
    $stmp = "<p>Укажите количество:\n<br/><input name=\"num\" format=\"*N\" maxlength=\"7\" value=\"" . $count .
            "\"/>\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&to=$to&start=$start&num=$(num)\">Отправить</a>";
    msg($stmp, $tchar[0], 1, "", "num", $count);
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
additem($loc, "", $login, "i.money", $price, "items", "items", 0, -1, 1);
additem($loc, $login, "", $to, $num, "items", "items", 0, -1, 1);
msg("Вы продали " . $num . " " . $item[0] . " за " . $price .
    " монет<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start\">Продолжить</a>");

