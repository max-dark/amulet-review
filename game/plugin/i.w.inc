<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */
$equip = $loc_i[$loc][$login]["equip"];
// если в руках нож и есть $to, то освежевание
if ($to && substr($use, 0, 6) == 'i.w.k.') {
    include __DIR__ . DIRECTORY_SEPARATOR . "i.w.k.inc";
} else
{
    if ($to && substr($use, 0, 6) == 'i.w.t.') {
        include __DIR__ . DIRECTORY_SEPARATOR . "i.w.t.inc"; // рубка деревьев
    } else {
        $ti = getBaseItemId($use);
        if (strpos($equip, $ti) === false) {    // одеваем
            // оружие
            if (substr($use, 0, 4) == 'i.w.') {
                if (strpos($equip, "i.w.") === false) {
                    if ( ! $equip) {
                        $equip = $use;
                    } else {
                        if ($equip{strlen($equip) - 1} == "|") {
                            $equip .= $use;
                        } else {
                            $equip .= "|" . $use;
                        }
                    }
                } else {
                    $equip = preg_replace('/i\.w\.[^|]+/', $use, $equip);
                }
            }
            // броня
            if (substr($use, 0, 4) == 'i.a.') {
                if (strpos($equip, substr($use, 0, 6)) === false) {
                    if ( ! $equip) {
                        $equip = $use;
                    } else {
                        if ($equip{strlen($equip) - 1} == "|") {
                            $equip .= $use;
                        } else {
                            $equip .= "|" . $use;
                        }
                    }
                } else {
                    $equip = preg_replace('/' . substr($use, 0, 6) . '[^|]+/', $use, $equip);
                }
            }
        } else {
            $equip = preg_replace('/' . $ti . '[^|]*\|?/', "", $equip);
        }
        $loc_i[$loc][$login]["equip"] = $equip;
        calcparam($loc, $login);
        if ( ! $list) {
            $list = 'inv';
        }    // только если не взяли нож
    }
}