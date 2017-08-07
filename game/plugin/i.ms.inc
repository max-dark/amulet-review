<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global use
 * @global char
 */
// призыв монстра

manageItems($loc, $login, "", $use, 1, "items", "items", 1, 0, 1);    // удаляем свиток
$inp  = substr($use, 5);
$name = "";
if (strpos($inp, "_") !== false) {
    $name = substr($inp, strpos($inp, "_") + 1);
    $inp  = substr($inp, 0, strpos($inp, "_"));
}

if ( ! npcExists($inp)) {
    msg("Существо " . $inp . " не существует");
}
if (substr($inp, 0, 4) != "n.c." && substr($inp, 0, 4) != "n.a.") {
    msg("Призвать можно только монстров и животных");
}
$npc = loadNpcById($inp);
$tc  = explode("|", $npc["char"]);

if ($name) {
    include __DIR__ . DIRECTORY_SEPARATOR . "../f_translit.inc";
    $name = trans($name);
    if ( ! $name || $name == " ") {
        msg("Неверное имя");
    }
    $tc[0] = $name;
}
addjournal($loc, $login, "Вы призвали " . $tc[0]);
addjournal($loc, "all", $char[0] . " призвал " . $tc[0], $login);

$tc[0]            = "призванный " . $tc[0];
$npc["char"]      = implode("|", $tc);
$npc["owner"]     = $login . "|" . $login . "||0";
$id               = "n.a." . rand(99, 9999);
$loc_i[$loc][$id] = $npc;
manageNPC($id, "", $loc);    // журнал

