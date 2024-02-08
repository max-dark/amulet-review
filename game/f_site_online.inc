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
 */

use MaxDark\Amulet\OldCode\PageType;

if (time() > $game["lastai"] + 240) {
    // обновим список
    include "f_online.inc";
}
$stmp = "";
if (!$start) {
    $start = 0;
}
$i = -1;
$listSize = 10;
$listEnd = $start + $listSize;

$dh = opendir("online/");
while (($fname = readdir($dh)) !== false) {
    if ($fname != "." && $fname != ".." && $fname != "1.htaccess" && $fname != ".htaccess") {
        $i++;
        if ($i >= $start && $i < $listEnd) {
            $tmp = file("online/" . $fname);
            if ($tmp[0] == ".") {
                continue;
            }
            for ($it = 0; $it < 2; $it++) {
                $tmp[$it] = str_replace("\n", "", $tmp[$it]);
                $tmp[$it] = str_replace("\r", "", $tmp[$it]);
            }
            if (!$tmp[0]) {
                @unlink("online/" . $fname);
                $i--;
                continue;
            }
            loadloc($tmp[0]);
            if (isset($loc_i[$tmp[0]][$fname])) {
                $tmp1 = explode("|", $loc_i[$tmp[0]][$fname]["char"]);
                $stmp .= "\n<br/>" . $tmp1[0];
            } else {
                $i--;
                @unlink("online/" . $fname);
            }
        }
    }
}
closedir($dh);

if ($i > -1) {
    $stmp = "<p>Сейчас в игре " . ($i + 1) . " (на этом сервере разрешено не более $g_max) игроков:" . $stmp;
}
if ($i > -1) {

    if ($start) {
        $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&site=online\">^ </a>";
    }
    if ($listEnd < $i + 1) {
        if (!$start) {
            $stmp .= "\n<br/>";
        }
        $stmp .= "<a href=\"$PHP_SELF?sid=$sid&site=online&start=" . $listEnd . "\">+ (" .
            ($i + 1 + 1 - $listEnd) . ")</a>";
    }
} else {
    $stmp = "<p>Сейчас в игре никого нет";
}

$stmp .= "\n<br/><br/><button class=\"btn btn-outline-secondary m-1\" name=\"b2\" type=\"accept\" label=\"Назад\" onclick=\"history.back()\"><prev>Назад</prev></button>";

msg($stmp, "Онлайн", 0, PageType::GUEST);