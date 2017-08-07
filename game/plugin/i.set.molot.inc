<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */
// делаем из руды i.ruda

$arr = [
    //    [ id, название, "сложность" -%, количество ресурса],
    10 => ["i.a.k.ring", "кольцо", 0, 1],
    11 => ["i.a.n.br", "браслет", 0, 1],
     1 => ["i.w.k.kor", "короткий нож", 0, 1],
     2 => ["i.w.s.short", "короткий меч", 20, 4],
     3 => ["i.w.t.plotnik", "плотницкий топор", 20, 4],
     4 => ["i.w.s.kach", "качественный меч", 50, 8],
     5 => ["i.w.s.master", "меч мастера", 70, 20],
     6 => ["i.a.s.krugl", "круглый щит", 10, 5],
     7 => ["i.a.s.prochn", "прочный щит", 40, 10],
     8 => ["i.a.b.mkol", "медная кольчуга", 50, 10],
     9 => ["i.a.b.sborn", "сборный доспех", 75, 30],
    12 => ["i.a.b.mag", "роба магистра", 75, 30],
];

if (isset($loc_i[$loc]["i.s.nakov"])) {
    $skills = explode("|", $loc_i[$loc][$login]["skills"]);
    function tsret($s)
    {
        if ($s > 0) {
            return $s;
        } else {
            return 0;
        }
    }

    if (!empty($to) || !empty($arr[$to])) {
        $stmp = "Выберите что сделать:";
        foreach ($arr as $i => $thing) {
            $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&use=$use&to=$i\">" . $thing[1] . " (" . $thing[3] .
                     " руды, шанс: " . tsret($skills[25] * 20 - $thing[2]) . "%)</a>";
        }
        msg($stmp);
    }

    if ($skills[25] * 20 - $arr[$to][2] > 0) {
        $char[6]                     = time() + 15;
        $loc_i[$loc][$login]["char"] = implode("|", $char);

        manageItems($loc, $login, "", "i.ruda", $arr[$to][3], "items", "items", 1, 0, 1);    // удаляем руду

        if (rand(0, 100) < $skills[25] * 20 - $arr[$to][2]) {
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
        addjournal($loc, $login, "Ваш навык кузнеца слишком низок, вам нужно вначале его повысить");
    }
} else {
    addjournal($loc, $login, "Чтобы ковать металл, рядом с вами должна быть наковальня");
}