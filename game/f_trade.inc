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
 * @global tchar
 * @global num
 * @global id
 */
// торговля

use MaxDark\Amulet\OldCode\PageType;

if ($trade == "help") {
    msg("Чтобы торговать или передать предмет, выберите игрока и потом в меню пункт \"Обмен\". То же самое, если предлагают обмен вам. Добавляйте предметы в список или ждите, когда добавит собеседник. Обмен происходит когда обе стороны выбирают ссылку \"Я согласен\". Выбор своего предмета удалит его из списка, в выбор в списке собеседника покажет информацию о предмете.");
}

if (substr($trade, 0, 2) == "u.") { //новая торговля
    if ($char[8]) {
        msg("Вы призрак и не можете вести обмен");
    }
    if ($trade == $login) {
        msg("Нельзя торговать с самим собой");
    }
    if (!isset($loc_i[$loc][$trade])) {
        msg("Рядом с вами нет того, с кем вы хотели торговать. Если он ушел, вы можете догнать его и продолжить обмен.");
    }
    $tchar = explode("|", $loc_i[$loc][$trade]["char"]);
    if ($tchar[8]) {
        msg($tchar[0] . " призрак, с ним нельзя вести обмен");
    }
    // Initialize "trade" structure if it does not exist
    if (!isset($loc_i[$loc][$login]["trade"])) {
        $loc_i[$loc][$login]["trade"] = ["to" => "", "i" => "", "ito" => "", "a" => 0];
    }

    if (!isset($loc_i[$loc][$trade]["trade"])) {
        $loc_i[$loc][$trade]["trade"] = ["to" => "", "i" => "", "ito" => "", "a" => 0];
    }
    $loc_i[$loc][$login]["trade"] = ["to" => $trade, "i" => "", "ito" => "", "a" => 0];
    if (isset($loc_i[$loc][$trade]["trade"]["to"]) && $loc_i[$loc][$trade]["trade"]["to"] != $login) {
        addjournal($loc, $trade, $char[0] . ": предлагает вам обмен, выберите разговор с ним и ссылку Обмен");
        msg("Вы предложили обмен, теперь надо подождать согласия другой стороны.<br/><a href=\"$PHP_SELF?sid=$sid&trade=1\">Продолжить</a>");
    } else {
        if (isset($loc_i[$loc][$trade]["trade"]) && isset($loc_i[$loc][$login]["trade"])) {
            $loc_i[$loc][$login]["trade"]["ito"] = $loc_i[$loc][$trade]["trade"]["i"];
            $loc_i[$loc][$trade]["trade"]["ito"] = $loc_i[$loc][$login]["trade"]["i"];
            $loc_i[$loc][$trade]["trade"]["a"] = 0;
        }
    }
}

// проверки
if (isset($loc_i[$loc][$login]["trade"])) {
    $to = $loc_i[$loc][$login]["trade"]["to"];
    if (substr($to, 0, 2) != "u.") {
        msg("Торговать можно только с игроками");
    }
    if (!isset($loc_i[$loc][$to])) {
        msg("Рядом с вами нет того, с кем вы хотели торговать. Если он ушел, вы можете догнать его и продолжить обмен.");
    }
    $tchar = explode("|", $loc_i[$loc][$to]["char"]);
    if ($tchar[8]) {
        msg($tchar[0] . " призрак, с ним нельзя вести обмен");
    }
    if ($char[8]) {
        msg("Вы призрак и не можете вести обмен");
    }
    $tchar = $tchar[0];
    if (!isset($loc_i[$loc][$to]["trade"])) {
        msg($tchar .
            " сейчас не торгует с вами, ждите пока он начнет с вами обмен (если захочет, конечно).<br/><a href=\"$PHP_SELF?sid=$sid&trade=1\">Продолжить</a>");
    }
    if ($loc_i[$loc][$to]["trade"]["to"] != $login) {
        msg($tchar .
            " в режиме торговли, но не с вами. Возможно, вам нужно подождать, пока он начнет с вами обмен.<br/><a href=\"$PHP_SELF?sid=$sid&trade=1\">Продолжить</a>");
    }
    if ($id != "") {
        $id = str_replace("*", "", $id);
    }

    $tc = intval(preg_replace('/.*' . $id . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["trade"]["i"]));
    if ($trade == "add") {    // добавляем предмет
        $tcount = manageItems($loc, $login, "", $id, "count");
        if ($tcount == 0) {
            msg("У вас нет этого предмета");
        }
        // проверим кол-во
        if ($tcount > 1 && !$num) {        // запросим кол-во
            $stmp = "<p>Укажите количество:\n<br/><form action=\"$PHP_SELF\" method=\"get\">" .
                "<input name=\"sid\" value=\"" . $sid . "\" type=\"hidden\"/>" .
                "<input name=\"trade\" value=\"$trade\" type=\"hidden\"/>" .
                "<input name=\"id\" value=\"" . $id . "\" type=\"hidden\"/>" .
                "<input class=\"form-control\" name=\"num\" format=\"*N\" maxlength=\"7\" value=\"" . $count .
                "\"/>\n<br/><button class=\"btn btn-outline-primary\" type=\"submit\">Отправить</button></form></p>";
            msg($stmp, "Добавить", 1, PageType::MSG, "num", $tcount);
        }
        $num = intval($num);
        if (!$num || $num < 1) {
            $num = 1;
        }    // если не указано, то 1 предмет
        if ($num > $tcount) {
            $num = $tcount;
        }    // и максимум берем все что есть
        if ($tc > 0) {
            $loc_i[$loc][$login]["trade"]["i"] = preg_replace(
                '/' . $id . ':\d+/',
                $id . ":" . ($tc + $num),
                $loc_i[$loc][$login]["trade"]["i"]
            );
        } else {
            if ($loc_i[$loc][$login]["trade"]["i"]) {
                $loc_i[$loc][$login]["trade"]["i"] .= "|" . $id . ":" . $num;
            } else {
                $loc_i[$loc][$login]["trade"]["i"] = $id . ":" . $num;
            }
        }
        $loc_i[$loc][$to]["trade"]["ito"] = $loc_i[$loc][$login]["trade"]["i"];
        $loc_i[$loc][$login]["trade"]["a"] = 0;
        $loc_i[$loc][$to]["trade"]["a"] = 0;
    }

    if ($trade == "del") {    // удаляем предмет
        if (strpos($loc_i[$loc][$login]["trade"]["i"], $id . ":") === false) {
            msg("В вашем списке для обмена нет этого предмета");
        }
        $loc_i[$loc][$login]["trade"]["i"] = preg_replace(
            '/\|?' . $id . ':\d+\|?/',
            "|",
            $loc_i[$loc][$login]["trade"]["i"]
        );
        if (substr($loc_i[$loc][$login]["trade"]["i"], 0, 1) == "|") {
            $loc_i[$loc][$login]["trade"]["i"] = substr($loc_i[$loc][$login]["trade"]["i"], 1);
        }
        if (substr($loc_i[$loc][$login]["trade"]["i"], -1) == "|") {
            $loc_i[$loc][$login]["trade"]["i"] = substr($loc_i[$loc][$login]["trade"]["i"], 0, -1);
        }
        $loc_i[$loc][$to]["trade"]["ito"] = $loc_i[$loc][$login]["trade"]["i"];
        $loc_i[$loc][$login]["trade"]["a"] = 0;
        $loc_i[$loc][$to]["trade"]["a"] = 0;
    }

    $b = 1;
    if ($trade == "cancel") {    // отказ
        unset($loc_i[$loc][$login]["trade"]);
        unset($loc_i[$loc][$to]["trade"]);
        addjournal($loc, $to, $char[0] . ": отказался от обмена с вами");
        $b = 0;
    }

    // подготовим списки вещей
    if ($loc_i[$loc][$login]["trade"]["i"]) {
        $i = explode("|", $loc_i[$loc][$login]["trade"]["i"]);
    } else {
        $i = [];
    }
    foreach (array_keys($i) as $j) {
        $i[$j] = explode(":", $i[$j]);
    }
    if ($loc_i[$loc][$login]["trade"]["ito"]) {
        $ito = explode("|", $loc_i[$loc][$login]["trade"]["ito"]);
    } else {
        $ito = [];
    }
    foreach (array_keys($ito) as $j) {
        $ito[$j] = explode(":", $ito[$j]);
    }

    if ($trade == "agry" && $b) {    // согласен, если список предметов равен у обоих и тот согласен, то обмен
        $loc_i[$loc][$login]["trade"]["a"] = 1;

        if ($loc_i[$loc][$login]["trade"]["i"] . $loc_i[$loc][$login]["trade"]["ito"] == "") {
            msg("Нет предметов для обмена");
        }
        if (
            $loc_i[$loc][$login]["trade"]["i"] . $loc_i[$loc][$login]["trade"]["ito"] !=
            $loc_i[$loc][$to]["trade"]["ito"] . $loc_i[$loc][$to]["trade"]["i"]
        ) {
            $loc_i[$loc][$login]["trade"]["a"] = 0;
            $loc_i[$loc][$to]["trade"]["a"] = 0;
        }
        if ($loc_i[$loc][$login]["trade"]["a"] && $loc_i[$loc][$to]["trade"]["a"]) {    // обмен
            // проверим наличие предметов
            $b = 0;
            foreach (array_keys($i) as $j) {
                if (
                    intval(preg_replace('/.*' . $i[$j][0] . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["items"])) <
                    $i[$j][1]
                ) {
                    $b = 1;
                    $item = findItemByFullId($i[$j][0]);
                    addjournal($loc, $login, "У вас нет: " . $i[$j][1] . " " . $item[0]);
                    break;
                }
            }
            foreach (array_keys($ito) as $j) {
                if (
                    intval(preg_replace('/.*' . $ito[$j][0] . ':(\d+).*/', "\\1", $loc_i[$loc][$to]["items"])) <
                    $ito[$j][1]
                ) {
                    $b = 1;
                    $item = findItemByFullId($ito[$j][0]);
                    addjournal($loc, $login, "У " . $tchar . " нет: " . $ito[$j][1] . " " . $item[0]);
                    break;
                }
            }
            if ($b == 0) {    // передаем вещи
                foreach (array_keys($i) as $j) {
                    manageItems($loc, $login, $to, $i[$j][0], $i[$j][1], "items", "items", 1, -1, 1);
                }
                foreach (array_keys($ito) as $j) {
                    manageItems($loc, $to, $login, $ito[$j][0], $ito[$j][1], "items", "items", 1, -1, 1);
                }
                unset($loc_i[$loc][$login]["trade"]);
                unset($loc_i[$loc][$to]["trade"]);
            }
        }
    }

    // выводим осн. страницу
    if ($b) {
        $stmp = $tchar . " предлагает:";
        if (count($ito) == 0) {
            $stmp .= "<br/>[пусто]";
        }
        foreach (array_keys($ito) as $j) {
            $item = findItemByFullId($ito[$j][0]);
            $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&look=" . $ito[$j][0] . "\">" . $item[0];
            if ($ito[$j][1] > 1) {
                $stmp .= " (" . $ito[$j][1] . ")";
            }
            $stmp .= "</a>";
        }
        $stmp .= "<br/>Вы за это отдаете:";
        if (count($i) == 0) {
            $stmp .= "<br/>[пусто]";
        }
        foreach (array_keys($i) as $j) {
            $item = findItemByFullId($i[$j][0]);
            $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&trade=del&id=" . $i[$j][0] . "\">" . $item[0];
            if ($i[$j][1] > 1) {
                $stmp .= " (" . $i[$j][1] . ")";
            }
            $stmp .= "</a>";
        }
        if ($loc_i[$loc][$to]["trade"]["a"]) {
            $stmp .= "<br/>" . $tchar . " согласен";
        } else {
            $stmp .= "<br/>" . $tchar . " пока не согласен";
        }
        $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&trade=agry\">Я согласен</a><br/><a href=\"$PHP_SELF?sid=$sid&list=inv\">Добавить предмет</a><br/><a href=\"$PHP_SELF?sid=$sid&trade=1\">Обновить</a><br/><a href=\"#say\">Сказать</a><br/><a href=\"$PHP_SELF?sid=$sid&trade=cancel\">Отмена</a><br/><br/><a href=\"$PHP_SELF?sid=$sid&trade=help\">Помощь</a>";
        $stmp .= "\n</p>\n</div>\n<div id=\"say\" title=\"Сказать\">\n<p><input name=\"say\" emptyok=\"true\" type=\"text\" value=\"\"/>\n<br/><select name=\"translit\" multiple=\"true\" value=\"1\"><option value=\"1\">Транслит</option></select>
<anchor>Сказать<go href=\"$PHP_SELF?sid=$sid&trade=1\" method=\"post\">
<postfield name=\"to\" value=\"$to\"/>
<postfield name=\"say\" value=\"$(say)\"/>
<postfield name=\"translit\" value=\"$(translit)\"/>
</go></anchor>";
        msg($stmp, "Обмен");
    }
} // если есть "trade"
