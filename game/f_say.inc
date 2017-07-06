<?php

/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global to
 * @global locai
 */
// сказать текст всем
if (strpos($say, "?sid=") !== false) {
    $say = substr($say, 0, strpos($say, "?sid="));
}
$s = substr($to, 2) . ' ' . $say;

$ok = GetBadWord($s);
if ($ok) {
    $s = substr($to, 2) . ' [БИП]';
}
$s   = str_replace('\\', "", $s);
$s   = str_replace('"', "", $s);
$s   = str_replace("'", "", $s);
$s   = str_replace("|", "", $s);
$s   = str_replace("<", "", $s);
$s   = str_replace(">", "", $s);
$s   = str_replace("$", "", $s);
$s   = str_replace("&", "", $s);
$say = substr($s, 0, 250);

$loc_i[$loc][$login]["macrol"] = "say|$say||";
// добавим всем, включая себя, в журнал что сказали
$b = 0;
if ($char[8]) {
    $ts = explode("|", $loc_i[$loc][$login]["skills"]);
    if (rand(0, 100) <= $ts[21] * 10) {
        $b = 1;
    }
}
if ( ! $char[8]) {
    addjournal($loc, "all", $char[0] . " говорит: " . $say);
} else {
    foreach (array_keys($loc_i[$loc]) as $i) {
        if (substr($i, 0, 2) == 'u.') {
            $skills = explode("|", $loc_i[$loc][$i]["skills"]);
            if ($b || rand(0, 100) <= $skills[21] * 20) {
                addjournal($loc, $i, $char[0] . " говорит: " . $say);
            } else {
                $stmp = preg_replace("/[A-ZА-ЯЁ]/u", "O", $say);
                $stmp = preg_replace("/[a-zа-яё]/u", "o", $stmp);
                addjournal($loc, $i, $char[0] . " говорит: " . $stmp);
            }
        }
    }
}
