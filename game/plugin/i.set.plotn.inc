<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */
// делаем броню из веток i.log

$arr = [    // id, name, dificalt -%, count_logs, 
            1 => ["i.a.m.oj", "ожерелье", 10, 1],
            2 => ["i.w.u.grom", "посох грома", 70, 15],
            3 => ["i.a.h.wood", "деревянный шлем", 5, 3],
            4 => ["i.a.p.wood", "дeрeвянныe пoручи", 10, 5],
            5 => ["i.a.l.wood", "дeрeвянныe пoнoжи", 20, 8],
            6 => ["i.a.s.wood", "дeрeвянный щит", 30, 10],
            7 => ["i.a.s.dub", "дубовый щит", 70, 20],
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
        $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&use=$use&to=$i\">" . $arr[$i][1] . " (" . $arr[$i][3] .
                 " веток, шанс: " . tsret($skills[27] * 20 - $arr[$i][2]) . "%)</a>";
    }
    msg($stmp);
}

if ($skills[27] * 20 - $arr[$to][2] > 0) {
    $char[6]                     = time() + 10;
    $loc_i[$loc][$login]["char"] = implode("|", $char);

    additem($loc, $login, "", "i.log", $arr[$to][3], "items", "items", 1, 0, 1);    // удаляем ветки

    if (rand(0, 100) < $skills[27] * 20 - $arr[$to][2]) {
        addjournal($loc, $login, "Вы сделали " . $arr[$to][1]);
        addjournal($loc, "all", $char[0] . " сделал " . $arr[$to][1], $login);
        additem($loc, "", $login, $arr[$to][0] . "_" . substr($login, 2) . "_", 1);
        if (round($arr[$to][2] / 10)) {
            addexp($loc, $login, round($arr[$to][2] / 10));
        }
    } else {
        addjournal($loc, $login, "Вы не смогли сделать " . $arr[$to][1]);
        addjournal($loc, "all", $char[0] . " не смог сделать " . $arr[$to][1], $login);
    }

} else {
    addjournal($loc, $login, "Ваш навык плотника слишком низок");
}
