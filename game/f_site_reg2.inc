<?php

use MaxDark\Amulet\database\DB;

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
 * @global id
 * @global pass
 * @global email
 * @global age
 * @global sex
 */

$nn = Request('nn');
$pass = Request('pass');
$sex = Request('sex');
$age = Request('age');
$email = Request('email');

if (strlen($nn) < 6 || strlen($pass) < 6 || strlen($nn) > 15 || strlen($pass) > 15) {
    msg("Длина логина и пароля должна быть не короче 6 и не длинее 15 символов.<br /><anchor>Назад<prev/></anchor>",
        'Ошибка', 0, 'none');
}
if ( ! preg_match('|^[-0-9a-z_]+$|i', $nn)) {
    msg("Неверный синтаксис в логине<br /><anchor>Назад<prev/></anchor>", 'Ошибка', 0, 'none');
}
if ( ! preg_match("|^[-0-9a-z_\\.]+@[-0-9a-z]+\\.[a-z]{2,6}+$|i", $email)) {
    msg("Неверный синтаксис в email " . $email . "<br /><anchor>Назад<prev/></anchor>", 'Ошибка', 0, 'none');
}
if (intval($age) != $age || $age < 5 || $age > 99) {
    msg("Неправдоподобный возраст, укажите более реальный.<br /><anchor>Назад<prev/></anchor>", "Ошибка", 0, 'none');
}
if ($sex != 'm' && $sex != 'f') {
    msg("Вы пришли играть или дыры искать?" . $sex . "<br /><anchor>Назад<prev/></anchor>", 'Ошибка', 0, 'none');
}

$now = time();
openDB();
$sqlLogin = 'INSERT INTO `users` (regtime,nick,pass,email) VALUES (?, ?, ?, ?)';
DB::link()->prepare($sqlLogin)->execute([
        $now,
        $nn,
        $pass,
        $email
    ]);
$loc = "_begin"; // начальная локация

// создаем игрока
$auser = [
    "user"    => "$pass||$sex|$age||$now|0|0|0|0",
    "char"    => "$nn|20|20|20|20|1|1||0|0|0||",
    "skills"  => "1|1|1|0|2|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0",
    "loc"     => $loc,
    "journal" => "",
    "items"   => "i.w.k.begin:1",
    "magic"   => "",
];
$login = 'u.' . $nn;
// добавим в локацию, чтобы посчитать параметры
loadloc($loc);
$loc_i[$loc][$login] = $auser;
calcparam($loc, $login);

// сохраним...
$auser = $loc_i[$loc][$login];
unset($loc_i[$loc][$login]);
if ($auser["user"] && $auser["char"] && $auser["skills"] && $auser["war"]) {
    $res = SetData($nn, $pass, serialize($auser));
} else {
    $res = "Неверный формат";
}
if ($res) {
    unset($loc_i[$loc][$login]);
    msg($res, "Ошибка", 0, 'none');
}

// общее число зарегистрированных
$fntmp = "data/all.dat";
if (file_exists($fntmp)) {
    $all = 0;
    $all = (@file_get_contents($fntmp));
    if ($all) {
        $all++;
        $file = fopen($fntmp, "w");
        if ($file !== false) {
            fputs($file, $all);
            fclose($file);
        }
    }
} else {
    $file = fopen($fntmp, "w");
    if ($file !== false) {
        fputs($file, "1");
        fclose($file);
    }
}

msg("Регистрация завершена<br/><a href=\"?site=connect&login=$nn&p=$pass\">Начать игру</a>", "Регистрация", 0, 'none');
