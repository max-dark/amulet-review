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
        $to = $speak;
        $speak = 1;
    }
    $npc_subtype = substr($speak, 0, 4);
    if ($char[8] && $npc_type == "n." && $npc_subtype != "n.h." && substr($speak, 0, 5) != "n.cap") {
        msg("<p>Вы призрак и поэтому не можете ни с кем говорить, найдите лекаря или камень воскрешения");
    }
    if ($speak == 1) {
        // чат
        if ($to && isset($loc_i[$loc][$to])) {
            $tt    = $to;
            $tchar = explode("|", $loc_i[$loc][$to]["char"]);
            $to    = $tchar[0] . ", ";
        } else {
            $to = "";
        }
        $stmp = "<p>";
        if (!empty($to)) {
            $stmp .= "<input name=\"to\" emptyok=\"true\" type=\"text\" value=\"$to\"/><br />";
        }
        $stmp .= "<input name=\"say\" emptyok=\"true\" type=\"text\" maxlength=\"250\" value=\"\"/>\n";
        $stmp .= "<br/>\n";
        $stmp .= "<anchor>Сказать\n";
        $stmp .= "<go href=\"?sid=$sid\" method=\"post\">\n";
        $stmp .= "<postfield name=\"to\" value=\"$(to)\"/>\n";
        $stmp .= "<postfield name=\"say\" value=\"$(say)\"/>\n";
        $stmp .= "</go></anchor>";

        if (!empty($tt)) {
            $stmp .= "<br/><a href=\"?sid=$sid&trade=$tt\">Обмен</a>";
        }

        msg($stmp, "Сказать", 1);
    } else {
        $tchar = explode("|", $loc_i[$loc][$speak]["char"]);
        if ($npc_subtype == "n.q.") {
            // с квестовым NPC
            // диалог с квестовым NPC:
            // при первом обращении присоединяется, при втором уходит.
            // только следует, выдает инфу в in и out.
            // присоединиться нанятый может только если рядом нет хозяина, иначе говорит, что следует за таким-то

            if (isset($loc_i[$loc][$speak]["nspeak"])) {
                $items = explode(" | ", $loc_i[$loc][$speak]["nspeak"]);
                if ($items[1] && strpos($loc_i[$loc][$login]["items"], $items[1] . ":") === false) {
                    msg($items[0] . "<br/><a href=\"?sid=$sid\">Гм... Понятно</a>", $tchar[0]);
                }
            }

            $owner = $loc_i[$loc][$speak]["owner"];
            if (!$owner || !isset($loc_i[$loc][substr($owner, 0, strpos($owner, "|"))])) {
                $loc_i[$loc][$speak]["owner"] = $login . "|" . $login . "||";
                msg($loc_i[$loc][$speak]["in"] . "<br/><a href=\"?sid=$sid\">Хорошо, следуй за мной</a>", $tchar[0]);
            } elseif (substr($owner, 0, strpos($owner, "|")) == $login) {
                if (isset($loc_i[$loc][$speak]["ok"]) &&
                    substr($loc_i[$loc][$speak]["ok"], 0, strpos($loc_i[$loc][$speak]["ok"], " | ")) == $loc
                ) {
                    $items = explode(" | ", $loc_i[$loc][$speak]["ok"]);
                    if ($items[2]) {
                        $it = explode("|", $items[2]);
                    } else {
                        $it = "";
                    }
                    if ($it) {
                        foreach ($it as $i) {
                            $i = explode(":", $i);
                            additem($loc, "", $login, $i[0], $i[1]);
                        }
                    }
                    if (isset($loc_i[$loc][$speak]["nspeak"])) {
                        $it1 = explode(" | ", $loc_i[$loc][$speak]["nspeak"]);
                        if ($it1[1] && strpos($loc_i[$loc][$login]["items"], $it1[1] . ":") !== false) {
                            additem($loc, $login, "", $it1[1], 1);
                        }
                    }
                    addnpc($speak, $loc, "");
                    unset($loc_i[$loc][$speak]);
                    msg($items[1] . "<br/><a href=\"?sid=$sid\">Ну ладно, прощай</a>", $tchar[0]);
                } else {
                    unset($loc_i[$loc][$speak]["owner"]);
                    msg($loc_i[$loc][$speak]["out"] . "<br/><a href=\"?sid=$sid\">Мне пора, прощай</a>", $tchar[0]);
                }
            } else {
                $tc = explode("|", $loc_i[$loc][substr($owner, 0, strpos($owner, "|"))]["char"]);
                msg("Извини, " . $char[0] . ", но я сейчас следую за " . $tc[0] . ", с тобой могу пойти только если " . $tc[0] .
                    " откажется провожать меня или я потеряю его из вида.<br/><a href=\"?sid=$sid\">Понятно, пока</a>",
                    $tchar[0]);
            }
        } elseif ($npc_subtype == "n.o.") {
            // диалог с стражником замка
            include "f_speako.inc"; // возможен возврат управления(?)
        } elseif (!empty([$loc][$speak]["owner"])) {
            // со своим
            $owner = explode("|", $loc_i[$loc][$speak]["owner"]);
            if ($owner[0] == $login) {
                include "f_speakowner.inc"; // возможен возврат управления
            }
            msg($tchar . " принадлежит другому персонажу");
        } elseif (file_exists("speak/" . $speak) || $npc_subtype == "n.g.") {
            // с npc
            if ($tchar[7] == $login) {
                msg("<p>Вы не можете разговаривать с " . $tchar[0] . ", т.к. он вас атакует");
            }
            // диалог загрузится в $dialog (у стражи тоже)
            $dialog_file = $npc_subtype == "n.g." ? "speak/n.g.guard" : "speak/{$speak}";
            // load dialog array
            $dialog = require $dialog_file;
            if ($char[7] == $speak) {
                // FIXME: явный баг. непонятно, что имелось в виду
                //$char[7];
                // Может быть "прекратить атаковать при начале разговора"?
                $char[7] = '';
                $loc_i[$loc][$login]["char"] = implode("|", $char);
            };

            // разговор с торговцем
            if ($id == 'buy' || $id == 'sell') {
                if (!empty($dialog[$id])) {
                    msg($tchar[0] . " торговать не будет");
                }
                $user_items = $loc_i[$loc][$login]["items"];
                if ($speak == "n.Natan" && strpos($user_items, "i.q.hagen:") === false &&
                    strpos($user_items, "i.q.ditrih:") === false
                ) {
                    msg("Прости, но я продаю палладинское оружие и броню только стражникам барона Дитриха или тем кто имеет грамоту от Лорда Хагена");
                }
                $trader = explode("|", $dialog[$id]);
                $inc_name = "f_speak" . $id . (empty($to) ? ".inc" : "to.inc");
                include $inc_name;
            }

            // разговор с банкиром
            if ($id == 'tobank' || $id == 'frombank') {
                if (!isset($dialog["bank"])) {
                    msg($tchar[0] . " не банкир");
                }
                $inc_name = "f_speak" . $id . (empty($to) ? ".inc" : "to.inc");
                include $inc_name; // без возврата управления
            }

            if (empty($id)) {
                // начальная тема "begin"
                $id = "begin";
            }
            if (!isset($dialog[$id])) {
                msg("Отсутствует тема \"$id\" в диалоге \"$speak\", сообщите разработчику игры, спасибо",
                    "Ошибка");
            }
            $current_dialog = explode("#", $dialog[$id]);
            $title = $current_dialog[0]; // может быть переназначен в "eval: "
            if (substr($title, 0, 6) == "eval: ") {
                // скриптовая часть диалога
                // TODO: избавиться от eval
                eval(substr($title, 6));
            }

            // обучение
            if (substr($title, 0, 5) == "skill") {
                include "f_speakskillup.inc"; // без возврата управления
            }
            $stmp = "<p>" . $title;
            for ($i = 1; $i < count($current_dialog); $i += 2) {
                $title = $current_dialog[$i]; // может быть переназначен в "eval: "
                $id = $current_dialog[$i + 1]; // может быть переназначен в "eval: "
                if (substr($title, 0, 6) == "eval: ") {
                    // скриптовая часть диалога
                    // TODO: избавиться от eval
                    eval(substr($title, 6));
                }
                if ($title) {
                    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id\">$title</a>";
                }
            }
            if (count($current_dialog) == 1) {
                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid\">[Конец диалога]</a>";
            }
            // заменяем <name> на имя игрока
            $stmp = str_replace("<name>", $char[0], $stmp);
            msg($stmp, $tchar[0], 1);

        } else {
            addjournal($loc, $login, $tchar[0] . " не может с вами разговаривать");
        }
    }
} else {
    addjournal($loc, $login, "Не с кем говорить");
}
