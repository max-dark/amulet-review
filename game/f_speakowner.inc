<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global id
 * @global tchar
 */

// диалог со своим животным или наемником

$who = Request('who');

$stmp = "<p>";
if (empty($id)) {            // основной диалог
    if (substr($speak, 0, 9) == "n.a.losh.") {
        $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=sedlo\">Сесть в седло</a><br/>";
    }
    $stmp .= "<a href=\"#battle\">Насчет боя...</a>";
    $stmp .= "<br/><a href=\"#move\">Насчет движения...</a>";
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=lask\">Приласкать</a>";
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=info\">Состояние</a>";
    if (in_array(substr($speak, 0, 4), ["n.a.", "n.z.", "n.s."])) {
        $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=name\">Изменить имя</a>";
    }
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=free\">Нам пора расстаться</a>";
    $stmp .= "\n</p>\n</card>\n<card id=\"battle\" title=\"Насчет боя...\">\n";
    $stmp .= "<p>\n<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=guardme\">Защищай меня</a>\n";
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=nelez\">Не лезь в драку</a>\n";
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=attacklist\">Атакуй...</a>\n";
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=guardlist\">Защищай...</a>";
    $stmp .= "\n</p>\n</card>\n<card id=\"move\" title=\"Насчет движения...\">\n";
    $stmp .= "<p>\n<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=followme\">Следуй за мной</a>\n";
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=stay\">Стой здесь</a>\n";
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=followlist\">Следуй за...</a>";
}
switch ($id) {
    case 'sedlo':    // в седло
        if ($char[8]) {
            msg("Вы призрак, лошадь вас боится и не подпускает близко");
        }
        if (!isset($loc_i[$loc][$speak])) {
            msg("<p>Рядом нет этого коня");
        }
        if ($char[12]) {
            msg("Вы и так на коне");
        }
        $tc = explode("|", $loc_i[$loc][$speak]["char"]);
        if ($char[6] >= time()) {
            msg("Вы должны отдохнуть");
        }
        $char[12] = "1:" . $tc[1];
        $char[6] = time() + 3;
        $loc_i[$loc][$login]["char"] = implode("|", $char);
        calcparam($loc, $login);
        addjournal($loc, "all", $char[0] . " сел на лошадь", $login);
        if (isset($loc_i[$loc][$speak]["name"])) {
            $loc_i[$loc][$login]["name"] = $loc_i[$loc][$speak]["name"];
        }
        unset($loc_i[$loc][$speak]);

        msg("Вы сели на лошадь и теперь вы всадник");
        break;
    case 'guardme':    // охранять меня
        $owner[2] = $login;
        $stmp .= $tchar[0] . " теперь будет защищать вас";
        break;
    case 'nelez':    // не вмешиваться в бой
        $owner[2] = "";
        $stmp .= $tchar[0] . " не будет вмешиваться в бой";
        break;
    case 'guardlist':    // список кого защищать
        $stmp = "<p>Выберите кого защищать:";
        foreach (array_keys($loc_i[$loc]) as $i) {
            if ($i != $login && $i != $speak) {
                if (substr($i, 0, 2) == 'n.' || substr($i, 0, 2) == 'u.') {
                    $tto = explode("|", $loc_i[$loc][$i]["char"]);
                    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=guard&who=" . $i . "\">" . $tto[0] .
                        "</a>";
                }
            }
        }
        break;
    case 'guard': // защищать $who
        if (empty($who)) {
            $stmp .= "Укажите цель";
            break;
        }
        // проверка не более 3-х наемников
        $tt = $who;
        $tn = 0;
        if ($loc_i[$loc][$who]["owner"]) {
            $tt = substr($loc_i[$loc][$who]["owner"], 0, strpos($loc_i[$loc][$who]["owner"], "|"));
        }
        foreach ($loc_i[$loc] as $i => $v) {
            if ($loc_i[$loc][$i]["owner"]) {
                $tow = explode("|", $loc_i[$loc][$i]["owner"]);
                if ($tow[0] == $tt || $tow[1] == $tt || $tow[2] == $tt) {
                    $tn++;
                }
            }
        }
        if ($tn > 2) {
            msg("Наемникам слишком тесно (можно не более 3 в одном месте)");
        }

        $owner[2] = $who;
        $stmp .= $tchar[0] . " с этого момента будет защищать кого вы указали.";
        break;
    case 'attacklist':    // список кого атаковать
        $stmp = "<p>Выберите кого атаковать:";
        foreach (array_keys($loc_i[$loc]) as $i) {
            if ($i != $login && $i != $speak) {
                if (substr($i, 0, 2) == 'n.' || substr($i, 0, 2) == 'u.') {
                    $tto = explode("|", $loc_i[$loc][$i]["char"]);
                    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=attack&who=" . $i . "\">" . $tto[0] .
                        "</a>";
                }
            }
        }
        break;
    case 'attack': // атаковать $who
        if (empty($who)) {
            $stmp .= "Укажите цель";
            break;
        }
        if (!isset($loc_i[$loc][$who])) {
            msg("Нет цели");
        }
        if ($who == $login) {
            msg("Нельзя натравливать на себя");
        }
        $tchar[7] = $who;
        $stmp .= $tchar[0] . " атакует кого вы указали.";
        // крим если атакует не крима или животное в городе
        $tc = explode("|", $loc_i[$loc][$who]["char"]);
        $fchar = explode("|", $loc_i[$loc][$login]["char"]);
        $fcrim = $fchar[9];
        $tcrim = $tc[9] || substr($who, 0, 4) == "n.c.";
        if (!$fcrim && $tc[7] != $login &&
            ((!$tcrim && substr($who, 0, 4) != "n.a.") || ($loc_c[1] == 1 && substr($who, 0, 4) == "n.a."))
        ) {
            docrim($loc, $login);
            if ($loc_c[1] == 1) {
                $tchar[9] = "преступник";
                $tchar[10] = time() + $g_crim;
            }
        }
        $loc_i[$loc][$speak]["char"] = implode("|", $tchar);
        break;
    case 'followme':    // следуй за мной
        $owner[1] = $login;
        $stmp .= $tchar[0] . " теперь будет следовать за вами";
        break;
    case 'stay':    // стой здесь
        $owner[1] = "";
        $stmp .= $tchar[0] . " будет ждать здесь, пока не позовете";
        break;
    case 'followlist':    // список за кем следовать
        $stmp = "<p>Выберите за кем следовать:";
        foreach (array_keys($loc_i[$loc]) as $i) {
            if ($i != $login && $i != $speak) {
                if (substr($i, 0, 2) == 'u.') {
                    $tto = explode("|", $loc_i[$loc][$i]["char"]);
                    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=follow&who=" . $i . "\">" . $tto[0] .
                        "</a>";
                }
            }
        }
        break;
    case 'follow':
        // следовать за $who
        if (empty($who)) {
            $stmp .= "Укажите цель";
            break;
        }
        // проверка не более 3-х наемников
        $tt = $who;
        $tn = 0;
        if ($loc_i[$loc][$who]["owner"]) {
            $tt = substr($loc_i[$loc][$who]["owner"], 0, strpos($loc_i[$loc][$who]["owner"], "|"));
        }
        foreach ($loc_i[$loc] as $i => $v) {
            if ($loc_i[$loc][$i]["owner"]) {
                $tow = explode("|", $loc_i[$loc][$i]["owner"]);
                if ($tow[0] == $tt || $tow[1] == $tt || $tow[2] == $tt) {
                    $tn++;
                }
            }
        }
        if ($tn > 2) {
            msg("Наемникам слишком тесно (можно не более 3 в одном месте)");
        }

        $owner[1] = $who;
        $owner[2] = $who;
        $stmp .= $tchar[0] . " с этого момента будет следовать за кем вы указали.";
        break;
    case 'name':    // расстаться
        if (substr($speak, 0, 4) != "n.a." && substr($speak, 0, 4) != "n.z." && substr($speak, 0, 4) != "n.s.") {
            msg("Давать имена можно только животным или зомби, у людей уже есть имена ;-)");
        }
        $new = Request('new');
        if ($new != "") {
            if (strlen($new) > 10) {
                msg("Имя должно быть не длиннее 10 символов");
            }
            if ($tr = Request('tr')) {
                include "f_translit.inc";
                $new = trans($new);
            }
            $s = $new;
            // UTF-8 русские буквы
            $s = str_replace("\xd0\x81", "Ё", $s);
            $s = str_replace("\xd1\x91", "ё", $s);
            $s = preg_replace("/\xd0([\x90-\xbf])/e", "chr(ord('\\1')+48)", $s);
            $s = preg_replace("/\xd1([\x80-\x8f])/e", "chr(ord('\\1')+112)", $s);
            $s = preg_replace('/[^A-zА-я0-9]/', "", $s);

            $new = $s;
            if (!$s) {
                msg("Не указано имя");
            }

            if (isset($loc_i[$loc][$speak]["name"])) {
                $tchar[0] = substr($tchar[0], 0, -strlen($loc_i[$loc][$speak]["name"]) - 1);
            }
            $tchar[0] .= " " . $new;
            $loc_i[$loc][$speak]["name"] = $new;
            $loc_i[$loc][$speak]["char"] = implode("|", $tchar);
            $stmp .= "Имя изменено на " . $tchar[0];
        } else {
            $stmp .= "<input name=\"new\" emptyok=\"true\" type=\"text\" value=\"" . $loc_i[$loc][$speak]["name"] . "\"/>";
            $stmp .= "<br/><select name=\"tr\" multiple=\"true\" value=\"1\">";
            $stmp .= "<option value=\"1\">Транслит</option>";
            $stmp .= "</select><br/>";
            $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=name&new=$(new)&tr=$(tr)\">Изменить имя</a>";
        }
        break;
    case 'lask':    // приласкать
        $skills = explode("|", $loc_i[$loc][$login]["skills"]);
        if ($owner[3] > time() + 240) {
            msg($tchar[0] .
                " не собирается покидать вас в ближайшие несколько минут, нет смысла уговаривать его остаться еще (попробуйте повторить когда срок службы будет на исходе).");
        }
        $char[6] = time() + 10;
        $loc_i[$loc][$login]["char"] = implode("|", $char);
        if (rand(0, 100) < $skills[7] * 10) {
            if ($owner[3]) {
                $owner[3] += rand(60, 60 + $skills[20] * 60);
                $stmp .= "Кажется, вы стали " . $tchar[0] . " нравиться немного больше";
            } else {
                $stmp .= $tchar[0] . " будет вам предан всегда, нет нужды в поощрении";
            }
        } else {
            $stmp .= "Ваша попытка не произвела на " . $tchar[0] . " впечатления";
        }
        break;
    case 'info':    // инфо когда покинет
        if (!$owner[3]) {
            $stmp .= $tchar[0] . " никогда вас не покинет";
        } else {
            $skills = explode("|", $loc_i[$loc][$login]["skills"]);
            $timeleft = ($owner[3] - time()) / 60;
            if ($timeleft < 60 * 6) {
                $pogr = rand(0, 10 * (10 - $skills[20] * 2));
                $pogr = $pogr * $timeleft / 100;
                $tc = round($timeleft - $pogr) . " - " . round($timeleft + $pogr) . " минут";
            } else {
                $tc = "примерно " . round($timeleft / 60) . " часа";
            }
            $stmp .= $tchar[0] . " покинет вас через " . $tc;
        }
        break;
    case 'free':    // расстаться
        if ($owner[6]) {
            unset($loc_i[$loc][$speak]["owner"]);
            addnpc($speak, $loc, $owner[6]);
        } else {
            if ($owner[4]) {
                // remove NPC from location
                addnpc($speak, $loc);
            } else {
                unset($loc_i[$loc][$speak]["owner"]);
            }
        }
        msg($tchar[0] . " покинул вас");
        break;
}

if (!empty($owner)) {
    $loc_i[$loc][$speak]["owner"] = implode("|", $owner);
}
msg($stmp, $tchar[0]);
