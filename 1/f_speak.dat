<?php
if ($speak) {
    if ($speak == 1 || ((substr($speak, 0, 2) == 'u.' || substr($speak, 0, 2) == 'n.') && isset($loc_i[$loc][$speak]))) {
        if (substr($speak, 0, 2) == 'u.') {
            $to = $speak;
            $speak = 1;
        }    // чат
        if ($speak != 1) $tchar = explode("|", $loc_i[$loc][$speak]["char"]);
        if ($char[8] && substr($speak, 0, 2) == "n." && substr($speak, 0, 4) != "n.h." && substr($speak, 0, 5) != "n.cap") msg("<p>Вы призрак и поэтому не можете ни с кем говорить, найдите лекаря или камень воскрешения");
        if ($speak == 1) {
            include "f_speak1.dat";
        } else        // с квестовым NPC
            if (substr($speak, 0, 4) == "n.q.") include "f_speakq.dat";
            else        // со своим
                if (substr($speak, 0, 4) == "n.o.") include "f_speako.dat";
                else
                    if ($loc_i[$loc][$speak]["owner"]) {
                        $owner = explode("|", $loc_i[$loc][$speak]["owner"]);
                        if ($owner[0] == $login) include "f_speakowner.dat"; else msg($tchar . " принадлежит другому персонажу");
                    } else        // с npc
                        if (file_exists("speak/" . $speak) || substr($speak, 0, 4) == "n.g.") {
                            if ($tchar[7] == $login) msg("<p>Вы не можете разговаривать с " . $tchar[0] . ", т.к. он вас атакует");
                            // диалог загрузится в $dialog (у стражи тоже)
                            if (substr($speak, 0, 4) == "n.g.") include "speak/n.g.guard"; else include "speak/" . $speak;
                            if ($char[7] == $speak) {
                                $char[7];
                                $loc_i[$loc][$login]["char"] = implode("|", $char);
                            };

                            if ($id == 'buy' || $id == 'sell') {
                                if (!$dialog[$id]) msg($tchar[0] . " торговать не будет");
                                if ($speak == "n.Natan" && strpos($loc_i[$loc][$login]["items"], "i.q.hagen:") === false && strpos($loc_i[$loc][$login]["items"], "i.q.ditrih:") === false) msg("Прости, но я продаю палладинское оружие и броню только стражникам барона Дитриха или тем кто имеет грамоту от Лорда Хагена");
                                $trader = explode("|", $dialog[$id]);
                                if (!$to) include "f_speak" . $id . ".dat"; else include "f_speak" . $id . "to.dat";
                            }

                            if ($id == 'tobank' || $id == 'frombank') {
                                if (!isset($dialog["bank"])) msg($tchar[0] . " не банкир");
                                if (!$to) include "f_speak" . $id . ".dat"; else include "f_speak" . $id . "to.dat";
                            }

                            if (!$id) $id = "begin";    // FIX: начальная тема "begin"
                            if (!isset($dialog[$id])) msg("Отсутствует тема \"$id\" в диалоге \"$speak\", сообщите разработчику игры, спасибо", "Ошибка");
                            $dialog = explode("#", $dialog[$id]);
                            $title = $dialog[0];    // может быть переназначен в "eval: "
                            if (substr($title, 0, 6) == "eval: ") eval(substr($title, 6));
                            if (substr($title, 0, 5) == "skill") include "f_speakskillup.dat";
                            $stmp = "<p>" . $title;
                            for ($i = 1; $i < count($dialog); $i += 2) {
                                $title = $dialog[$i];
                                $id = $dialog[$i + 1];
                                if (substr($title, 0, 6) == "eval: ") eval(substr($title, 6));
                                if ($title) $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=" . $id . "\">" . $title . "</a>";
                            }
                            if (count($dialog) == 1) $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid\">[Конец диалога]</a>";
                            $stmp = str_replace("<name>", $char[0], $stmp);    // заменяем <name> на имя игрока
                            msg($stmp, $tchar[0], 1);

                        } else addjournal($loc, $login, $tchar[0] . " не может с вами разговаривать");
    } else addjournal($loc, $login, "Не с кем говорить");
}
	

