<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */

/**
 * @param $s
 *
 * @return int
 */
function tsret($s)
{
    if ($s > 0) {
        return $s;
    } else {
        return 0;
    }
}

$war = explode("|", $loc_i[$loc][$login]["war"]);
if ($loss) {
    $war[0] = 0;
} else {
    $war[0] = 100;
}
$war[1]  = tsret($magic[4] - 10 + $skills[2] * 2);
$war[2]  = $magic[5] + $skills[2] * 2;
$war[3]  = $magic[8];
$war[4]  = 0;
$war[12] = "магией";
$war[14] = "";
$war     = implode("|", $war);

$arr = [];
if (substr($use, 0, 6) == "m.w.a.") {
    foreach (array_keys($loc_i[$loc]) as $to) {
        if (substr($to, 0, 2) != 'i.' && $to != $login) {
            $arr[$to] = "";
        }
    }
} else {
    if (isset($loc_i[$loc][$to]) && substr($to, 0, 2) != 'i.') {
        $arr[$to] = "";
    }
}

if (count($arr) > 0) {
    $keys = array_keys($arr);
    foreach ($keys as $to) {
        $tchar = explode("|", $loc_i[$loc][$to]["char"]);
        $tcrim = $tchar[9] || substr($to, 0, 4) == 'n.c.';
        if ($use == 'm.w.hit' && ! $loss) {
            $heal = rand($magic[4], $magic[5]);
            $tchar[3] -= $heal;
            if ($tchar[3] < 0) {
                $tchar[3] = 0;
            }
            $loc_i[$loc][$to]["char"] = implode("|", $tchar);
        }
        if ($use == 'm.w.vamp' && ! $tcrim) {
            attack($loc, $login, $to, $war, 0, 1);
        } else {
            if ( ! $magic[7] || $magic[7] && $tcrim) {
                attack($loc, $login, $to, $war, 0, 1);
            }
        }
    }

    if ($use == 'm.w.vamp' && ! $loss) {
        $tchar = explode("|", $loc_i[$loc][$login]["char"]);
        if ( ! $tchar[8]) {
            $heal = rand(2, $magic[4]);    // лечит
            if ($tchar[1] + $heal > $tchar[2]) {
                $heal = $tchar[2] - $tchar[1];
            }
            if ($heal > 0) {
                $tchar[1] += $heal;
                $loc_i[$loc][$login]["char"] = implode("|", $tchar);
                addjournal($loc, $login, "Жизнь +" . $heal);
            }
        }
    }
} else {
    addjournal($loc, $login, "Заклинание не эффективно");
}
