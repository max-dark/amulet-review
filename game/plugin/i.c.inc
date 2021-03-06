<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */

// шлифуем камень, если есть набор для шлифования, набор может сломаться

if (intval(preg_replace('/.*' . "i.set.shlif" . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["items"])) > 0) {
    $skills = explode("|", $loc_i[$loc][$login]["skills"]);
    if ($skills[28] > 0) {

        if (rand(1, 100) < $skills[28] * 10) {
            manageItems($loc, $login, "", $use, 1);
            manageItems($loc, "", $login, $use . ".good", 1);
            addjournal($loc, $login, "Вы отшлифовали камень");
            addjournal($loc, "all", $char[0] . " отшлифовал камень");
        } else {
            addjournal($loc, $login, "Вам не удалось отшлифовать камень");
            addjournal($loc, "all", $char[0] . " не смог отшлифовать камень");
        }

        // шлифовалка может сломаться всегда 5%
        if (rand(1, 100) <= 5) {
            addjournal($loc, $login, "Вы сломали набор для шлифования камней");
            addjournal($loc, "all", $char[0] . " набор для шлифования камней", $login);
            manageItems($loc, $login, "", "i.set.shlif", 1);
        }

    }
    addjournal($loc, $login, "Ваш навык камнетеса равен 0, вы не умеете обрабатывать камни");
} else {
    addjournal($loc, $login, "Чтобы обработать камень, у вас должен быть в инвентори набор для шлифования камней");
}