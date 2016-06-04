<?php
$tmp = explode("|", $loc_t[$i][$j]);
$item = explode("|", implode("", file("items/" . $tmp[0])));
$tc = rand($tmp[1], $tmp[2]);
if ($tc > 0) $loc_i[$i][$tmp[0]] = $item[0] . "|" . $tc . "|0"; else unset($loc_i[$i][$tmp[0]]);
addtimer($i, $j, rand($tmp[3], $tmp[4]), $loc_t[$i][$j], 1);
