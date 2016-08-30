<?php
/**
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global j
 * @global locai
 * @global game
 * @global char
 */
// преследуем цель

$tfound = 0;
for ($k = 3; $k < count($locai); $k += 2) {
    if (isset($loc_i[$locai[$k]][$char[7]])) {
        $tfound = 1;
        $loc1 = explode("|", $loc_tt[$locai[$k]]["d"]);
        if ($locai[1] == $loc1[1] || substr($j, 0, 4) == "n.g.") {
            $b = 1;
        }
        // hiding от гардов не действует
        if (substr($char[7], 0, 2) == 'u.' && substr($j, 0, 4) != "n.g.") {
            $skills = explode("|", $loc_i[$locai[$k]][$char[7]]["skills"]);
            if (rand(0, 100) <= ($skills[17] * 4 + $skills[1])) {
                $b = 0;
                addjournal($locai[$k], $char[7], "Вы скрылись от погони");
            }
        }
        // призраков не преследуем
        if (substr($char[7], 0, 2) == "u.") {
            $tc = explode("|", $loc_i[$locai[$k]][$char[7]]["char"]);
            if ($tc[8]) {
                $char[7] = "";
                $loc_i[$i][$j]["char"] = implode("|", $char);
                $b = 0;
            }
        }

        if ($b) {    // погоня
            $loc_i[$i][$j]["char"] = implode("|", $char);
            addnpc($j, $i, $locai[$k]);
        }
        else {
            $tfound = 0;
        }
        break;
    }
}
if (!$tfound) {
    $char[7] = "";
    $loc_i[$i][$j]["char"] = implode("|", $char);
}
