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
 * @global war
 * @global tchar
 * @global g_list
 * @global trader
 * @global title
 * @global skilldown
 * @global to
 */

// диалог повышения скилла или аттрибута на 1

$tmp   = explode("|", $title);
$skill = $tmp[1];
$cost  = $tmp[2];
$min   = $tmp[3];
$max   = $tmp[4];

$skills = explode("|", $loc_i[$loc][$login]["skills"]);
if (substr($skill, 0, 2) == "m.") {
    // магия
    // проверки
    if (strpos($loc_i[$loc][$login]["magic"], $skill . ":") !== false) {
        msg("<p>У вас уже есть это заклинание", $tchar[0]);
    }
    if ($min) {
        if ($skills[13] < $min) {
            msg("<p>У вас недостаточный навык магии (надо минимум " . $min . ")", $tchar[0]);
        }
    }
    if ($max) {
        if ($skills[13] > $max) {
            msg("<p>У вас слишком высокий навык магии (максимум " . $max . ")", $tchar[0]);
        }
    }
    if ($cost) {
        $money = intval(preg_replace('/.*i\.money:(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
        if ($money < $cost) {
            msg("<p>У вас недостаточно денег (надо " . $cost . " монет)", $tchar[0]);
        }
        additem($loc, $login, "", "i.money", $cost, "items", "items", 1, -1, 1);
    }
    if ( ! $loc_i[$loc][$login]["magic"]) {
        $loc_i[$loc][$login]["magic"] = $skill . ":1";
    } else {
        $loc_i[$loc][$login]["magic"] .= "|" . $skill . ":1";
    }
    msg("<p>Вы выучили новое заклинание!", $tchar[0]);
}
if (substr($skill, 0, 2) == "p.") {
    include "f_speakskillupp.inc"; // без возврата управления
}

$lev = 0;
for ($i = 0; $i < count($skills); $i++) {
    if ($i != 3) {
        $lev += $skills[$i];
    }
}

$arr_skills = [
    "str"          => 0,
    "dex"          => 1,
    "int"          => 2,
    "meditation"   => 5,
    "steal"        => 6,
    "animaltaming" => 7,
    "hand"         => 8,
    "coldweapon"   => 9,
    "ranged"       => 10,
    "parring"      => 11,
    "uklon"        => 12,
    "magic"        => 13,
    "magic_resist" => 14,
    "magic_uklon"  => 15,
    "regeneration" => 16,
    "hiding"       => 17,
    "look"         => 18,
    "steallook"    => 19,
    "animallore"   => 20,
    "spirit"       => 21,
    "healing"      => 22,
    "alchemy"      => 23,
    "mine"         => 24,
    "smith"        => 25,
    "lumb"         => 26,
    "bow"          => 27,
    "stone"        => 28,
    "fish"         => 29,
    "food"         => 30,
    "necro"        => 31,
    "currier"      => 32
];

$arr_title = [
    "str"          => "Сила",
    "dex"          => "Ловкость",
    "int"          => "Интеллект",
    "meditation"   => "Медитация",
    "steal"        => "Кража",
    "animaltaming" => "Прир.животных",
    "hand"         => "Рукопашная",
    "coldweapon"   => "Холодн.оружие",
    "ranged"       => "Стрельба",
    "parring"      => "Парирование",
    "uklon"        => "Уклон",
    "magic"        => "Магия",
    "magic_resist" => "Сопр.магии",
    "magic_uklon"  => "Уклон от магии",
    "regeneration" => "Регенерация",
    "hiding"       => "Скрытность",
    "look"         => "Осторожность",
    "steallook"    => "Подглядывание",
    "animallore"   => "Изуч.животных",
    "spirit"       => "Спиритизм",
    "healing"      => "Лечение",
    "alchemy"      => "Алхимия",
    "mine"         => "Рудокоп",
    "smith"        => "Кузнец",
    "lumb"         => "Лесоруб",
    "bow"          => "Плотник",
    "stone"        => "Ювелир",
    "fish"         => "Рыболов",
    "food"         => "Повар",
    "necro"        => "Некромант",
    "currier"      => "друид",
];

// проверки
if ( ! isset($arr_skills[$skill])) {
    msg("<p>Несуществующий скилл: $skill в диалоге с $speak, сообщите разработчику игры.", "Ошибка");
}
if ($skills[4] < 1) {
    msg("<p>Недостаточно очков опыта", $tchar[0]);
}
if ($min) {
    if ($skills[$arr_skills[$skill]] < $min) {
        msg("<p>Вы должны иметь уровень навыка не ниже " . $min, $tchar[0]);
    }
}
if ($max) {
    if ($skills[$arr_skills[$skill]] > $max) {
        msg("<p>Вы и так достаточно опытны, я учу только до уровня " . ($max + 1), $tchar[0]);
    }
}

// надо ли понизить
if ($skill == 'str' || $skill == 'dex' || $skill == 'int') {    //АТТР
    if ($skills[$arr_skills[$skill]] >= $g_attr_one) {
        msg("<p>Невозможно повысить, т.к. аттрибут уже на максимальном уровне " . $g_attr_one, $tchar[0]);
    }
    if ($skilldown && $skilldown != "str" && $skilldown != "dex" && $skilldown != "int") {
        msg("Неверный аттрибут");
    }
    if ($skilldown && $skills[$arr_skills[$skilldown]] <= 1) {
        msg("<p>Невозможно понизить, т.к. аттрибут уже на минимальном уровне 1, выберите другой", $tchar[0]);
    }
    if ( ! $skilldown) {
        if ($skills[0] + $skills[1] + $skills[2] >= $g_attr) {
            $stmp = "<p>Превышен предел суммы очков (" . $g_attr . ") для аттрибутов, выберите что уменьшить: ";
            if ($skill != 'str') {
                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=str\">Сила: " . $skills[0] .
                         "</a>";
            }
            if ($skill != 'dex') {
                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=dex\">Ловкость: " .
                         $skills[1] . "</a>";
            }
            if ($skill != 'int') {
                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=int\">Интеллект: " .
                         $skills[2] . "</a>";
            }
            msg($stmp, $tchar[0]);
        }
    }
} else {    //НАВЫКИ
    if ($skills[$arr_skills[$skill]] >= $g_skills_one) {
        msg("<p>Невозможно повысить, т.к. навык уже на максимальном уровне " . $g_skills_one);
    }
    if ($skilldown && ($skilldown == "str" || $skilldown == "dex" || $skilldown == "int")) {
        msg("Неверный навык");
    }
    if ($skilldown && $skills[$arr_skills[$skilldown]] <= 0) {
        msg("<p>Невозможно понизить, т.к. навык уже на минимальном уровне 0, выберите другой");
    }
    $sum = 0;
    foreach (array_keys($arr_skills) as $i) {
        if ($i != 'str' && $i != 'dex' && $i != 'int') {
            $sum += $skills[$arr_skills[$i]];
        }
    }
    if ( ! $skilldown) {
        if ($sum >= $g_skills) {
            $stmp = "<p>Превышен предел суммы очков (" . $g_skills . ") для навыков, выберите что уменьшить: ";
            if ( ! $start) {
                $start = 0;
            }
            $keys = array_keys($arr_skills);
            for ($i = $start; $i < $start + $g_list && $i < count($keys); $i++) {
                if ($keys[$i] != 'str' && $keys[$i] != 'dex' && $keys[$i] != 'int') {
                    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=" . $keys[$i] . "\">" .
                             $arr_title[$keys[$i]] . ": " . $skills[$arr_skills[$keys[$i]]] . "</a>";
                }
            }
            if (count($keys) > 1 && $start) {
                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id\">^ </a>";
            }
            if ($start + $g_list < count($keys)) {
                if ( ! $start) {
                    $stmp .= "\n<br/>";
                }
                $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=" . ($start + $g_list) . "\">+ (" .
                         (count($keys) - $start - $g_list) . ")</a>";
            }
            msg($stmp, $tchar[0]);
        }
    }
}

// хватит ли денег
if ($cost) {
    $twar = explode("|", $loc_i[$loc][$login]["war"]);
    if ($war[13] == 0) {
        calcparam($loc, $login);
    }
    if ($lev == 5) {
        $stmp = "<p>Ладно, так уж и быть, раз ты новичок, то я это сделаю бесплатно.</p>";
    } else {
        $money = intval(preg_replace('/.*i\.money:(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
        if ($money < $cost) {
            msg("<p>У вас недостаточно денег (надо " . $cost . " монет)");
        }
        additem($loc, $login, "", "i.money", $cost, "items", "items", 1, -1, 1);
    }
}

// повышаем +1
$skills[$arr_skills[$skill]] += 1;
if ($skilldown) {
    $skills[$arr_skills[$skilldown]] -= 1;
}
$skills[4] -= 1;    // очки опыта
$loc_i[$loc][$login]["skills"] = implode("|", $skills);
calcparam($loc, $login);
$stmp .= "<p>" . $arr_title[$skill] . ": +1";
if ($skilldown) {
    $stmp .= "<br/>" . $arr_title[$skilldown] . ": -1";
}
msg($stmp, $tchar[0]);