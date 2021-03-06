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
 * @global id
 * @global locai
 * @global pass
 */

use MaxDark\Amulet\OldCode\PageType;

if ($nn) {
    $login = $nn;
}
if ($pass) {
    $p = $pass;
}
if (substr($login, 0, 2) != "u.") {
    $login = "u." . $login;
}
$login = strtolower($login);

// вначале ищем в списке онлайн
if (file_exists("online/" . $login) && filesize("online/" . $login) != 1) {
    $data   = "";
    $tmp    = file("online/" . $login);
    $tmp[0] = trim($tmp[0]);
    loadloc($tmp[0]);
    if (isset($loc_i[$tmp[0]][$login])) {
        if ($loc_i[$tmp[0]][$login]["user"] && $loc_i[$tmp[0]][$login]["char"] && $loc_i[$tmp[0]][$login]["skills"] &&
            $loc_i[$tmp[0]][$login]["war"]
        ) {
            $auser  = $loc_i[$tmp[0]][$login];
            $loc    = $tmp[0];
            $char   = explode("|", $loc_i[$loc][$login]["char"]);
            $tuser  = explode("|", $loc_i[$loc][$login]["user"]);
            $tp     = $tuser[0];
            $tfound = 1; // уже онлайн
        } else {
            unset($loc_i[$tmp[0]][$login]);
            @unlink("online/" . $login);
            $tfound = 0;
        }
    } else {
        @unlink("online/" . $login);
        $tfound = 0;
    }
} else {
    $tfound = 0;
}

// если нет, то в оффлайн
if ( ! file_exists("online/" . $login) || ( ! $tfound && ! $data)) {
    msg("Нажмите на ссылку ниже<br/><a href=\"?site=connect&amp;login=$login&p=$p&f_c=$f_c\">Продолжить</a>",
        $GAME_NAME, 0, PageType::GUEST);
}
if ( ! $tfound) {

    $data = str_replace("\\", "", $data);

    if (preg_match_all('/(i\.[^fh]\.[^:|]*:\d{4,})/', $data, $arr)) {
        $data = str_replace($arr[0], "", $data);
    }

    $auser = unserialize($data);
    if ( ! $auser["char"] || ! $auser["skills"]) {
        $data  = preg_replace('/s:(?:\d+):"(.*?)";/e', "calcser('\\1')", $data);
        $auser = unserialize($data);
    }
    if ( ! $auser["char"] || ! $auser["skills"]) {
        msg("Неверный формат, обновите страницу", $GAME_NAME, 0, PageType::GUEST);
    }
    $auser["srv"] = time();
    $tuser        = explode("|", $auser["user"]);
    $tp           = $tuser[0];
    if ($tp != $p) {
        $tuser[0]      = $p;
        $auser["user"] = implode("|", $tuser);
        $tp            = $p;
    }
    $tfound = 2;
}

if ($p != $tp) {
    include_once("f_npass.inc");
}

if ($tfound == 2) {
    $loc = $auser["loc"];
    if ($loc == "arena") {
        $loc = "x1229x582";
    }
    if (substr($loc, 0, 2) == "c.") {
        $loc = substr($loc, 0, 4) . "in";
    }
    // если последняя лока была одной из комнат в таверне
    if (in_array($loc, [
            "x1087x543",
            "x1080x539",
            "x1087x528",
            "x1080x532"
        ])) {
        // выйти в коридор
        $loc = "x1095x532";
    }
    $auser["journal"] = "";
    $auser["magic"]   = str_replace("m.n", "m.roj", $auser["magic"]);
    $auser["items"]   = str_replace("i.ms_n.a.b.jarpt", "xren_vam", $auser["items"]);
    $char             = explode("|", $auser["char"]);
    if (count($char) < 16) {
        for ($i = count($char); $i < 16; $i++) {
            $char[$i] = "";
        }
    }
    $tuser    = explode("|", $auser["user"]);
    $tuser[3] = intval($tuser[3]); //Возраст
    if ($tuser[3] == 0) {
        $tuser[3] = 16;
    }
    $auser["user"] = implode("|", $tuser);
    $char[5]       = time(); // таймер регенерации
    $char[6]       = time() - 1; // таймер атаки
    $char[11]      = time(); // таймер последнего обновления
    $char[15]      = time() - 10; // FIXME: использования не найдено
    $char[16]      = time() - 10; // FIXME: использования не найдено(возможно лишняя "Copy-Paste")
    if ($char[9]) {
        $char[10] = time() + $char[10] - $auser["time"]; //time_crim
    }
    unset($auser["time"]);
    unset($auser["loc"]);
    unset($auser["def"]);
    unset($auser["ua"]);
    $auser["char"] = implode("|", $char);
    loadloc($loc);
    $loc_i[$loc][$login] = $auser;
    // FIXME: добавим событие появления в журнал(костыль)
    manageNPC($login, "", $loc);
}

$sid = substr($login, 2) . "." . $p . "." . chr(rand(97, 122));

// проверим клан
/// FIXME: PHP Notice:  Undefined variable: clan
if ($clan == "no") {
    addjournal($loc, $login, "Вы изгнаны из клана, поговорите с Мирандером");
    $char[0]                     = preg_replace("/ \*.*?\*/", "", $char[0]);
    $loc_i[$loc][$login]["char"] = implode("|", $char);
}
if ($char[13]) {
    $ts = explode(":", $char[13]);
    $ts = $ts[0];
    if (file_exists("online/" . $ts) && filesize("online/" . $ts) != 1) {
        $tus = file("online/" . $ts);
        $tus = trim($tus[0]);
        loadloc($tus);
        $tus = explode("|", $loc_i[$tus][$ts]["char"]);
        if (substr($tus[13], 0, strlen($login)) != $login) {
            $char[13]                    = "";
            $loc_i[$loc][$login]["char"] = implode("|", $char);
            addjournal($loc, $login, substr($ts, 2) . " развелся(ась) с вами!");
        }
    }
}

/// FIXME: PHP Notice:  Undefined variable: tnews
msg("Добро пожаловать, " . $char[0] . "!
      <br/><a href=\"?site=news\">Новости </a>[" . $tnews . "]
      <br/><a href=\"?sid=$sid&cnick=1\">Настройки</a>
        <br/>Чтобы не вводить логин и пароль, сделайте в телефоне закладку на эту страницу.
        <br/>Для выхода используйте пункт меню \"Сохранить\", чтобы не потерять накопленные вещи и опыт.
        <br/><a href=\"?sid=$sid\">В игру</a>", $GAME_NAME, 0, PageType::GUEST);
