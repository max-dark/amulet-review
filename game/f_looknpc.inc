<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global item
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global $look
 */
if (isset($loc_i[$loc][$look])) {
    $char = explode("|", $loc_i[$loc][$look]["char"]);    // для оптимизации
    $stmp = "<p>Тип: ";
    $npc_subtype = substr($look, 0, 4);
    switch ($npc_subtype) {
        case 'n.a.':
            $stmp .= "животное";
            break;
        case 'n.m.':
            $stmp .= "гном";
            break;
        case 'n.w.':
            $stmp .= "божественаая личность";
            break;
        case 'n.c.':
            $stmp .= "монстр";
            break;
        case 'n.z.':
            $stmp .= "зомби, поднятый из мертвых с помощью некромантии";
            break;
        case 'n.g.':
            $stmp .= "городская стража";
            break;
        case 'n.s.':
            $stmp .= "призван с помощью магии";
            break;
        case 'n.h.':
            $stmp .= "лекарь, может воскрешать мертвых (для этого надо поговорть с ним)";
            break;
        default:
            if (substr($look, 0, 9) == 'n.a.losh.') {
                $stmp .= "лошадь, на ней можно ездить, если она принадлежит вам (выберите в меню Говорить, Сесть в седло)";
            } else {
                $stmp .= "человек";
            }
            break;
    }
    if (file_exists("speak/" . $look)) {
        $stmp .= "<br/>Может с вами говорить";
    }
    if ($npc_subtype == 'n.a.' && $char[8]) {
        $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=char.tame\">Сложность приручения</a>: " . $char[8];
    }

    if (isset($loc_i[$loc][$look]["owner"])) {
        $owner = explode("|", $loc_i[$loc][$look]["owner"]);
        if (isset($loc_i[$loc][$owner[0]])) {
            $tmp = explode("|", $loc_i[$loc][$owner[0]]["char"]);
            $stmp .= "<br/>Принадлежит " . $tmp[0];
        } else {
            $stmp .= "<br/>Кому-то принадлежит, но хозяина рядом нет";
        }
    }

    // что одето
    if (isset($loc_i[$loc][$look]["equip"])) {
        $str   = "";
        $sto   = "";
        $equip = explode("|", $loc_i[$loc][$look]["equip"]);
        foreach ($equip as $i) {
            if ($i) {
                $item = findItemByFullId($i);
                if (substr($i, 0, 4) == "i.a.") {
                    if ( ! $sto) {
                        $sto = $item[0];
                    } else {
                        $sto .= ", " . $item[0];
                    }
                } else {
                    if ( ! $str) {
                        $str = $item[0];
                    } else {
                        $str .= ", " . $item[0];
                    }
                }
            }
        }
        if ($str) {
            $stmp .= "<br/>Держит в руках: " . $str;
        }
        if ($sto) {
            $stmp .= "<br/>Одет: " . $sto;
        }
    }

    msg($stmp, $char[0]);
} else {
    addjournal($loc, $login, "Не на кого смотреть");
}
