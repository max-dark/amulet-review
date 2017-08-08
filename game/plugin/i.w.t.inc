<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */
// рубка веток с дерева

/*
 * 0 - название источника ресурса
 * 1 - текущее количество ресурса
 * 2 - таймер исчезновения. 0 == постоянный
 * 3 - период обновления после исчерпания ресурсв
 * 4 - количество ресурса при обновлении
 * 5 - таймер обновления
 */

if (isset($loc_i[$loc]["i.s.tree"])) {
    $skills = explode("|", $loc_i[$loc][$login]["skills"]);
    if ($skills[26] > 0) {
        $char[6]                     = time() + 4;
        $loc_i[$loc][$login]["char"] = implode("|", $char);

        $tree = explode("|", $loc_i[$loc]["i.s.tree"]);
        if ($tree[1] == 0 && time() > $tree[5]) {
            $tree[1] = $tree[4];
        }
        if ($tree[1] > 0) {
            if (rand(1, 100) < $skills[26] * 10) {
                $tree[1]--;
                if ($tree[1] == 0) {
                    $tree[5] = time() + $tree[3];
                }
                $loc_i[$loc]["i.s.tree"] = implode("|", $tree);
                addjournal($loc, $login, "Вы срубили ветку");
                addjournal($loc, "all", $char[0] . " срубил ветку", $login);
                manageItems($loc, "", $login, "i.log", 1);
                if (rand(1, 100) < 50) {
                    addexp($loc, $login, rand(1, 3));
                }
            } else {
                addjournal($loc, $login, "Вам не удалось срубить ветку");
                addjournal($loc, "all", $char[0] . " пытался срубить ветку", $login);
            }
        } else {
            addjournal($loc, $login, "На дереве не осталось веток! Ждите пока отрастут новые");
        }
    } else {
        addjournal($loc, $login, "Ваш навык лесоруба равен 0, вы не умеете рубить деревья");
    }
} else {
    addjournal($loc, $login, "Рядом нет деревьев, подходящих для рубки");
}

