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
 * @global id
 * @global msg
 * @global i
 * @global j
 * @global steps
 * @global locai
 */
// случайное движение
if ($char[10]) {
    $move = explode(":", $char[10]);
}
if ($char[12]) {
    $steps = explode(":", $char[12]);
}
$b = 0;
if ( ! $char[10] && $char[12]) {
    $loc_i[$i][$j]["char"] = implode("|", $char);
    $lt                    = $steps[count($steps) - 1];
    $blt                   = 1;
    if ($j == "n.a.b.jarpt.1") {
        loadloc($lt);
        if (count($loc_i[$lt]) != 0) {
            $blt = 0;
        }
    }
    if ($blt) {
        addnpc($j, $i, $lt);
        $b = 1;
    }
} else {
    /// FIXME: PHP Notice:  Undefined offset: 3
    if (time() > $move[3]) {
        if ($char[12] && count($steps) >= $move[0]) {
            $b = 1;
            $k = $steps[count($steps) - 1];
        } else {
            $b = 0;
            $k = $locai[2 + 2 * rand(0, (count($locai) - 2) / 2 - 1) + 1];
        }
        if ( ! isset($loc_tt[$k]["d"])) {
            loadloc($k);
        }
        $loc1 = explode("|", $loc_tt[$k]["d"]);
        if ($locai[1] == $loc1[1] || $b) {
            $move[3]               = time() + rand($move[1], $move[2]);    // след. ход
            $char[10]              = implode(":", $move);
            $loc_i[$i][$j]["char"] = implode("|", $char);
            $blt                   = 1;
            if ($j == "n.a.b.jarpt.1") {
                loadloc($k);
                if (count($loc_i[$k]) != 0) {
                    $blt = 0;
                }
            }
            if ($blt) {
                addnpc($j, $i, $k);
                $b = 1;
            }
        }
    }
}