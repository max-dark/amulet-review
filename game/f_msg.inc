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
 * @global msg
 * @global id
 * @global mall
 * @global translit
 */

use MaxDark\Amulet\OldCode\ViewOptions;

if ($msg) {
    $del = Request("del");
    if ($del && strpos($loc_i[$loc][$login]["msgt"], "{" . $del . "}") !== false) {
        $loc_i[$loc][$login]["msgt"] = substr(
            $loc_i[$loc][$login]["msgt"],
            strpos($loc_i[$loc][$login]["msgt"], "{" . $del . "}") + strlen("{" . $del . "}")
        );
    }
    if ($msg == 1) {    // основная страница
        $stmp .= "\n<br/><a href=\"?sid=$sid&msg=add\">Добавить</a>";
        // текст
        $del = 0;
        if ($loc_i[$loc][$login]["msgt"]) {
            $del = substr($loc_i[$loc][$login]["msgt"], -11, -1);
            $stmp .= preg_replace("/{[^}]+}/", "", $loc_i[$loc][$login]["msgt"]) .
                "<br/><a href=\"?sid=$sid&msg=clear&del=" . $del . "\">Очистить</a><br/>";
        } else {
            $stmp .= "\n<br/>Сообщений нет";
        }
        // список онлайн
        $stmp .= "<br/>Онлайн:";
        $count = 0;
        $c1 = 0;
        $c2 = 0;
        if ($loc_i[$loc][$login]["msg"]) {
            $keys = explode("|", $loc_i[$loc][$login]["msg"]);
        }
        if (!$start) {
            $start = 0;
        }
        $listEnd = $start + ViewOptions::getInstance()->getMaxListSize();
        if ($keys) {
            foreach ($keys as $i) {
                if ($i) {
                    if (file_exists("online/" . $i) && filesize("online/" . $i) != 1) {
                        $count++;
                        if ($c1 >= $start && $c1 < $listEnd) {
                            $stmp .= "\n<br/><div>" .
                                '<a tabindex="0" class="btn btn-secondary m-1" role="button" data-trigger="focus" data-toggle="popover" data-content="template" title="' . substr($i, 2) . '" label="' . $i . '">' . substr($i, 2) . '</a>';
                        }
                        $c1++;
                    }
                }
            }
        }
        if ($count == 0) {
            $stmp .= " никого нет";
        }

        if ($start) {
            $stmp .= "\n<br/><a href=\"?sid=$sid&msg=1\">^ </a>";
        }
        if ($listEnd < $c1) {
            if (!$start) {
                $stmp .= "\n<br/>";
            }
            $stmp .= "<a href=\"?sid=$sid&msg=1&start=" . ($listEnd) . "\">+ (" . ($c1 - ($listEnd)) .
                ")</a>";
        }
        $cnt = (count($keys) - $count);
        $stmp .= <<<TAG

<br/><a href="?sid=$sid&msg=off">Оффлайн ($cnt)</a>
<br/><br/><a href="?sid=$sid&msg=help">Помощь</a>
</p>
</div>
<div id="menu" title="Меню">
<p>
<a href="#write">Написать</a><br/>
[<a href="?sid=$sid&msg=up&id=$(to)">Вверх</a> | <a href="?sid=$sid&msg=down&id=$(to)">Вниз</a>]
<br/>---<br/>
<a href="?sid=$sid&msg=del&id=$(to)">Удалить</a>
</p>
</div>
<div id="write" title="Написать">
<p>
<form action="?sid=$sid" method="post">
<input class="form-control" name="text" emptyok="true" type="text" maxlength="140"/><br/>
<div class="form-check">
<input class="form-check-input" name="translit" type="checkbox" value="" id="translitCheck">
<label class="form-check-label" for="translitCheck">
Транслит
</label>
</div>
<div class="form-check">
<input class="form-check-input" name="mall" type="checkbox" value="" id="mallCheck">
<label class="form-check-label" for="mallCheck">
Всем
</label>
</div>
<input type="hidden" name="msg" value="write"/>
<postfield name="id" value="write"/>
<input class="btn btn-primary mt-1" type="submit" value="Отправить"/>
</form>
TAG;
    }

    if ($msg == 'off') {
        // список оффлайн
        $c1 = 0;
        $c2 = 0;
        $count = 0;
        if ($loc_i[$loc][$login]["msg"]) {
            $keys = explode("|", $loc_i[$loc][$login]["msg"]);
        }
        $listEnd = $start + ViewOptions::getInstance()->getMaxListSize();
        if ($keys) {
            if (count($keys) - $count > 0) {
                $stmp .= "Оффлайн (выберите, чтобы удалить):";
                foreach ($keys as $i) {
                    if ($i) {
                        if (!file_exists("online/" . $i) || filesize("online/" . $i) == 1) {
                            if ($c1 >= $start && $c1 < $listEnd) {
                                $stmp .= "\n<br/><a href=\"?sid=$sid&msg=del&id=" . $i . "\">" . substr($i, 2) . "</a>";
                            }
                            $c1++;
                            $count++;
                        }
                    }
                }
            }
        }

        if ($start) {
            $stmp .= "\n<br/><a href=\"?sid=$sid&msg=off\">^ </a>";
        }
        if ($listEnd < $c1) {
            if (!$start) {
                $stmp .= "\n<br/>";
            }
            $stmp .= "<a href=\"?sid=$sid&msg=off&start=" . ($listEnd) . "\">+ (" . ($c1 - $listEnd) .
                ")</a>";
        }
        if ($count == 0) {
            $stmp = "Список оффлайн пуст";
        }
    }
    if ($msg == 'add') {
        if (strlen($loc_i[$loc][$login]["msg"]) > 500) {
            msg("В контактах нет места, удалите кого-нибудь", "Ошибка");
        }
        if (!$id) {
            $stmp = "";
            foreach (array_keys($loc_i[$loc]) as $i) {
                if ($i != $login && substr($i, 0, 2) == "u.") {
                    $stmp .= "<br/><a href=\"?sid=$sid&msg=add&id=$i\">" . substr($i, 2) . "</a>";
                }
            }
            if ($stmp) {
                $stmp = "<p>Кого добавить:" . $stmp;
            } else {
                $stmp = "<p>Рядом с вами никого нет";
            }
        } else {
            if (isset($loc_i[$loc][$id])) {
                if (strpos($loc_i[$loc][$login]["msg"], $id . "|") === false) {
                    if (!$loc_i[$loc][$login]["msg"]) {
                        $loc_i[$loc][$login]["msg"] = $id;
                    } else {
                        $loc_i[$loc][$login]["msg"] .= "|" . $id;
                    }
                }
                $stmp = substr($id, 2) . " добавлен";
            } else {
                $stmp = substr($id, 2) . " нет рядом с вами.";
            }
        }
    }
    if ($msg == 'del') {
        if (strpos($loc_i[$loc][$login]["msg"], $id) === false) {
            msg("В ваших контактах нет " . substr($id, 2));
        }
        $loc_i[$loc][$login]["msg"] = str_replace("|" . $id, "", $loc_i[$loc][$login]["msg"]);
        $loc_i[$loc][$login]["msg"] = str_replace($id, "", $loc_i[$loc][$login]["msg"]);
        $stmp = substr($id, 2) . " удален";
    }
    if ($msg == 'write') {
        $text = Post("text");
        if (!$mall && (!file_exists("online/" . $id) || filesize("online/" . $id) == 1)) {
            msg(substr($id, 2) . " сейчас не в игре, оправлять сообщения можно только тем кто онлайн");
        }
        if (!$text) {
            msg("Нет текста для отправки");
        }
        if (strpos($text, "?sid=") !== false) {
            $text = substr($text, 0, strpos($text, "?sid="));
        }
        if ($translit) {
            include "f_translit.inc";
            $text = trans($text);
        }

        $s = $text;

        $s = str_replace('\\', "", $s);
        $s = preg_replace('/([^ -}А-я#])|\$|&/u', '', $s);
        $s = str_replace("|", "", $s);
        $s = str_replace("<", "", $s);
        $s = str_replace(">", "", $s);
        $text = $s;

        $keys = explode("|", $loc_i[$loc][$login]["msg"]);
        if ($keys) {
            foreach ($keys as $i) {
                if ($i) {
                    if (file_exists("online/" . $i) && filesize("online/" . $i) != 1 && ($id == $i || $mall)) {
                        $tmp = file("online/" . $i);
                        $tmp[0] = str_replace("\n", "", $tmp[0]);
                        $tmp[0] = str_replace("\r", "", $tmp[0]);

                        loadloc($tmp[0]);
                        if (!isset($loc_i[$tmp[0]][$i])) {
                            if ($mall) {
                                continue;
                            } else {
                                msg("В игре нет данных " . substr($i, 2), "Ошибка");
                            }
                        }
                        if (strpos($loc_i[$tmp[0]][$i]["msg"], $login) === false) {
                            if ($mall) {
                                continue;
                            } else {
                                msg("Вы не добавлены в контакты у " . substr($i, 2) .
                                    ", попросите его это сделать, иначе не сможете отправлять сообщения.");
                            }
                        }

                        $loc_i[$tmp[0]][$i]["msgt"] = substr($loc_i[$tmp[0]][$i]["msgt"] .= "<br/>" .
                            substr($login, 2) . ": " .
                            strip_tags($text) . "{" .
                            time() . "}", -500);
                        if (!$mall) {
                            $stmp = "Сообщение для " . substr($id, 2) . " отправлено";
                            break;
                        }
                    }
                }
            }
        }
        if (!$stmp) {
            $stmp = "Сообщение для всех отправлено";
        }
        //if ($login == "u.tatus") @mail("blade17@bk.ru", "BAG " . $text, $text);
    }

    if ($msg == "up" || $msg == "down") {
        include "f_msgm.inc";
    }

    if ($msg == 'help') {
        $stmp = 'Вы можете отправлять текстовые сообщения только игрокам, находящимся в игре.' .
            ' Причем вы должны быть добавлены у них в контактах тоже.' .
            ' Если вам придет новое сообщение, вы увидите спец. ссылку на игровом экране.';
    }

    if ($msg != 'clear') {
        msg($stmp . "<br/><a href=\"?sid=$sid&msg=1\">Контакты</a>", "Контакты", 0);
    }
}

