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
 */

use MaxDark\Amulet\OldCode\ViewOptions;

if ($speak == "n.t.bankir" && $char[14] != "t") {
    msg("Извини, услуги банка только для вступивших в Орден Тамплиера.");
}
if ($speak == "n.p.bankir" && $char[14] != "p") {
    msg("Ха, ты не не один из нас. Вначале стань настоящим пиратом, тогда и поговорим.");
}
// список
$items = explode("|", $loc_i[$loc][$login]["items"]);

if ( ! $items) {
    msg("У вас нет ни одного предмета", $tchar[0]);
}
$stmp = "<p>Выберите предмет:";
if ( ! $start) {
    $start = 0;
}
$keys = array_keys($items);
$listEnd = $start + ViewOptions::getInstance()->getMaxListSize();
for ($i = $start; $i < $listEnd && $i < count($keys); $i++) {
    if ($items[$keys[$i]]) {
        $k = explode(":", $items[$keys[$i]]);
        $item = findItemByFullId($k[0]);
        $item = $item[0];
        if (strpos($k[0], "..") !== false) {
            $item .= " *";
        }
        if ($k[1] > 1) {
            $item .= " (" . $k[1] . ")";
        }
        if (strpos($loc_i[$loc][$login]["equip"], $k[0]) !== false) {
            $item .= " [одето]";
        }
        $stmp .= "\n<br/><anchor>" . $item . "<go href=\"#menu\"><setvar name=\"to\" value=\"" . $k[0] .
                 "\"/></go></anchor>";
    }
}
if ($start) {
    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id\">^ </a>";
}
if ($listEnd < count($keys)) {
    if ( ! $start) {
        $stmp .= "\n<br/>";
    }
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=" . ($listEnd) . "\">+ (" .
             (count($keys) - $listEnd) . ")</a>";
}
$stmp .= "\n</p>\n</card>\n<card id=\"menu\" title=\"Меню\">\n<p>\n<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start&to=$(to)\">Положить в банк</a>\n<br/><a href=\"$PHP_SELF?sid=$sid&look=$(to)\">Инфо</a>";
msg($stmp, $tchar[0]);
