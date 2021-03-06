<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global $use
 */
// инкрустация если есть i.set.shlif

if ($to) {
    if (strpos($to, "_") === false) {
        msg("Инкрустировать самоцветы можно только в именные предметы, созданные игроками");
    }
    if (strpos($loc_i[$loc][$login]["items"], "i.set.shlif:") === false) {
        msg("В вашем рюкзаке должен быть набор ювелира");
    }
    $item = findItemByFullId($to);
    $title = $item[0];
    if (intval(preg_replace('/.*' . $to . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["items"])) < 1) {
        msg("У вас нет " . $title);
    }
    if (strpos($to, ".." . substr($use, 4)) !== false) {
        msg("Этот самоцвет уже инкрустирован в " . $title);
    }
    $xF = preg_match_all("/\.\.(\w+)/", $to, $regF);
    if ($xF > 2) {
        msg("В один предмет можно инкрустировать не более 3 самоцветов");
    }

    $skills = explode("|", $loc_i[$loc][$login]["skills"]);
    if ($skills[28] > 0) {
        $char[6]                     = time() + 10;
        $loc_i[$loc][$login]["char"] = implode("|", $char);
        if (rand(0, 100) < $skills[28] * 10 + 20) {
            manageItems($loc, $login, "", $use, 1);
            manageItems($loc, $login, "", $to, 1);
            if (rand(0, 100) >= (6 - $skills[28]) * 10 + $xF * 10) {
                $to .= ".." . substr($use, 4);
                addjournal($loc, $login, "Вы инкрустировали самоцвет в " . $title);
                addjournal($loc, "all", $char[0] . " инкрустировал самоцвет в " . $title, $login);
                manageItems($loc, "", $login, $to, 1);
                addexp($loc, $login, rand(0, 10));
            } else {
                addjournal($loc, $login, "Вы испортили " . $title);
                addjournal($loc, "all", $char[0] . " испортил " . $title, $login);
            }
            // шлифовалка может сломаться всегда 5%
            if (rand(1, 100) <= 5) {
                addjournal($loc, $login, "Вы сломали набор ювелира");
                addjournal($loc, "all", $char[0] . " сломал набор ювелира", $login);
                manageItems($loc, $login, "", "i.set.shlif", 1);
            }
        } else {
            addjournal($loc, $login, "Вам не удалось инкрустировать самоцвет, попробуйте еще раз");
            addjournal($loc, "all", $char[0] . " пытался инкрустировать самоцвет", $login);
        }
    } else {
        addjournal($loc, $login, "Ваш навык ювелира равен 0, вы не умеете инкрустировать самоцветы");
    }
} else {
    $list = 'all';
}    // куда инкрустировать