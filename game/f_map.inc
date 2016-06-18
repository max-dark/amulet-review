<?php

// для вывода картинки использует m.php!!! (чтобы лишний раз не обращаться к игре)

require_once 'modules/map.php';

$char_coord = calculateCoordinates($l);
$flag_coord = calculateCoordinates($f);


$description = array(
    "основной территории.",
    "территории Ансалона.",
    "Волчьем острове."
);

srand((float)microtime() * 10000000);
$stmp = '<p align="center">' .
    "<img alt=\"map\" src=\"m.php?l=$loc&amp;f=" . $game["floc"] . "&amp;img=1&amp;r=" . rand(99, 999) . "&amp;bw=$map\"/>" .
    "<br/><a href=\"$PHP_SELF?sid=$sid\">В игру</a></p>";
$stmp .= "<p>Вы на " . $description[$char_coord[0]];
if (!$game["floc"]) {
    $fc = "неизвестно у кого (попробуйте обновить карту) ";
}
else {
    if ($game["fid"]) {
        $fc = "у " . $game["fchar"] . " ";
    }
    else {
        $fc = "сейчас никому не принадлежит ";
    }
}
$stmp .= "<br/>Флаг лидерства [<a href=\"m.php?flag=1\">?</a>] " . $fc;
if ($flag_coord[0] != $char_coord[0]) {
    $stmp .= "на " . $description[$flag_coord[0]];
}

$stmp .= "<br/><a href=\"m.php?info=1\">Помощь</a>";

msg($stmp);
