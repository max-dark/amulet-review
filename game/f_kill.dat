<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global to
 * @global from
 * @global exits
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global tchar
 * @global twar
 * @global fwar
 * @global fcrim
 */
// если убили
$fchar[7]                   = "";
$loc_i[$loc][$from]["char"] = implode("|", $fchar);
addjournal($loc, "all", $tchar[0] . " погиб", $to);
// труп
$id = "i.s.d." . $to . "." . rand(5, 99);
while (isset($loc_i[$loc][$id])) {
    $id = "i.s.d." . $to . "." . rand(5, 99);
}
$item = "труп (" . $tchar[0] . ")|";
if ($tcrim || substr($to, 0, 4) == "n.a." || (substr($loc, 0, 2) == "c." && substr($loc, 3) != ".in")) {
    $item .= "1|";
} else {
    $item .= "0|";
}
$item .= time() + $g_destroy . "|";
if (strpos($loc_i[$loc][$to]["items"], "i.q.pjpt:") === false || substr($to, 0, 2) != "u.") {
    // предметы
    if ($loc_i[$loc][$to]["items"]) {
        $item .= str_replace("|", ",", $loc_i[$loc][$to]["items"]) . "|";
    } else {
        $item .= "|";
    }
    $pjpt = 0;
} else {
    $pjpt = 1;
    addjournal($loc, "all", $tchar[0] . " спасает свои вещи пером жар-птицы!", $to);
    addjournal($loc, $to, "Вы спасли свои вещи пером жар-птицы!");
    additem($loc, $to, "", "i.q.pjpt", 1);
    $item .= "|";
}
// шкура и т.д.
if ($loc_i[$loc][$to]["osvej"]) {
    $item .= str_replace("|", ",", $loc_i[$loc][$to]["osvej"]);
}
$loc_i[$loc][$id] = $item;
// убийца
if (substr($to, 0, 2) == "u." && substr($from, 0, 2) == "u." && $from != $to && $tchar[7] != $from && ! $tcrim &&
    ( ! $clan1 || ($clan1 && $clan1 != $clan)) && ! $wife
) {
    $loc_i[$loc][$from]["char"] = implode("|", $fchar);
    docrim($loc, $from, "убийца");
    $fchar = explode("|", $loc_i[$loc][$from]["char"]);
}
// статус игрока
if (substr($to, 0, 2) == 'u.') {
    if ($game["floc"] == $loc && $game["fid"] == $to) {
        addjournal($loc, "all", $tchar[0] . " потерял флаг!", $to);
        $loc_i[$loc]["i.flag"] = "флаг лидерства|1|0";
        $game["fid"]           = "";
    }
    $loc_i[$loc][$to]["equip"] = "";
    if ( ! $pjpt) {
        $loc_i[$loc][$to]["items"] = "";
    }
    $tchar[7] = "";
    $tchar[8] = 1;
    if ($tchar[12]) {
        $npc    = include "npc/n.a.losh";
        $tc     = explode(":", $tchar[12]);
        $tc1    = explode("|", $npc["char"]);
        $tc1[5] = time();
        if ($tc[1] < $tc1[1]) {
            $tc1[1] = $tc[1];
        }
        if (isset($loc_i[$loc][$to]["name"])) {
            $tc1[0] .= " " . $loc_i[$loc][$to]["name"];
            $npc["name"] = $loc_i[$loc][$to]["name"];
            unset($loc_i[$loc][$to]["name"]);
        }
        $npc["char"]                               = implode("|", $tc1);
        $npc["owner"]                              = $to . "|" . $to . "|0|0|0";
        $loc_i[$loc]["n.a.losh." . rand(99, 9999)] = $npc;
    }
    $tchar[12]                = "";
    $loc_i[$loc][$to]["char"] = implode("|", $tchar);
    if ( ! $tcrim && $fcrim && substr($from, 0, 2) == "u." && $from != $to) {    //killedby
        if ($twar[15]) {
            $tmp      = explode(":", $twar[15]);
            $twar[15] = $from . ":" . $tmp[1];
        } else {
            $twar[15] = $from . ":";
        }
        $loc_i[$loc][$to]["war"] = implode("|", $twar);
    }
    calcparam($loc, $to);    // пересчитаем параметры
    addjournal($loc, $to, "Вы погибли!");
} else {    // npc респавн
    if ($twar[15]) {
        $resp = explode(":", $twar[15]);
        if ($resp[3]) {
            // восстановить здоровье
            $tchar[1] = $tchar[2];
            // восстановить ману
            $tchar[3] = $tchar[4];
            // сбросить цель атаки
            $tchar[7] = "";
            // убрать коня
            $tchar[12]                = "";
            $loc_i[$loc][$to]["char"] = implode("|", $tchar);
            $loc_i[$loc][$to]["id"]   = $to;
            // подгрузить оригинал
            $npc = "npc/" . $to;
            $npc = include $npc;
            // сбросить настройки вещей
            $loc_i[$loc][$to]["items"] = $npc["items"];
            $loc_i[$loc][$to]["equip"] = $npc["equip"];
            $loc_i[$loc][$to]["osvej"] = $npc["osvej"];
            addtimer($resp[0], 0, rand($resp[1], $resp[2]), $loc_i[$loc][$to], 0);
        } else {
            if ($tchar[10]) {
                $tt = "|" . $tchar[10];
            } else {
                $tt = "";
            }
            addtimer($resp[0], 0, rand($resp[1], $resp[2]), $to . "|" . $resp[1] . ":" . $resp[2] . $tt, 0);
        }
    }
    // удаляем npc
    unset($loc_i[$loc][$to]);
}
// призрака не преследует
$fchar[7]                   = "";
$loc_i[$loc][$from]["char"] = implode("|", $fchar);

// экспа и уровень
if (substr($from, 0, 2) == 'u.') {
    if (substr($to, 0, 2) == 'u.') {
        $fwarr     = explode("|", $loc_i[$loc][$from]["war"]);
        $fwarr[16] = $to;
        if ( ! $fcrim && $tcrim && substr($to, 0, 2) == "u." && $from != $to) {    //killedto
            if ($fwarr[15]) {
                $tmp       = explode(":", $fwarr[15]);
                $fwarr[15] = $tmp[0] . ":" . $to;
            } else {
                $fwarr[15] = ":" . $to;
            }
        }
        $loc_i[$loc][$from]["war"] = implode("|", $fwarr);
    }
    if (substr($to, 0, 2) != "u.") {    //killedto
        $skills = explode("|", $loc_i[$loc][$from]["skills"]);
        $skills[3] += $twar[13];
        addjournal($loc, $from, "Опыт +" . intval($twar[13]));
        if ($skills[3] > $fwar[13] * $g_exp) {
            $skills[3] = 0;
            $skills[4] += 1;
            addjournal($loc, $from, "Вы получили 1 очко опыта!");
        }
        $loc_i[$loc][$from]["skills"] = implode("|", $skills);
        if ($skills[3] == 0) {
            calcparam($loc, $from); //levelup
        }
    }
}