<?php
// телепорт к жене/мужу

$char = explode("|", $loc_i[$loc][$login]["char"]);
if ($char[13]) {
    $tm = explode(":", $char[13]);
    if (file_exists("online/" . $tm[0]) && filesize("online/" . $tm[0]) != 1) {
        $tmf = file("online/" . $tm[0]);
        $tmf = trim($tmf[0]);
        if ($tmf != $loc) {
            loadloc($tmf);
            $tsc = explode("|", $loc_i[$tmf][$tm[0]]["char"]);
            $tsm = explode(":", $tsc[13]);
            if (time() < $tsm[1]) addnpc($login, $loc, $tmf); else addjournal($loc, $login, "Время для телепорта к мужу/жене истекло");
        }
    } else addjournal($loc, $login, "Ваш(а) муж(жена) покинул(а) игру");
}