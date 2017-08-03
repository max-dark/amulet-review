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
 * @global p
 * @global s
 */

use MaxDark\Amulet\OldCode\DBOperations;

if ( ! preg_match('|^[-0-9a-z_\.]+$|i', $login)) {
    msg("Неверный синтаксис в логине $login<br /><anchor>Назад<prev/></anchor>", 'Проверка входа', 0, 'none');
}
if (substr($login, 0, 2) != "u.") {
    $login = "u." . $login;
}
$login = strtolower($login);
$login = substr($login, 0, 15);
clearstatcache();
if (file_exists("online/" . $login) && filesize("online/" . $login) != 1) {
    if ($gm == $gm_id) {
        $p .= "&gm=" . $gm;
    }
    msg("Нажмите на ссылку ниже<br /><a href=\"?site=connect2&login=$login&p=$p\">Продолжить</a>");
}
$file = @fopen("online/" . $login, "w");
if ($file !== false) {
    fwrite($file, ".");
    fclose($file);
} else {
    msg("Ошибка создания файла");
}

// получаем данные с основного сервера
list($res, $data) = DBOperations::getData(substr($login, 2), $p);
if ($res) {
    msg($res, 'Проверка входа', 0, 'none');
}
$auser = unserialize($data);
if ( ! $auser["char"] || ! $auser["skills"]) {
    $data  = preg_replace('/s:(?:\d+):"(.*?)";/e', "calcser('\\1')", $data);
    $auser = unserialize($data);
}

// проверим клан
if (strpos($auser["char"], "*") === false) {
    $clan = "";
} else {
    $clan = substr($auser["char"], strpos($auser["char"], "*") + 1,
        strrpos($auser["char"], "*") - strpos($auser["char"], "*") - 1);
}
if ($clan && (check_clan($clan, $login)) == "no") {
    $clan = "no";
} else {
    $clan = "yes";
}
// ок, получили данные, пробуем войти в игру
$t     = filemtime('../news/news.htm');
$tnews = date("d/m/Y", $t);
include_once('f_site_connect2.inc');
if (filesize("online/" . $login) == 1 || strpos($s, "cnick=1") === false) {
    @unlink("online/" . $login);
}
exit;

/**
 * проверяет, состоит ли $login в $clan
 *
 * @param string $clan
 * @param string $login
 *
 * @return string
 */
function check_clan($clan, $login)
{
    $dh = opendir("data/clans/");
    while (($fname = readdir($dh)) !== false) {
        if ($fname != "." && $fname != ".." && $fname != "1.htaccess" && $fname != ".htaccess") {
            if (strtolower($fname) === strtolower($clan)) {
                closedir($dh);
                $tmp = @unserialize((@file_get_contents("data/clans/" . $fname)));
                if (is_array($tmp) && ! isset($tmp["m"][$login]) && ! isset($tmp["g"][$login])) {
                    return ("no");
                }

                return ("yes");
            }
        }
    }
    closedir($dh);

    return ("no");
}