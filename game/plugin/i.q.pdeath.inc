<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global game
 * @global char
 */
// урон всем вокруг по 150

$war     = explode("|", $loc_i[$loc][$login]["war"]);
$war[0]  = 100;
$war[1]  = 0;
$war[2]  = 90;
$war[3]  = 10;
$war[4]  = 0;
$war[12] = "порошком смерти";
$war[14] = "";
$war     = implode("|", $war);

$char[6]                     = time() + 10;
$loc_i[$loc][$login]["char"] = implode("|", $char);

manageItems($loc, $login, "", "i.q.pdeath", 1, "items", "items", 1, 0, 1);
if ($loc == "x2375x934") {
    addjournal($loc, "all", "[громовой голос] КАК ТЫ СМЕЕШЬ МЕНЯ ТРЕВОЖИТЬ, " . strtoupper(substr($login, 2)) . "?");
}

foreach (array_keys($loc_i[$loc]) as $to) {
    if (substr($to, 0, 2) != 'i.' && $to != $login) {
        attack($loc, $login, $to, $war, 0, 1);
    }
}
