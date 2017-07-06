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
 */

// перемещение в банке
$items = explode("|", $loc_i[$loc][$login]["bank"]);
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
        $tc            = $items[$i - 1];
        $items[$i - 1] = $items[$i];
        $items[$i]     = $tc;
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
        $tc            = $items[$i + 1];
        $items[$i + 1] = $items[$i];
        $items[$i]     = $tc;
    }

    $loc_i[$loc][$login]["bank"] = implode("|", $items);
    msg("Предмет передвинут вниз.");
}