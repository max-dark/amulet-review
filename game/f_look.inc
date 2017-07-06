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

if ($look) {
    if ($look == 1) {
        $page_d = "1";
    } else {
        if (substr($look, 0, 2) == 'i.') {
            include "f_lookitem.inc";
        } else {
            if (substr($look, 0, 2) == 'u.') {
                include "f_lookuser.inc";
            } else {
                if (substr($look, 0, 2) == 'n.') {
                    include "f_looknpc.inc";
                } else {
                    if (substr($look, 0, 2) == 'm.') {
                        include "f_lookmagic.inc";
                    } else {
                        if (substr($look, 0, 2) == 'p.') {
                            if (file_exists("items/" . $look)) {
                                $item = explode("|", (file_get_contents("items/" . $look)));
                            } else {
                                die("err: no items/" . $look);
                            }
                            $skills = explode("|", $loc_i[$loc][$login]["skills"]);
                            $stmp   = "<a href=\"$PHP_SELF?sid=$sid&look=p\">Период</a>: " . $item[1] . "<br/>" .
                                      $item[2];
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
                        } else {
                            if (file_exists("desc/" . $look)) {
                                $stmp .= "<p>" . (file_get_contents("desc/" . $look));
                            } else {
                                $stmp .= "Описание недоступно";
                            }
                        }
                    }
                }
            }
        }
    }
    if ( ! $page_d && $stmp) {
        msg($stmp, "Описание");
    }
}
