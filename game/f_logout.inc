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
 * @global g_list
 */

use MaxDark\Amulet\OldCode\DBOperations;

if ($login != $g_admin && $gm != $gm_id && $loc_i[$loc][$login]["srv"] > time() - 120) {
    $logout_msg = [
        'text'         => ("Вы только что вошли в игру, персонаж может покинуть этот сервер не раньше, чем через " .
                           round(($loc_i[$loc][$login]["srv"] - time() + 120) / 60 + 1) .
                           " минуты<br/><a href=\"$PHP_SELF?sid=$sid\">В игру</a>"),
        'title'        => 'Уже уходите?(',
        'show_journal' => 1
    ];
} else { // если никого рядом нет, то можем удалить персонажа
    $b = 0;
    foreach (array_keys($loc_i[$loc]) as $j) {
        if ($j != $login) {
            if ((substr($j, 0, 2) == 'u.') || substr($j, 0, 2) == 'n.') {
                $b = 1;
                break;
            }
        }
    }
    if ( ! $b) {
        for ($i = 2; $i < count($loc_c); $i += 2) {
            if ($loc_c[$i + 1] != $loc) {
                if (count($loc_i[$loc_c[$i + 1]]) > 0) {
                    $keys = array_keys($loc_i[$loc_c[$i + 1]]);
                    foreach ($keys as $j) {
                        if ((substr($j, 0, 2) == 'u.') || substr($j, 0, 2) == 'n.') {
                            $b = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (isset($loc_i[$loc][$login])) {
        if ($game["floc"] == $loc && $game["fid"] == $login) {
            addjournal($loc, "all", $char[0] . " бросил флаг!", $login);
            if ($loc == "x1087x543" || $loc == "x1080x539" || $loc == "x1080x532" || $loc == "x1087x528") {
                $last_loc = "x1095x532";
            } else {
                $last_loc = $loc;
            }
            $loc_i[$last_loc]["i.flag"] = "флаг лидерства|1|0";
            $game["floc"]               = $last_loc;
            $game["fid"]                = "";
            calcparam($loc, $login);
        }
        $auser            = $loc_i[$loc][$login];
        $auser["journal"] = "";
        $auser["loc"]     = $loc;
        $auser["time"]    = time();
        unset($auser["msgt"]);
        unset($auser["macrol"]);
        unset($auser["trade"]);
        $tp = explode("|", $auser["user"]);
        if ($auser["user"] && $auser["char"] && $auser["skills"] && $auser["war"]) {
            $error_message = DBOperations::setData(substr($login, 2), $tp[0], serialize($auser));
        } else {
            $error_message = "Неверный формат.";
        }
    } else {
        $error_message = "Нет данных";
    }

    if ($error_message) {
        $logout_msg = [
            'text'         => ("<p>Ошибка при сохранении персонажа: " . $error_message .
                               "<br/>Рекомендуем вам оставаться онлайн и регулярно пытаться сохраниться, " .
                               "иначе весь опыт, найденные предметы и т.д. с последнего сохранения, " .
                               "когда покидали игру, будут потеряны.<br/><anchor>Назад<prev/></anchor>"),
            'title'        => "Ошибка",
            'show_journal' => 0
        ];
    } else {
        if ($b) {
            $b = "Если вы сейчас покинете игру, то ваш персонаж останется в игре еще минимум на " .
                 round($g_logout / 60) . " минут, так как рядом кто-то есть.";
        } else {
            manageNPC($login, $loc, "");
            @unlink("online/" . $login);
            $b     = "Так как рядом никого нет, то ваш персонаж только что покинул игру, " .
                     "теперь его не смогут в ваше отсутствие убить или обворовать." . "<br/>Удачного дня, " . $char[0] .
                     "!<br/><a href=\"$PHP_SELF\">На главную</a>";
            $login = "";
        }

        $logout_msg = [
            'text'         => "<p>Персонаж сохранен успешно.<br/>" . $b,
            'title'        => "Сохранение",
            'show_journal' => 0
        ];
    }
}
msg($logout_msg['text'], $logout_msg['title'], $logout_msg['show_journal']);