<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global to
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global id
 * @global skills
 * @global i
 * @global j
 */
//  загружаем в $npc из папки npc	id|resp_min:resp_max|move_num:time_min:time_max
if (is_array($loc_t[$i][$j])) {
    $npc = $loc_t[$i][$j];
    $tid = $npc["id"];
    unset($npc["id"]);
}
else {
    $ta = explode("|", $loc_t[$i][$j]);
    $tid = $ta[0];
    if (in_array(substr($tid, 0, 4), ["n.c.", "n.a."])) {
        $tid = substr($tid, 0, strrpos($tid, "."));
    }
    if (!file_exists("npc/" . $tid)) {
        unset($loc_t[$i][$j]);

        return;//("err: no npc/".$tid);
    }
    $npc = include "npc/" . $tid;
    $tid = $ta[0];
    $twar = explode("|", $npc["war"]);
    $twar[15] = $i . ":" . $ta[1];
    $npc["war"] = implode("|", $twar);
    if ($ta[2]) {
        $tchar = explode("|", $npc["char"]);
        $tchar[10] = $ta[2];
        $npc["char"] = implode("|", $tchar);
    }
}

// случ. предметы
if (isset($npc["itemsrnd"])) {
    // itemsrnd = item_type:chance:min_count:max_count
    $irnd = explode("|", $npc["itemsrnd"]);
    foreach (array_keys($irnd) as $k) {
        if ($irnd[$k]) {
            $trnd = explode(":", $irnd[$k]);
            $trndc = round(rand($trnd[2], $trnd[3]));
            if (rand(0, 100) <= $trnd[1] && $trndc > 0) {
                if ($npc["items"]) {
                    $npc["items"] .= "|" . $trnd[0] . ":" . $trndc;
                }
                else {
                    $npc["items"] = $trnd[0] . ":" . $trndc;
                }
            }
        }
    }
    unset($npc["itemsrnd"]);
}

// респавн текущий
$loc_i[$i][$tid] = $npc;
unset($loc_t[$i][$j]);