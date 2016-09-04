<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 */
// ранит всех плохих вокруг

$war     = explode("|", $loc_i[$loc][$login]["war"]);
$war[0]  = 100;
$war[1]  = 6;
$war[2]  = 24;
$war[3]  = 4;
$war[4]  = 0;
$war[12] = "святой водой";
$war[14] = "";
$war     = implode("|", $war);

$char[6]                     = time() + 5;
$loc_i[$loc][$login]["char"] = implode("|", $char);

additem($loc, $login, "", "i.b.holy", 1, "items", "items", 1, 0, 1);

foreach (array_keys($loc_i[$loc]) as $to) {
    if (substr($to, 0, 2) != 'i.' && $to != $login) {
        $tchar = explode("|", $loc_i[$loc][$to]["char"]);
        if ($tchar[9] || substr($to, 0, 4) == 'n.c.') {
            attack($loc, $login, $to, $war, 0, 1);
        }
    }
}
