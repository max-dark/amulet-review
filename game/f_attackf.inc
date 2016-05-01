<?php

global $loc_i, $loc_tt, $g_destroy, $g_crim, $g_exp, $PHP_SELF, $game;
// проверки
if ((substr($from, 0, 2) != 'u.' && substr($from, 0, 2) != 'n.') || (substr($to, 0, 2) != 'u.' && substr($to, 0, 2) != 'n.') || !isset($loc_i[$loc][$from]) || !isset($loc_i[$loc][$to]) || $from == $to) return;
$fchar = explode("|", $loc_i[$loc][$from]["char"]);
if (substr($from, 0, 2) == 'u.' && $fchar[8]) {
    if ($answer) addjournal($loc, $from, "Вы призрак");
    return;
}

$loct = $loc;
$aloct = explode("|", $loc_tt[$loc]["d"]);
$tchar = explode("|", $loc_i[$loct][$to]["char"]);
if (substr($to, 0, 2) == 'u.' && $tchar[8]) {
    if ($answer) addjournal($loc, $from, "Нельзя атаковать призрака");
    return;
}
$twar = explode("|", $loc_i[$loct][$to]["war"]);

if ($fchar[6] - time() > 300) $fchar[6] = time() - 1;
if (!$rmagic && time() <= $fchar[6]) {
    if ($answer) addjournal($loc, $from, "Вы должны отдохнуть " . round($fchar[6] - time() + 1) . " сек");
    return;
}
if ($loc_i[$loc][$to]["def"]) $tdef = explode("|", $loc_i[$loc][$to]["def"]); else $tdef = array("", "", 0);
if ($tdef[2] && time() > $tdef[2]) {
    $loc_i[$loc][$to]["def"] = "";
    $tdef = array("", "", 0);
}
if ($ptitle) $ptitle = " (" . $ptitle . ")";
$tloc = explode("x", $loc);
if ($magic) $fwar = explode("|", $magic); else $fwar = explode("|", $loc_i[$loc][$from]["war"]);
if ($answer) {
    $fchar[6] = time() + $fwar[3];
    $loc_i[$loc][$from]["char"] = implode("|", $fchar);
}
if ($fwar[12] == "мaгиeй") $fwar[12] = "магией";    //eng a,e
if ($rmagic || $fwar[12] == "магией" || $fwar[12] == "молнией") {
    if ($tdef[0] == "p.d.z" && rand(0, 100) <= $tdef[3] * 0.10) {
        if (substr($loc_i[$loc][$from]["def"], 0, 5) == "p.d.c") {
            $fdef = explode("|", $loc_i[$loc][$from]["def"]);
            $fdef = $fdef[3];
        } else $fdef = 0;
        if (rand(0, 100) > $fdef) $fwar[0] = 0;
    }
    if (substr($loc_i[$loc][$from]["def"], 0, 5) == "p.d.c") $loc_i[$loc][$from]["def"] = "";
    if ($tdef[0] == "p.d.z") {
        $loc_i[$loc][$to]["def"] = "";
        $t2 = $tdef[1];
    }
    $uklon = $twar[9];
    $parring = $twar[10];
    $shield = $twar[11];
} else {
    $uklon = $twar[6];
    $parring = $twar[7];
    $shield = $twar[8];
    if ($tdef[0] == "p.d.u" && $fwar[4]) {
        if (rand(0, 100) <= $tdef[3]) $uklon += 35;
        $loc_i[$loc][$to]["def"] = "";
        $t2 = $tdef[1];
    }
    if ($tdef[0] == "p.d.re") {
        if (rand(0, 100) <= $tdef[3]) $uklon += 20;
        $loc_i[$loc][$to]["def"] = "";
        $t2 = $tdef[1];
    }
    if ($tdef[0] == "p.d.p") {
        if (rand(0, 100) <= $tdef[3]) $parring *= 2;
        $loc_i[$loc][$to]["def"] = "";
        $t2 = $tdef[1];
    }
}

if ($priem == "p.g" && $tdef[0] == "p.d.g") {
    if (rand(0, 100) <= $tdef[3]) {
        $fwar[1] = 0;
        $fwar[2] = 0;
    }
    $loc_i[$loc][$to]["def"] = "";
    $t2 = $tdef[1];
}
if (substr($loc_i[$loc][$from]["def"], 0, 6) == "p.d.re") {
    $fwar[1] = round($fwar[1] * 0.6);
    $fwar[2] = round($fwar[2] * 0.6);
}
if ($tdef[0] == "p.d.o" && !$rmagic) {
    if (rand(0, 100) <= $tdef[3]) {
        $fwar[1] = round($fwar[1] * 0.4);
        $fwar[2] = round($fwar[2] * 0.4);
    }
    $t2 = $tdef[1];
}
if ($priem == "p.n" && $tdef[0] == "p.d.n" || $priem == "p.r" && $tdef[0] == "p.d.r" || $priem == "p.vs" && $tdef[0] == "p.d.s" || $priem == "p.vw" && strpos($loc_i[$loc][$to]["equip"], "i.a.s.") !== false) $t2 = $tdef[1];
if ($t2) $t2 = " (" . $t2 . ")";


// крим если атакует не крима или животное в городе
$fstp = strpos($fchar[0], "*");
$tstp = strpos($tchar[0], "*");
if ($fstp === false) $clan1 = ""; else $clan1 = substr($fchar[0], $fstp + 1, strrpos($fchar[0], "*") - $fstp - 1);
if ($tstp === false) $clan = ""; else $clan = substr($tchar[0], $tstp + 1, strrpos($tchar[0], "*") - $tstp - 1);
$fcrim = $fchar[9] || substr($from, 0, 4) == "n.c.";

$tcrim = $tchar[9] || substr($to, 0, 4) == "n.c." || $to == "n.w.Veelzevul" || $to == "n.whitewolf" || $game["floc"] == $loc && $game["fid"] == $to;
if ($tloc[2] >= 1099) {
    $tcrim = $tcrim || $tchar[14] == "p" || substr($to, 0, 4) == "n.p." || $fchar[14] == "p" && $tchar[14] == "t";
}
if ($fchar[13]) $wife = $to == substr($fchar[13], 0, strlen($to)); else $wife = 0;
if ($from != $to && !$fcrim && $tchar[7] != $from && !$tcrim && (!$clan1 || ($clan1 && $clan1 != $clan)) && !$wife && $from != "u.qv" && $to != "u.qv") {
    if (isset($loc_i[$loc][$to]["owner"])) docrim($loc, $from, "живодер");    //$aloct[1] && substr($to,0,4)=="n.a." ||
    else if (substr($to, 0, 4) != "n.a.") docrim($loc, $from, "бандит");
    $fchar = explode("|", $loc_i[$loc][$from]["char"]);
}

// патроны
if ($fwar[14])
    if (strpos($loc_i[$loc][$from]["items"], $fwar[14] . ":") !== false) {
        additem($loc, $from, "", $fwar[14], 1, "items", "", 0);
        if (strpos($loc_i[$loc][$from]["items"], $fwar[14] . ":") === false) addjournal($loc, $from, "Боеприпасы кончились");
    } else {
        addjournal($loc, $from, "Нет боеприпасов");
        return;
    }
// цель конник
if (substr($to, 0, 2) == "u." && $tchar[12] && !$rmagic && $fwar[12] != "магией") $fwar[0] -= 10;
if (substr($to, 0, 4) == "n.c.")
    if (strpos($loc_i[$loc][$from]["equip"], "i.a.m.vlast") !== false) {
        $fwar[1] = $fwar[1] * 2;
        $fwar[2] = $fwar[2] * 2;
    }

// заклинание сорвалось?
if ($fwar[0] || !$fwar[0] && !$rmagic && $fwar[12] != "магией") {
// попадание
    if (rand(0, 100) <= $fwar[0]) {
// урон
        $damage = round(rand($fwar[1], $fwar[2]));
// уклон
        if (rand(0, 100) > $uklon) {
// щит
            if ($parring && $shield) if (rand(0, 100) <= $parring) {
                if (!$rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией") {
                    $damage -= $shield;
                    $t1 = " (щит " . $shield . ")";
                } else {
                    $resist = round($damage * $shield / 100);
                    if ($resist) $tsh = rand(0, $resist); else $tsh = 0;
                    $damage -= $tsh;
                    $t1 = " (сопр. магии " . $tsh . ")";
                }
            }
// броня
            if (!$rmagic && $fwar[12] != "магией" && $twar[5] && $fwar[12] != "молнией" && $twar[5]) $damage -= round(rand(0, $twar[5])); // armor
            if ($damage < 0) $damage = 0;
            if ($fwar[4]) $skrit = 5; else if ($rmagic || $fwar[12] == "магией" || $fwar[12] == "молнией") $skrit = 1; else $skrit = 2;
            if ($damage && rand(0, 100) < $skrit) {
                $damage *= 2;
                $tkrit = " критически";
            } else $tkrit = "";
            if ($loc_i[$loct][$to]["god"]) $damage = 0;    // БОГ
// урон
            $tchar[1] -= $damage;
            $tchar[5] = time();
            if ($tchar[1] < 0) $tchar[1] = 0;
            if (!$answer && !$rmagic) {
                addjournal($loc, $from, "вы" . $ptitle . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2, "", "", ", ");
                addjournal($loc, "all", $fchar[0] . $ptitle . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2, $from, "", ", ");
            } else {
                addjournal($loc, $from, "Вы" . $ptitle . " по " . $tchar[0] . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2);
                addjournal($loc, $to, $fchar[0] . $ptitle . " по вам" . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2);
                addjournal($loc, "all", $fchar[0] . $ptitle . " по " . $tchar[0] . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2, $from, $to);
            }

// жена/муж
            if (substr($to, 0, 2) == "u." && $tchar[13] && $tchar[1] < $tchar[2]) {
                $tm = explode(":", $tchar[13]);
                if (time() > $tm[1] && file_exists("online/" . $tm[0]) && filesize("online/" . $tm[0]) != 1) {
                    $tmf = file("online/" . $tm[0]);
                    $tmf = trim($tmf[0]);
                    if ($tmf != $loc) {
                        loadloc($tmf);
                        $tup = explode("|", $loc_i[$tmf][$tm[0]]["user"]);
                        if (strpos($aloct[0], "*") !== false) $aloct[0] = substr($aloct[0], 0, strpos($aloct[0], "*"));
                        if ($tup[2] == "m") $ts = "Ваша жена (" . $aloct[0] . ") ранена!"; else $ts = "Ваш муж (" . $aloct[0] . ") ранен!";
                        addjournal($tmf, $tm[0], "<a href=\"$PHP_SELF?sid=" . $tm[0] . "&p=" . $tup[0] . "&stele=1\">" . $ts . "</a>");
                        $tm[1] = time() + 300;
                        $tchar[13] = implode(":", $tm);
                    }
                }
            }
// если убили, добавим труп
            if ($tchar[1] < 1) {
                include "f_kill.dat";
            } else $loc_i[$loct][$to]["char"] = implode("|", $tchar); // иначе сохраним в f_kill.dat

        } else {
            if (!$answer) {
                if (!$rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией") {
                    addjournal($loct, $from, "вы" . $ptitle . " мимо (уклон)" . $t2, "", "", ", ");
                    addjournal($loct, "all", $fchar[0] . $ptitle . " мимо (уклон)" . $t2, $from, "", ", ");
                } else {
                    addjournal($loct, $from, "вы" . $ptitle . " мимо (уклон от магии)" . $t2, "", "", ", ");
                    addjournal($loct, "all", $fchar[0] . $ptitle . " мимо (уклон от магии)" . $t2, $from, "", ", ");
                }
            } else {
                if (!$rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией") {
                    addjournal($loct, $from, "Вы" . $ptitle . " по " . $tchar[0] . " мимо (уклон)" . $t2);
                    addjournal($loct, $to, $fchar[0] . " по вам мимо (уклон)");
                    addjournal($loct, "all", $fchar[0] . $ptitle . " по " . $tchar[0] . " мимо (уклон)" . $t2, $from, $to);
                } else {
                    addjournal($loct, $from, "Вы" . $ptitle . " по " . $tchar[0] . " мимо (уклон от магии)" . $t2);
                    addjournal($loct, $to, $fchar[0] . $ptitle . " по вам мимо (уклон от магии)" . $t2);
                    addjournal($loct, "all", $fchar[0] . $ptitle . " по " . $tchar[0] . " мимо (уклон от магии)" . $t2, $from, $to);
                }
            }
        }
    } else {
        if (!$answer && !$rmagic) {
            addjournal($loc, $from, "вы" . $ptitle . " мимо" . $t2, "", "", ", ");
            addjournal($loc, "all", $fchar[0] . $ptitle . " мимо" . $t2, $from, "", ", ");
        } else {
            addjournal($loc, $from, "Вы" . $ptitle . " по " . $tchar[0] . " мимо" . $t2);
            addjournal($loc, $to, $fchar[0] . $ptitle . " по вам мимо" . $t2);
            addjournal($loc, "all", $fchar[0] . $ptitle . " по " . $tchar[0] . " мимо" . $t2, $from, $to);
        }
    }
}// заклинание сорвалось

// если npc свободен, то атакует
if (isset($loc_i[$loc][$from]) && ($answer || $rmagic)) {
    $fchar[7] = $to;
    $loc_i[$loc][$from]["char"] = implode("|", $fchar);
}
if (isset($loc_i[$loc][$from]) && isset($loc_i[$loct][$to]) && $from != $to && ($fwar[0] || !$fwar[0] && !$rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией")) {
    if (substr($to, 0, 2) == "n." && !$tсhar[7]) {
        $tchar[7] = $from;
        $loc_i[$loct][$to]["char"] = implode("|", $tchar);
    }
    if ($answer) attack($loct, $to, $from, 0, 0);
}