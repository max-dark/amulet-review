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
 * @global to
 * @global use
 * @global id
 */
// использование предметов

if ($use == "i.h") {
    if ( ! $char[12]) {
        msg("Вы не на коне");
    }
    $tc                          = explode(":", $char[12]);
    $char[12]                    = "";
    $loc_i[$loc][$login]["char"] = implode("|", $char);
    calcparam($loc, $login);
    $npc    = loadNpcById("n.a.losh");
    $tc1    = explode("|", $npc["char"]);
    $tc1[5] = time();
    if ($tc[1] < $tc1[1]) {
        $tc1[1] = $tc[1];
    }
    if (isset($loc_i[$loc][$login]["name"])) {
        $tc1[0] .= " " . $loc_i[$loc][$login]["name"];
        $npc["name"] = $loc_i[$loc][$login]["name"];
        unset($loc_i[$loc][$login]["name"]);
    }
    $npc["char"]                               = implode("|", $tc1);
    $npc["owner"]                              = $login . "|" . $login . "|0|" . (time() + 60 * 60) . "|1";
    $loc_i[$loc]["n.a.losh." . rand(99, 9999)] = $npc;
    addjournal($loc, "all", $char[0] . " спешился", $login);

    return;    // $char в index.php
    //msg("Вы спешились и держите коня за уздечку.");
}

if ($use{2} > 0 && $use{2} < 10) {
    $tm = explode("|", $loc_i[$loc][$login]["items"]);
    $tc = intval($use{2}) - 1;
    if ($tm[$tc]) {
        $use                           = substr($tm[$tc], 0, strpos($tm[$tc], ":"));
        $loc_i[$loc][$login]["macrol"] = "use|$use|to|$to";
    } else {
        msg("В вашем рюкзаке нет предмета под номером " . ($tc + 1));
    }
}
if (strpos($loc_i[$loc][$login]["items"], $use . ":") === false) {
    msg("<p>Использовать можно только те предметы, что у вас в рюкзаке");
}

$item_type = substr($use, 0, 4);
switch ($item_type) {
    case 'i.m.':
    case 'i.r.':
        $scroll = $use;
        $use = "m." . substr($use, 4);    // ниже исп. магию
        $m = findItemByFullId($use);
        if ($m[6] && !$to) {
            $use = $scroll;
            $list = 'all'; // чтобы вывести список целей
        } else {
            require "f_usemagic.inc";
        }
        break;
    case 'i.w.':
    case 'i.a.':
        include "plugin/i.w.inc";
        break;
    case "i.f.":
        include "plugin/i.f.inc";
        break;
    case "i.i.":
        include "plugin/i.i.inc";
        break;
    case "i.bc":
        include "plugin/i.bc.inc";
        break;
    case "i.ms":
        include "plugin/i.ms.inc";
        break;
    default:
        $inc_plugin = "plugin/" . $use . ".inc";
        if (file_exists($inc_plugin)) {
            include $inc_plugin;
        } else {
            addjournal($loc, $login, "Никакого эффекта");
        }
        break;
}
