<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */

// инструменты кожевника, изготавливают одежду для магов из шкур и кислоты.

$arr = [    // id, name, dificalt -%, count, need_title, need_id
            1 => ["i.a.h.wolf", "голова волка", 65, 1, "шкура волка", "i.h.s.wolf"],
            2 => ["i.a.h.whitewolf", "голова белого волка", 85, 1, "шкура белого волка", "i.h.s.whitewolf"],
            3 => ["i.a.p.ogr", "лапы огра", 70, 2, "шкуры огра", "i.h.s.ogr"],
            4 => ["i.a.l.ogr", "ноги огра", 65, 2, "шкуры огра", "i.h.s.ogr"],
            5 => ["i.a.b.troll", "дубленная кожа тролля", 80, 3, "шкуры тролля", "i.h.s.troll"],
];

$skills = explode("|", $loc_i[$loc][$login]["skills"]);

function tsret($s)
{
    if ($s > 0) {
        return $s;
    } else {
        return 0;
    }
}

if ( ! $to || ! isset($arr[$to])) {
    $stmp = "Выберите что сделать:";
    foreach (array_keys($arr) as $i) {
        $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&use=$use&to=$i\">" . $arr[$i][1] . " (" . $arr[$i][3] . " " .
                 $arr[$i][4] . ", шанс: " . tsret($skills[32] * 20 - $arr[$i][2]) . "%)</a>";
    }
    msg($stmp);
}

if ($skills[32] * 20 - $arr[$to][2] > 0) {
    if (manageItems($loc, $login, "", "i.acid", "count") == 0) {
        msg("Необходима кислота");
    }
    $char[6]                     = time();
    $loc_i[$loc][$login]["char"] = implode("|", $char);

    manageItems($loc, $login, "", $arr[$to][5], $arr[$to][3], "items", "items", 1, 0, 1);    // удаляем ингридиенты
    manageItems($loc, $login, "", "i.acid", 1, "items", "items", 1, 0, 1);            // кислоту
    if (rand(0, 100) < $skills[32] * 20 - $arr[$to][2]) {
        addjournal($loc, $login, "Вы сделали " . $arr[$to][1]);
        addjournal($loc, "all", $char[0] . " сделал " . $arr[$to][1], $login);
        manageItems($loc, "", $login, $arr[$to][0] . "_" . substr($login, 2) . "_", 1);

        if (round($arr[$to][2] / 10)) {
            addexp($loc, $login, round(rand(0, $arr[$to][2] / 10)));
        }
    } else {

        addjournal($loc, $login, "у вас не получилось сделать" . $arr[$to][1]);
        addjournal($loc, "all", $char[0] . " не смог сделать " . $arr[$to][1], $login);
    }

} else {
    addjournal($loc, $login, "Ваш навык кожевника слишком низок");
}