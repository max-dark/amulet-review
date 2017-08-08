<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global to
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global $look
 */

use MaxDark\Amulet\OldCode\ViewOptions;

if ($look) {
    if ($look == 1) {
        ViewOptions::getInstance()->setDescEnabled(true);
    } else {
        switch (substr($look, 0, 2)) {
            case 'i.':
                include "f_lookitem.inc";
                break;
            case 'u.':
                include "f_lookuser.inc";
                break;
            case 'n.':
                include "f_looknpc.inc";
                break;
            case 'm.':
                include "f_lookmagic.inc";
                break;
            case 'p.':
                $item = findItemByBaseId($look);
                $skills = explode("|", $loc_i[$loc][$login]["skills"]);
                $stmp = "<a href=\"$PHP_SELF?sid=$sid&look=p\">Период</a>: " . $item[1] . "<br/>" . $item[2];
                if ($look != "p.d.c" && ($skills[2] - 1) * 10 > 0) {
                    $stmp .= "<br/>---<br/>Шанс успешного использования (с учетом вашего интеллекта): -" .
                        (($skills[2] - 1) * 10) . "%";
                }
                if ($char[12]) {
                    $stmp .= "<br/>Так как вы на коне: -10%";
                }
                if ($game["fid"] == $login && $game["floc"] == $loc) {
                    $stmp .= "<br/>Так как у вас флаг лидерства: +10%";
                }
                msg($stmp, $item[0]);
                break;
            default:
                if (file_exists("desc/" . $look)) {
                    $stmp .= "<p>" . (file_get_contents("desc/" . $look));
                } else {
                    $stmp .= "Описание недоступно";
                }
                break;
        }
    }
    if ( ! ViewOptions::getInstance()->getDescEnabled() && $stmp) {
        msg($stmp, "Описание");
    }
}
