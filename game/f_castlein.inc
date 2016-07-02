<?php

// проверка флага лидерства
if ($game["fid"] != $login && intval($game["floc"]{2}) == $loc{2}) {
    loadloc($game["floc"]);
    if (!$game["fid"] || !isset($loc_i[$game["floc"]][$game["fid"]])) {
        $loc_i[$loc]["i.flag"] = "флаг лидерства|1|0";
        $game["floc"] = $loc;
        $game["fid"] = "";
    }
}

if ($clanc) {    // инфа
    if ($char[8] && $stuk) {
        msg("Призраки не могут стучать в ворота");
    }
    if ($stuk) {    // стук в ворота
        if (strpos($d[0], "[") !== false) {
            $tmp = substr($d[0], strpos($d[0], "[") + 1, strpos($d[0], "]") - strpos($d[0], "[") - 1);
        }
        else {
            $tmp = time() - 1;
        }
        if (time() < $tmp) {
            msg("Стучать можно не чаще чем раз в 3 минуты");
        }
        $tclan = unserialize((file_get_contents("data/clans/" . $clanc)));
        if ($tclan) {
            $k1 = array_keys($tclan["g"]);
            $tclan["m"][$k1[0]] = 1;//глава
            $ctitle = preg_replace('/Замок ([А-я]+).*/', "\\1", $d[0]);
            foreach (array_keys($tclan["m"]) as $k) {
                if (file_exists("online/" . $k) && filesize("online/" . $k) != 1) {
                    $tmp = file("online/" . $k);
                    $tmp[0] = trim($tmp[0]);
                    loadloc($tmp[0]);
                    $tu = explode("|", $loc_i[$tmp[0]][$k]["user"]);
                    addjournal($tmp[0], $k, $char[0] . " стучит в ворота вашего замка " . $ctitle . "!");
                }
            }
            if (strpos($d[0], "[") === false) {
                $d[0] .= "[" . (time() + 60 * 3) . "]";
            }
            else {
                $d[0] = preg_replace("/\[\d+\]/", "[" . (time() + 60 * 3) . "]", $d[0]);
            }
            $loc_tt[$gate]["d"] = implode("|", $d);
            $loc_tt[$gate]["save"] = 1;
        }
        msg("Вы постучали в ворота, все кто состоит в клане, которому принадлежит замок, извещены об этом.");
    }

    $stmp .= "<br/>Замок принадлежит клану " . $clanc;
    if (strpos($d[0], "(") === false) {
        $d[0] .= "()";
    }
    if (strpos($d[0], "()") !== false) {
        $ttext = "";
    }
    else {
        $ttext = substr($d[0], strpos($d[0], "(") + 1, strpos($d[0], ")") - strpos($d[0], "(") - 1);
    }

    // заперто/открыто
    if (strpos($d[0], "{") === false) {
        $timez = 0;
    }
    else {
        $timez = substr($d[0], strpos($d[0], "{") + 1, strrpos($d[0], "}") - strpos($d[0], "{") - 1);
    }
    if (time() < $timez) {
        $stmp .= " (ворота заперты до " . date("d/m H:i", $timez) . ")";
    }
    //---

    if ($clanc == $clan) {
        $stmp .= ", можете войти";
        if (time() < $timez) {
            if ($copen) {
                // ок, открываем
                if (strpos($d[0], "{") !== false) {
                    $d[0] = substr($d[0], 0, strpos($d[0], "{")) . substr($d[0], strpos($d[0], "}") + 1);
                }
                if (strpos($d[0], "#") !== false) {
                    $d[0] = substr($d[0], 0, strpos($d[0], "#")) . substr($d[0], strrpos($d[0], "#") + 1);
                }
                $d[0] .= "#" . (time() + 60 * 60 * 8) . "#";
                $loc_tt[$gate]["d"] = implode("|", $d);
                $loc_tt[$gate]["save"] = 1;
                msg("Вы открыли ворота, чтобы запереть их снова поговорите со стражниками");
            }
            $stmp .= ", но вначале надо <a href=\"$PHP_SELF?sid=$sid&copen=1\">открыть ворота</a>";
        }
        if ($ctext == 1) {
            $stmp .= "<br/><input name=\"ctext\" type=\"text\" value=\"$to\"/>\n<br/><select name=\"translit\" multiple=\"true\" value=\"1\"><option value=\"1\">Транслит</option></select><anchor>Сохранить<go href=\"$PHP_SELF?sid=$sid\" method=\"post\"><postfield name=\"ctext\" value=\"$(ctext)\"/><postfield name=\"translit\" value=\"$(translit)\"/></go></anchor>";
        }
        else {
            if ($ctext) {
                if ($translit == 1) {
                    include "f_translit.inc";
                    $ctext = trans($ctext);
                }
                $ctext = preg_replace('/([^ ,!.?А-я#])|\$|&/e', "", $ctext);
                $ctext = str_replace("#", "", $ctext);
                $ctext = str_replace("{", "", $ctext);
                $ctext = str_replace("}", "", $ctext);
                $d[0] = str_replace("(" . $ttext . ")", "(" . $ctext . ")", $d[0]);
                $loc_tt[$gate]["d"] = implode("|", $d);
                $loc_tt[$gate]["save"] = 1;
            }
            else {
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&ctext=1\">[Вывеска]</a>";
            }
        }
    }
    if ($char[8] && $clanc != $clan && strpos($d[0], ":" . $login . ":") === false) {
        $stmp .= ", вы призрак, поэтому не можете войти внутрь";
    }
    else {
        if (strpos($d[0], ":" . $login . ":") !== false) {
            $stmp .= ", вы признаны гостем, можете войти";
        }
        if ($clanc != $clan && strpos($d[0], ":" . $login . ":") === false) {
            $stmp .= "<br/>Если войдете, это будет нападением на замок";
        }
        if ($clanc != $clan) {
            $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&stuk=1\">Постучать в ворота</a>";
        }
    }
    if ($ttext) {
        $stmp .= "<br/>Надпись на воротах: " . $ttext;
    }

}
else {
    $stmp .= "<br/>Замок никому не принадлежит, можете войти";
} 