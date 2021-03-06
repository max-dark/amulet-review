<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */
// делаем луки из веток i.log

$arr = [    // id, name, dificalt -%, count_logs, 
            1 => ["i.w.r.b.detsk", "детский лук", 0, 1],
            2 => ["i.w.r.b.short", "короткий лук", 10, 2],
            3 => ["i.w.r.b.samod", "самодельный лук", 20, 4],
            4 => ["i.w.r.b.prochn", "прочный лук", 40, 8],
            5 => ["i.w.r.b.kach", "качественный лук", 50, 12],
            6 => ["i.w.r.b.master", "лук мастера", 70, 20],
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
    $char[6]                     = time() + 15;
    $loc_i[$loc][$login]["char"] = implode("|", $char);

    manageItems($loc, $login, "", "i.log", $arr[$to][3], "items", "items", 1, 0, 1);    // удаляем ветки
    manageItems($loc, $login, "", "i.set.bow", 1, "items", "items", 1, 0, 1);        // удаляем тетиву

    if (rand(0, 100) < $skills[27] * 20 - $arr[$to][2]) {
        addjournal($loc, $login, "Вы сделали " . $arr[$to][1]);
        addjournal($loc, "all", $char[0] . " сделал " . $arr[$to][1], $login);
        manageItems($loc, "", $login, $arr[$to][0] . "_" . substr($login, 2) . "_", 1);
        if (round($arr[$to][2] / 10)) {
            addexp($loc, $login, round($arr[$to][2] / 10));
        }
    } else {
        addjournal($loc, $login, "Вы не смогли сделать " . $arr[$to][1]);
        addjournal($loc, "all", $char[0] . " не смог сделать " . $arr[$to][1], $login);
    }

} else {
    addjournal($loc, $login, "Ваш навык плотника слишком низок, чтобы делать луки");
}
