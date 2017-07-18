<?php
/**
 * @global loc_i
 * @global loc
 * @global char
 * @global speak
 * @global login
 * @global id
 * @global PHP_SELF
 * @global sid
 */

if (empty($speak)) {
    return;
}
$npc_type = substr($speak, 0, 2);
if ($speak == 1 || (($npc_type == 'u.' || $npc_type == 'n.') && isset($loc_i[$loc][$speak]))) {
    if ($npc_type == 'u.') {
        $to    = $speak;
        $speak = 1;
    }
    $npc_subtype = substr($speak, 0, 4);
    if ($char[8] && $npc_type == "n." && $npc_subtype != "n.h." && substr($speak, 0, 5) != "n.cap") {
        msg("<p>Вы призрак и поэтому не можете ни с кем говорить, найдите лекаря или камень воскрешения");
    }
    // чат
    if ($speak == 1) {
        include "f_speak1.inc";
    } else {
        $tchar = explode("|", $loc_i[$loc][$speak]["char"]);
        // с квестовым NPC
        if ($npc_subtype == "n.q.") {
            include "f_speakq.inc";
        }
        else {
            // со своим
            if ($npc_subtype == "n.o.") {
                // диалог с стражником замка
                include "f_speako.inc";
            } else {
                if (!empty([$loc][$speak]["owner"])) {
                    $owner = explode("|", $loc_i[$loc][$speak]["owner"]);
                    if ($owner[0] == $login) {
                        include "f_speakowner.inc";
                    }
                    msg($tchar . " принадлежит другому персонажу");
                } else {
                    // с npc
                    if (file_exists("speak/" . $speak) || $npc_subtype == "n.g.") {
                        if ($tchar[7] == $login) {
                            msg("<p>Вы не можете разговаривать с " . $tchar[0] . ", т.к. он вас атакует");
                        }
                        // диалог загрузится в $dialog (у стражи тоже)
                        $dialog = $npc_subtype == "n.g." ? "speak/n.g.guard" : "speak/{$speak}";
                        // load dialog array
                        $dialog = require $dialog;
                        if ($char[7] == $speak) {
                            // FIXME: явный баг. непонятно, что имелось в виду
                            //$char[7];
                            // Может быть "прекратить атаковать при начале разговора"?
                            $char[7] = '';
                            $loc_i[$loc][$login]["char"] = implode("|", $char);
                        };

                        // разговор с торговцем
                        if ($id == 'buy' || $id == 'sell') {
                            if ( ! empty($dialog[$id])) {
                                msg($tchar[0] . " торговать не будет");
                            }
                            $user_items = $loc_i[$loc][$login]["items"];
                            if ($speak == "n.Natan" && strpos($user_items, "i.q.hagen:") === false &&
                                strpos($user_items, "i.q.ditrih:") === false
                            ) {
                                msg("Прости, но я продаю палладинское оружие и броню только стражникам барона Дитриха или тем кто имеет грамоту от Лорда Хагена");
                            }
                            $trader   = explode("|", $dialog[$id]);
                            $inc_name = "f_speak" . $id . (empty($to) ? ".inc" : "to.inc");
                            include $inc_name;
                        }

                        // разговор с банкиром
                        if ($id == 'tobank' || $id == 'frombank') {
                            if ( ! isset($dialog["bank"])) {
                                msg($tchar[0] . " не банкир");
                            }
                            $inc_name = "f_speak" . $id . (empty($to) ? ".inc" : "to.inc");
                            include $inc_name;
                        }

                        if (empty($id)) {
                            // начальная тема "begin"
                            $id = "begin";
                        }
                        if ( ! isset($dialog[$id])) {
                            msg("Отсутствует тема \"$id\" в диалоге \"$speak\", сообщите разработчику игры, спасибо",
                                "Ошибка");
                        }
                        $dialog = explode("#", $dialog[$id]);
                        $title  = $dialog[0];    // может быть переназначен в "eval: "
                        if (substr($title, 0, 6) == "eval: ") {
                            // скриптовая часть диалога
                            // TODO: избавиться от eval
                            eval(substr($title, 6));
                        }

                        // обучение
                        if (substr($title, 0, 5) == "skill") {
                            include "f_speakskillup.inc";
                        }
                        $stmp = "<p>" . $title;
                        for ($i = 1; $i < count($dialog); $i += 2) {
                            $title = $dialog[$i];
                            $id    = $dialog[$i + 1];
                            if (substr($title, 0, 6) == "eval: ") {
                                // скриптовая часть диалога
                                // TODO: избавиться от eval
                                eval(substr($title, 6));
                            }
                            if ($title) {
                                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=" . $id . "\">" . $title .
                                         "</a>";
                            }
                        }
                        if (count($dialog) == 1) {
                            $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid\">[Конец диалога]</a>";
                        }
                        // заменяем <name> на имя игрока
                        $stmp = str_replace("<name>", $char[0], $stmp);
                        msg($stmp, $tchar[0], 1);

                    } else {
                        addjournal($loc, $login, $tchar[0] . " не может с вами разговаривать");
                    }
                }
            }
        }
    }
} else {
    addjournal($loc, $login, "Не с кем говорить");
}
