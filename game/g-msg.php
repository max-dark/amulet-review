<?php
require_once('config.php'); // настройки игры
require_once('datafunc.php'); // функции игры
require_once('game_function.php'); // игровые функции

$game = unserialize(file_get_contents('game.dat'));
msg('test');