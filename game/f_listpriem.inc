<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global to
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global up
 * @global down
 */
// список магии

use MaxDark\Amulet\OldCode\ViewOptions;

if ( ! $loc_i[$loc][$login]["priem"]) {
    msg("<p>Вы не знаете ни одного приема, найдите NPC, который согласится обучить вас.");
}
$items = explode("|", $loc_i[$loc][$login]["priem"]);
if ($up) {
    if (strpos($loc_i[$loc][$login]["priem"], $up . ":") === false) {
        msg("Нет такого приема");
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
        $tc            = $items[$i - 1];
        $items[$i - 1] = $items[$i];
        $items[$i]     = $tc;
    }
    $loc_i[$loc][$login]["priem"] = implode("|", $items);
    msg("Прием передвинут вверх.");
}
if ($down) {
    $up = $down;
    if (strpos($loc_i[$loc][$login]["priem"], $up . ":") === false) {
        msg("Нет такого приема");
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
        $tc            = $items[$i + 1];
        $items[$i + 1] = $items[$i];
        $items[$i]     = $tc;
    }
    $loc_i[$loc][$login]["priem"] = implode("|", $items);
    msg("Прием передвинут вниз.");
}
$stmp = "<p>Ваши приемы:";
if ( ! $start) {
    $start = 0;
}
$keys = array_keys($items);
$listEnd = $start + ViewOptions::getInstance()->getMaxListSize();
for ($i = $start; $i < $listEnd && $i < count($keys); $i++) {
    $k = explode(":", $items[$keys[$i]]);
    $item = findItemByBaseId($k[0]);
    if (time() < $k[1]) {
        $item = $item[0] . " [осталось " . (round(($k[1] - time()) / 60) + 1) . " минут]";
    } else {
        $item = $item[0];
    }
    if ($to) {
        $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&to=$to&use=" . $k[0] . "\">" . $item . "</a>";
    } else {
        $stmp .= "\n<br/><anchor>" . $item . "<go href=\"#menu\"><setvar name=\"to\" value=\"" . $k[0] .
                 "\"/></go></anchor>";
    }
}
if ($start) {
    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&list=priem\">^ </a>";
}
if ($listEnd < count($keys)) {
    if ( ! $start) {
        $stmp .= "\n<br/>";
    }
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&to=$to&list=priem&start=" . ($listEnd) . "\">+ (" .
             (count($keys) - $listEnd) . ")</a>";
}
// карта меню
$stmp .= "\n</p>\n</card>\n<card id=\"menu\" title=\"Меню\">\n<p><a href=\"$PHP_SELF?sid=$sid&look=$(to)\">Инфо</a>\n<br/>[<a href=\"$PHP_SELF?sid=$sid&list=priem&up=$(to)\">Вверх</a> | <a href=\"$PHP_SELF?sid=$sid&list=priem&down=$(to)\">Вниз</a>]\n";
msg($stmp, "Приемы", 1);