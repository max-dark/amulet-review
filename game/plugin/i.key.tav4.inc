<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global game
 * @global char
 */

// открывает и запирает дверь в комнате в таверне, изменяя файл с выходами loc_d/x1095x532
// записи о посетителях хранятся в x1098x542 у n.Frederik в tav=user:time|user:time|user:time|user:time
// к закрытым дверям приписываем [закрыто] и ссылку заменяем на самого себя

loadloc("x1098x542");
if ($loc != "x1095x532" && $loc != "x1087x528") {
    msg("Вначале надо подняться на 2 этаж таверны");
}
if ( ! isset($loc_i["x1098x542"]["n.Frederik"])) {
    msg("Бармена нет на месте, пока он не появится, вы не можете использовать ключ");
}

if (strpos($loc_i["x1098x542"]["n.Frederik"]["tav"], $login . ":") !== false) {
    // загружаем статус дверей
    $tc = [0, 0, 0, 0];    // какие закрыты
    if (strpos($loc_tt["x1095x532"]["d"], "x1087x543") === false) {
        $tc[0] = 1;
    }
    if (strpos($loc_tt["x1095x532"]["d"], "x1080x539") === false) {
        $tc[1] = 1;
    }
    if (strpos($loc_tt["x1095x532"]["d"], "x1080x532") === false) {
        $tc[2] = 1;
    }
    if (strpos($loc_tt["x1095x532"]["d"], "x1087x528") === false) {
        $tc[3] = 1;
    }

    // проверим, не вышло ли время
    $t   = intval(preg_replace('/.*' . $login . ':(\d+).*/', "\\1", $loc_i["x1098x542"]["n.Frederik"]["tav"]));
    $tav = explode("|", $loc_i["x1098x542"]["n.Frederik"]["tav"]);
    $tr  = explode(":", $tav[3]);
    if (time() < $t) {
        if ($tr[0] != $login) {
            msg("Это не ваша комната");
        }
        $tc[3] = ! $tc[3];
        if ( ! $tc[3]) {
            addjournal($loc, $login, "Вы открыли дверь в комнату 4");
            addjournal($loc, "all", $char[0] . " открыл дверь в комнату 4", $login);
        } else {
            addjournal($loc, $login, "Вы заперли дверь в комнату 4");
            addjournal($loc, "all", $char[0] . " запер дверь в комнату 4", $login);
            if ($game["floc"] == $loc) {
                unset($loc_i[$loc]["i.flag"]);
                $loc_i["x1095x532"]["i.flag"] = "флаг лидерства|1|0";
                $game["floc"]                 = "x1095x532";
                $tl                           = $game["fid"];
                $game["fid"]                  = "";
                if (isset($loc_i[$loc][$tl])) {
                    calcparam($loc, $tl);
                    if ($tl == $login) {
                        $char = explode("|", $loc_i[$loc][$login]["char"]);
                    }
                }
            }
        }

    } else {    // открываем дверь, удаляем юзера
        $tc[3]                                   = 0;
        $tav[3]                                  = "";
        $loc_i["x1098x542"]["n.Frederik"]["tav"] = implode("|", $tav);
        addjournal($loc, $login, "Срок аренды кончился");
    }

    // сохраняем новый файл
    $new = "Таверна, 2 этаж|1|спуститься по южной лестнице|x1098x542|спуститься по северной лестнице|x1098x530";

    $new .= "|дверь в комнату 1";
    if ($tc[0]) {
        $new .= " [закрыто]|x1095x532";
    } else {
        $new .= "|x1087x543";
    }
    $new .= "|дверь в комнату 2";
    if ($tc[1]) {
        $new .= " [закрыто]|x1095x532";
    } else {
        $new .= "|x1080x539";
    }
    $new .= "|дверь в комнату 3";
    if ($tc[2]) {
        $new .= " [закрыто]|x1095x532";
    } else {
        $new .= "|x1080x532";
    }
    $new .= "|дверь в комнату 4";
    if ($tc[3]) {
        $new .= " [закрыто]|x1095x532";
    } else {
        $new .= "|x1087x528";
    }

    $loc_tt["x1095x532"]["d"]    = $new;
    $loc_c                       = explode("|", $loc_tt[$loc]["d"]);
    $loc_tt["x1095x532"]["save"] = 1;

} else {
    addjournal($loc, $login, "Ключ не подходит ни к одной двери");
}

