<?php
// добавляем 1 стража
srand((float)microtime() * 10000000);
for ($k = 0; $k < 1; $k++) {
    $id = "n.g." . rand(5, 9999);
    $title = rndname() . " [стража]";
    $loc_i[$i][$id] = array("char" => $title . "|1000|1000|100|100|" . time() . "1||||||" . (time() + 600), "war" => "100|100|100|2|0|10|20|0|0|10|30|40|алебардой|0||", "items" => "i.w.t.alebarda:1", "equip" => "i.w.t.alebarda");
    addjournal($i, "all", "Появился " . $title);
}