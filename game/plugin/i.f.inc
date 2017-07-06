<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 * @global loc_water1
 * @global loc_water2
 */

// пустую бутылку заправляем водой, если есть рядом
if ($use == "i.f.b.empty") {
    include __DIR__ . DIRECTORY_SEPARATOR . 'loc_water.inc';
    if (strpos($loc_water1, $loc) !== false || strpos($loc_water2, $loc) !== false) {
        additem($loc, $login, "", "i.f.b.empty", 1, "items", "items", 1, 0, 1);
        additem($loc, "", $login, "i.f.b.water", 1, "items", "items", 1, 0, 1);
        msg("Вы наполнили водой пустую бутылку.");
    } else {
        msg("Рядом нет источника воды, чтобы наполнить бутылку водой.");
    }
}

if (file_exists("items/" . $use)) {
    $item = explode("|", (file_get_contents("items/" . $use)));
} else {
    die("err: no items/" . $use);
}
$char[6]                     = time() + 4;
$loc_i[$loc][$login]["char"] = implode("|", $char);
if ($item[2]) {
    if ($char[1] + $item[2] > $char[2]) {
        $item[2] = $char[2] - $char[1];
    }
    $char[1] += $item[2];
    $loc_i[$loc][$login]["char"] = implode("|", $char);
    addjournal($loc, $login, 'Жизнь +' . $item[2]);
}
if ($item[3]) {
    if ($char[3] + $item[3] > $char[4]) {
        $item[3] = $char[4] - $char[3];
    }
    $char[3] += $item[3];
    $loc_i[$loc][$login]["char"] = implode("|", $char);
    addjournal($loc, $login, 'Мана +' . $item[3]);
}
// удаляем
additem($loc, $login, "", $use, 1);
// пустая бутылка
if (substr($use, 0, 11) == 'i.f.b.water') {
    additem($loc, "", $login, "i.f.b.empty", 1);
}