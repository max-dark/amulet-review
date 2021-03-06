<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global game
 * @global char
 */
// добыча руды

/*
 * 0 - название источника ресурса
 * 1 - текущее количество ресурса
 * 2 - таймер исчезновения. 0 == постоянный
 * 3 - период обновления после исчерпания ресурсв
 * 4 - количество ресурса при обновлении
 * 5 - таймер обновления
 */

$art = [
    ["i.i.ag", "агат", "5"],
    ["i.i.az", "амазонит", "5"],
    ["i.i.am", "аметист", "5"],
    ["i.i.do", "долерит", "5"],
    ["i.i.dy", "дымчатый кварц", "5"],
    ["i.i.zm", "змеевик", "5"],
    ["i.i.ku", "кунцит", "5"],
    ["i.i.ma", "малахит", "5"],
    ["i.i.me", "мельхиор", "5"],
    ["i.i.ne", "нефрит", "5"],
    ["i.i.ob", "обсидиан", "5"],
    ["i.i.on", "оникс", "5"],
    ["i.i.sa", "сапфир", "5"],
    ["i.i.hr", "хризолит", "5"],
];

if (isset($loc_i[$loc]["i.s.rudnik"])) {
    $skills = explode("|", $loc_i[$loc][$login]["skills"]);
    if ($skills[24] > 0) {
        $char[6]                     = time() + 4;
        $loc_i[$loc][$login]["char"] = implode("|", $char);
        $tree                        = explode("|", $loc_i[$loc]["i.s.rudnik"]);
        if ($tree[1] == 0 && time() > $tree[5]) {
            $tree[1] = $tree[4];
        }
        if ($tree[1] > 0) {
            if (rand(1, 100) < $skills[24] * 10) {
                $tree[1]--;
                if ($tree[1] == 0) {
                    $tree[5] = time() + $tree[3];
                }
                $loc_i[$loc]["i.s.rudnik"] = implode("|", $tree);
                addjournal($loc, $login, "Вы добыли немного руды");
                addjournal($loc, "all", $char[0] . " добыл немного руды", $login);
                manageItems($loc, "", $login, "i.ruda", 1);
                // самоцветы
                $tc = explode("x", $loc);
                if ($tc[2] >= 1099 || $loc == "c.5.gate") {
                    $rnd = rand(0, count($art) - 1);
                    if (rand(0, 100) < $art[$rnd][2]) {
                        addjournal($loc, $login, "Вы нашли " . $art[$rnd][1] . "!");
                        addjournal($loc, "all", $char[0] . " нашел " . $art[$rnd][1] . "!", $login);
                        manageItems($loc, "", $login, $art[$rnd][0], 1);
                    }
                }
                if (rand(1, 100) < 30) {
                    addexp($loc, $login, 1);
                }
            } else {
                addjournal($loc, $login, "Вам не удалось добыть руду");
                addjournal($loc, "all", $char[0] . " пытался добыть руду", $login);
            }
        } else {
            addjournal($loc, $login, "В жиле нет больше руды! Ждите пока появится снова");
        }
    } else {
        addjournal($loc, $login, "Ваш навык рудокопа равен 0, вы не умеете добывать руду");
    }
} else {
    addjournal($loc, $login, "Рядом нет рудной жилы");
}

