<?php
// замковые функции
if ($ctele) {
    loadloc($ctele);
    if (!isset($loc_tt[$ctele]) || substr($ctele, 3) != ".gate") return;
    $gate = $ctele;
} else $gate = substr($loc, 0, 4) . "gate";


$d = explode("|", $loc_tt[$gate]["d"]);
if (strpos($d[0], "*") === false) $clanc = ""; else $clanc = substr($d[0], strpos($d[0], "*") + 1, strrpos($d[0], "*") - strpos($d[0], "*") - 1);
$char = explode("|", $loc_i[$loc][$login]["char"]);
if (strpos($char[0], "*") === false) $clan = ""; else $clan = substr($char[0], strpos($char[0], "*") + 1, strrpos($char[0], "*") - strpos($char[0], "*") - 1);
if (!$ctele && $char[8] && substr($loc, 3) != ".in" && $clan != $clanc && strpos($d[0], ":" . $login . ":") === false) {
    addnpc($login, $loc, substr($gate, 0, 4) . "in");
    msg("Призрак не может находиться в не своем замке.");
}

$arr = array("main", "gate", "tron", "sklad");
if ($ctele) {
    if ($clanc && $clanc == $clan) {// телепорт только если есть враги
        $tc = substr($ctele, 0, 4);
        foreach ($arr as $i) {
            loadloc($tc . $i);
            if ($loc_i[$tc . $i]) foreach (array_keys($loc_i[$tc . $i]) as $k) if ($k != $login && substr($k, 0, 2) == "u.") {
                $tchar = explode("|", $loc_i[$tc . $i][$k]["char"]);
                if (strpos($tchar[0], "*") === false) $tclan = ""; else $tclan = substr($tchar[0], strpos($tchar[0], "*") + 1, strrpos($tchar[0], "*") - strpos($tchar[0], "*") - 1);
                if ($tclan != $clanc && strpos($d[0], ":" . $k . ":") === false) {
                    addnpc($login, $loc, $ctele);
                    return;
                }
            }
        }
    }
    return;
}
if (substr($loc, 3, 3) == ".in") {
    include "f_castlein.inc";
    return;
}

// заперто/открыто
if (substr($loc, 3) != ".in") {
    if (strpos($d[0], "{") === false) $timez = 0; else $timez = substr($d[0], strpos($d[0], "{") + 1, strrpos($d[0], "}") - strpos($d[0], "{") - 1);
    if (time() < $timez) {
        addnpc($login, $loc, substr($gate, 0, 4) . "in");
        msg("Ворота замка заперты изнутри, их могут открыть только хозяева замка, либо стражники откроют сами, когда истечет максимальный срок.");
    } else if (strpos($d[0], "#") === false) {    // открываем, время истекло
        if ($timez != 0) $d[0] = substr($d[0], 0, strpos($d[0], "{")) . substr($d[0], strpos($d[0], "}") + 1);
        $d[0] .= "#" . ($timez + 60 * 60 * 8) . "#";
        $loc_tt[$gate]["d"] = implode("|", $d);
        $loc_tt[$gate]["save"] = 1;
    }
}
//---

$b = 1;
if (substr($loc, 3) == ".gate" && $clanc && $clanc != $clan && strpos($d[0], ":" . $login . ":") === false) {            // вход
    $tc = substr($loc, 0, 4);
    $b = 1;
    foreach ($arr as $i) {
        loadloc($tc . $i);
        if ($loc_i[$tc . $i]) foreach (array_keys($loc_i[$tc . $i]) as $k) if ($k != $login) {
            $tchar = explode("|", $loc_i[$tc . $i][$k]["char"]);
            if (strpos($tchar[0], "*") === false) $tclan = ""; else $tclan = substr($tchar[0], strpos($tchar[0], "*") + 1, strrpos($tchar[0], "*") - strpos($tchar[0], "*") - 1);
            if (substr($k, 0, 4) == "n.o." || (substr($k, 0, 2) == "u." && !$tchar[8] && ($tclan == $clanc || strpos($d[0], ":" . $k . ":") !== false))) $b = 0;
        }
    }

    $tclan = unserialize((file_get_contents("data/clans/" . $clanc)));
    if ($tclan) {
        $k1 = array_keys($tclan["g"]);
        $tclan["m"][$k1[0]] = 1;//глава
        foreach (array_keys($tclan["m"]) as $k) if (file_exists("online/" . $k) && filesize("online/" . $k) != 1) {
            $tmp = file("online/" . $k);
            $tmp[0] = trim($tmp[0]);
            loadloc($tmp[0]);
            $tu = explode("|", $loc_i[$tmp[0]][$k]["user"]);
            if ($tmp[0] != $gate) addjournal($tmp[0], $k, "<a href=\"$PHP_SELF?sid=$k." . $tu[0] . "&amp;ctele=" . $tc . "gate\">На ваш замок напали!</a>");
        }
    }
}

// захват замка
if (substr($loc, 3, 5) == ".gate" && $b && $clan && $clanc != $clan && strpos($d[0], ":" . $login . ":") === false) {
    if ($clanc) $tclan = unserialize((file_get_contents("data/clans/" . $clanc))); else $tclan = 0;    // сообщ. о захвате замка
    if ($tclan) {
        $k1 = array_keys($tclan["g"]);
        $tclan["m"][$k1[0]] = 1;//глава
        foreach (array_keys($tclan["m"]) as $k) if (file_exists("online/" . $k) && filesize("online/" . $k) != 1) {
            $tmp = file("online/" . $k);
            $tmp[0] = trim($tmp[0]);
            loadloc($tmp[0]);
            $tu = explode("|", $loc_i[$tmp[0]][$k]["user"]);
            addjournal($tmp[0], $k, "Ваш замок захватил клан " . $clan . "!");
        }
    }
    if ($clanc) $d[0] = substr($d[0], 0, strpos($d[0], "*")) . "*" . $clan . "*()" . "#" . (time() + 60 * 60 * 2) . "#";
    else $d[0] .= "*" . $clan . "*()" . "#" . (time() + 60 * 60 * 2) . "#";
    addjournal($loc, $login, "Вы захватили замок!");
    $loc_tt[$gate]["d"] = implode("|", $d);
    $loc_tt[$gate]["save"] = 1;
}
