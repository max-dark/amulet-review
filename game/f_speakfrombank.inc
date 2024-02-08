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
 * @global trader
 * @global up
 * @global down
 */

use MaxDark\Amulet\OldCode\ViewOptions;

if ($speak == "n.t.bankir" && $char[14] != "t") {
    msg("Извини, услуги банка только для вступивших в Орден Тамплиера.");
}
if ($speak == "n.p.bankir" && $char[14] != "p") {
    msg("Ха, ты не не один из нас. Вначале стань настоящим пиратом, тогда и поговорим.");
}
$items = explode("|", $loc_i[$loc][$login]["bank"]);
if (empty($items)) {
    msg("У вас в банке нет ни одного предмета", $tchar[0]);
}

#start: перемещение предметов
if ($up) {
    if (strpos($loc_i[$loc][$login]["bank"], $up . ":") === false) {
        msg("Нет такого предмета");
    }
    for ($i = 0; $i < count($items); $i++) {
        if (strpos($items[$i], $up . ":") !== false) {
            break;
        }
    }
    if ($i == 0) {
        array_push($items, $items[$i]);
        array_shift($items);
    } else {
        $tc = $items[$i - 1];
        $items[$i - 1] = $items[$i];
        $items[$i] = $tc;
    }
    $loc_i[$loc][$login]["bank"] = implode("|", $items);
    msg("Предмет передвинут вверх.");
}
if ($down) {
    $up = $down;
    if (strpos($loc_i[$loc][$login]["bank"], $up . ":") === false) {
        msg("Нет такого предмета");
    }
    for ($i = 0; $i < count($items); $i++) {
        if (strpos($items[$i], $up . ":") !== false) {
            break;
        }
    }
    if ($i == count($items) - 1) {
        array_unshift($items, $items[$i]);
        array_pop($items);
    } else {
        $tc = $items[$i + 1];
        $items[$i + 1] = $items[$i];
        $items[$i] = $tc;
    }

    $loc_i[$loc][$login]["bank"] = implode("|", $items);
    msg("Предмет передвинут вниз.");
}
#end: перемещение предметов

#start список
$stmp = "";
if (!$start) {
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
        $stmp .= "\n" . '<br/><a tabindex="0" class="btn btn-secondary m-1" role="button" data-trigger="focus" data-toggle="popover" data-content="template" title="' . $item . '" label="' . $k[0] . '">' . $item . '</a>';
    }
}
#end: список
if ($stmp) {
    $stmp = "<p>Выберите предмет:" . $stmp;
} else {
    msg("У вас в банке нет ни одного предмета", $tchar[0]);
}
if ($start) {
    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id\">^ </a>";
}
if ($listEnd < count($keys)) {
    if (!$start) {
        $stmp .= "\n<br/>";
    }
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=" . ($listEnd) . "\">+ (" .
        (count($keys) - $listEnd) . ")</a>";
}
$stmp .= "\n</p>\n</div>\n<div id=\"menu\" class=\"d-none\" title=\"Меню\">\n";
$stmp .= "<p><a class=\"btn btn-outline-primary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start&to=$(to)\">Забрать из банка</a>\n";
$stmp .= "<br/><a class=\"btn btn-outline-secondary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&look=$(to)\">Инфо</a>\n";
$stmp .= "<br/>[<a class=\"btn btn-secondary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&up=$(to)\">Вверх</a> | <a class=\"btn btn-secondary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&down=$(to)\">Вниз</a>]";
msg($stmp, $tchar[0]);
