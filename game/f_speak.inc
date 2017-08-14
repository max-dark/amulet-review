<?php
/**
 * @global loc_i
 * @global loc_tt
 * @global loc
 * @global char
 * @global speak
 * @global login
 * @global id
 * @global PHP_SELF
 * @global sid
 */

use MaxDark\Amulet\OldCode\Transliterate;
use MaxDark\Amulet\OldCode\ViewOptions;

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
                            manageItems($loc, "", $login, $i[0], $i[1]);
                        }
                    }
                    if (isset($loc_i[$loc][$speak]["nspeak"])) {
                        $it1 = explode(" | ", $loc_i[$loc][$speak]["nspeak"]);
                        if ($it1[1] && strpos($loc_i[$loc][$login]["items"], $it1[1] . ":") !== false) {
                            manageItems($loc, $login, "", $it1[1], 1);
                        }
                    }
                    manageNPC($speak, $loc, "");
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

            if (substr($loc, 0, 2) != "c.") {
                // в дороге...
                $owner = explode("|", $loc_i[$loc][$speak]["owner"]);
                msg("Я нанят для защиты ворот замка от непрошенных гостей. Если в течении " . round(($owner[3] - time()) / 60) .
                    " минут я не попаду в замок, который должен защищать, то контракт будет расторгнут.");
            }

            // в замке...
            // инфа о замке
            $gate = substr($loc, 0, 4) . "gate";
            $d    = explode("|", $loc_tt[$gate]["d"]);
            if (strpos($d[0], "*") === false) {
                msg("Этот замок никому не принадлежит, я не буду защищать его, пока не определится хозяин этой земли!");
            }
            $clanc = substr($d[0], strpos($d[0], "*") + 1, strrpos($d[0], "*") - strpos($d[0], "*") - 1);

            // слушается только клановцев, владеющих замком, гостей нет
            if (strpos($char[0], "*") === false) {
                $clan = "";
            } else {
                $clan = substr($char[0], strpos($char[0], "*") + 1, strrpos($char[0], "*") - strpos($char[0], "*") - 1);
            }
            if ($clanc != $clan) {
                msg("Вы не состоите в клане, которому принадлежит этот замок, поэтому мне нечего вам сказать. Найдите в замке барона Дитриха моего командира, стражника Лансета, он знает все о крепостях, о том как их захватывать и защищать.");
            }

            if ( ! $id) {            // основной диалог
                $stmp = "<p>Что прикажете, сир?";
                $stmp .= "<br/><a href=\"#move\">Иди...</a>";
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=close\">Запереть ворота</a>";
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=new\">Пропустить гостя...</a>";
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=list\">Список гостей</a>";
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=status\">Статус?</a>";
                $stmp .= "\n</p>\n</card>\n<card id=\"move\" title=\"Иди...\">\n<p>Куда?";
                if (substr($loc, 3) == ".gate") {
                    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=move&to=tron\">В тронный зал</a>\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=move&to=sklad\">В кладовую</a>\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=move&to=main\">Общий зал</a>";
                } else {
                    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=move&to=gate\">К воротам</a>";
                }
            }

            if ($id == 'new') {    // список кого пропустить
                $stmp = "";
                $in   = substr($loc, 0, 4) . "in";
                loadloc($in);
                if ($loc_i[$in]) {
                    foreach (array_keys($loc_i[$in]) as $i) {
                        if (substr($i, 0, 2) == 'u.') {
                            $tto = explode("|", $loc_i[$in][$i]["char"]);
                            $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=add&who=" . $i . "\">" . $tto[0] . "</a>";
                        }
                    }
                }
                if ($stmp) {
                    $stmp = "<p>Кого из ожидающих у ворот замка отныне считать гостем?" . $stmp;
                } else {
                    $stmp = "<p>У ворот замка никого нет";
                }
            }
            if ($id == 'add' && ($who = Request('who'))) { // добавить гостя $who
                if (strpos($loc_tt[$gate]["d"], ":" . $who . ":") !== false) {
                    msg(substr($who, 2) . " и так является гостем этого замка, сир.");
                }
                $d[0] .= ":" . $who . ":";
                $loc_tt[$gate]["d"]    = implode("|", $d);
                $loc_tt[$gate]["save"] = 1;
                $stmp .= substr($who, 2) .
                    " с этого момента является гостем этого замка и может беспрепятственно входить и выходить из него.";
            }
            if ($id == 'list') {    // список гостей
                $stmp = "<p>Кого больше не считать гостем замка?";
                $xF   = preg_match_all("':([^:]+):'", $d[0], $regF);
                for ($i = 0; $i < $xF; $i++) {
                    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=del&who=" . $regF[1][$i] . "\">" .
                        substr($regF[1][$i], 2) . "</a>";
                }
                if ($xF == 0) {
                    $stmp = "<p>Сожалею, сир, но список гостей замка пуст.";
                }
            }

            if ($id == 'del' && ($who= Request('who'))) { // удалить гостя $who
                if (strpos($loc_tt[$gate]["d"], ":" . $who . ":") === false) {
                    msg(substr($who, 2) . " не является гостем этого замка, сир.");
                }
                $d[0]                  = str_replace(":" . $who . ":", "", $d[0]);
                $loc_tt[$gate]["d"]    = implode("|", $d);
                $loc_tt[$gate]["save"] = 1;
                $stmp .= $tchar[0] .
                    " с этого момента больше не является гостем замка, если попытается проникнуть внутрь, будет немедленно атакован стражниками.";
            }

            if ($id == 'status') {    // инфо когда покинет
                $owner = explode("|", $loc_i[$loc][$speak]["owner"]);
                $stmp .= "Я буду на службе до " . date("d/m Y H:i", $owner[3]) . ", вы хотите продлить контракт?";
                if (substr($speak, 0, 11) == "n.o.castle1") {
                    $tc = 50;
                }
                if (substr($speak, 0, 11) == "n.o.castle2") {
                    $tc = 60;
                }
                if (substr($speak, 0, 11) == "n.o.castle3") {
                    $tc = 70;
                }
                if (substr($speak, 0, 11) == "n.o.castle4") {
                    $tc = 100;
                }
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=cont&val=1\">1 день - " . $tc . " монет</a>";
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=cont&val=3\">3 дня - " . (3 * $tc) . " монет</a>";
                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=cont&val=5\">5 дней - " . (5 * $tc) . " монет</a>";
            }

            if ($id == 'cont' && ($val = Request('val'))) {
                // продление контракта, в зав. от типа наемника
                if ($val != 1 && $val != 3 && $val != 5) {
                    msg("Не понял, на какой срок вы хотите продлить контракт?");
                }
                $owner = explode("|", $loc_i[$loc][$speak]["owner"]);
                if (substr($speak, 0, 11) == "n.o.castle1") {
                    $tc = 50;
                }
                if (substr($speak, 0, 11) == "n.o.castle2") {
                    $tc = 60;
                }
                if (substr($speak, 0, 11) == "n.o.castle3") {
                    $tc = 70;
                }
                if (substr($speak, 0, 11) == "n.o.castle4") {
                    $tc = 100;
                }
                manageItems($loc, $login, "", "i.money", $tc * $val, "items", "items", 1, 0, 1);
                $owner[3] += (int)$val * 60 * 60 * 24;
                $loc_i[$loc][$speak]["owner"] = implode("|", $owner);
                $stmp .= "Контракт продлен на " . $val .
                    " дней. Клянусь защищать этот замок от любого, кто посмеет оспаривать ваши права на него, будь то человек или зверь.";
            }

            if ($id == 'move' && $to) {    // идти
                if ( ! isset($loc_tt[substr($loc, 0, 4) . $to])) {
                    msg("Сожалею, сир, но в вашем замке нет такого места");
                }
                $count = 0;
                if ($loc_i[substr($loc, 0, 4) . $to]) {
                    foreach (array_keys($loc_i[substr($loc, 0, 4) . $to]) as $i) {
                        if (substr($i, 0, 4) == "n.o.") {
                            $count++;
                        }
                    }
                }
                if ($count >= 5) {
                    msg("Сожалею, сир, но там уже полно стражников, во время боя мы будем только мешать друг другу.");
                }
                addjournal($loc, "all", $tchar[0] . " говорит: слушаюсь, сир");
                manageNPC($speak, $loc, substr($loc, 0, 4) . $to);
                $stmp = "";
            }

            if ($id == 'close') {    // запереть ворота
                if (strpos($d[0], "#") === false) {
                    $timez = 0;
                } else {
                    $timez = substr($d[0], strpos($d[0], "#") + 1, strrpos($d[0], "#") - strpos($d[0], "#") - 1);
                }
                if ($rclose = Request('rclose')) {
                    if (time() < $timez) {
                        msg("Сожалею, мы не можем закрыть ворота до " . date("d/m H:i", $timez) .
                            ", так как их открыли совсем недавно и рычаги еще не возвращены в исходное состояние");
                    }
                    // проверим, нет ли врагов
                    $arr = ["main", "gate", "tron", "sklad"];
                    $tc  = substr($loc, 0, 4);
                    if (strpos($d[0], "*") === false) {
                        $clanc = "";
                    } else {
                        $clanc = substr($d[0], strpos($d[0], "*") + 1, strrpos($d[0], "*") - strpos($d[0], "*") - 1);
                    }
                    $b = 0;
                    foreach ($arr as $i) {
                        loadloc($tc . $i);
                        if ($loc_i[$tc . $i]) {
                            foreach (array_keys($loc_i[$tc . $i]) as $k) {
                                if ($k != $login && substr($k, 0, 2) == "u.") {
                                    $tchar = explode("|", $loc_i[$tc . $i][$k]["char"]);
                                    if (strpos($tchar[0], "*") === false) {
                                        $tclan = "";
                                    } else {
                                        $tclan = substr($tchar[0], strpos($tchar[0], "*") + 1,
                                            strrpos($tchar[0], "*") - strpos($tchar[0], "*") - 1);
                                    }
                                    if ( ! $tchar[8] && $tclan != $clanc && strpos($d[0], ":" . $k . ":") === false) {
                                        $b = 1;
                                    }
                                }
                            }
                        }
                    }
                    if ($b) {
                        msg("В замке враги, мы не можем отвлекаться на ворота, вначале надо завершить бой");
                    }
                    // ок, запираем на 10 часов
                    if (strpos($d[0], "{") !== false) {
                        $d[0] = substr($d[0], 0, strpos($d[0], "{")) . substr($d[0], strpos($d[0], "}") + 1);
                    }
                    if (strpos($d[0], "#") !== false) {
                        $d[0] = substr($d[0], 0, strpos($d[0], "#")) . substr($d[0], strrpos($d[0], "#") + 1);
                    }
                    $d[0] .= "{" . (time() + 60 * 60 * 10) . "}";
                    $loc_tt[$gate]["d"]    = implode("|", $d);
                    $loc_tt[$gate]["save"] = 1;
                    addjournal($loc, $login, "Вы заперли ворота замка на ближайшие 10 часов");
                } else {
                    $stmp = "Вы действительно хотите закрыть ворота замка на ближайшие 10 часов?<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=close&rclose=1\">Да, опускайте решетку прямо сейчас!</a><br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak\">Нет, я передумал</a><br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=iclose\">Расскажи подробнее об этом</a>";
                }
            }

            if ($id == 'iclose') {
                $stmp = "Мы можем сейчас закрыть ворота на ближайшие 10 часов и никто из посторонних не попадет внутрь замка, но если придет кто-то из хозяев замка и откроет ворота, либо истекут эти 10 часов, то ворота нельзя будет закрыть в течении следующих 8 часов.<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=iclose2\">Т.е. замок невозможно будет захватить?</a><br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=iclose3\">А если закрыть ворота во время боя?</a><br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=close\">Ясно</a>";
            }
            if ($id == 'iclose2') {
                $stmp = "Верно. Правда, есть небольшой нюанс - враги могут осаждать замок, пытаясь попасть внутрь даже при запертых воротах! Решетка их, конечно, остановит, но стража во внутреннем дворе будет вынуждена участвовать в бою, т.к. в нее могут стрелять сквозь решетку и навесом через стены замка. Поэтому если ты не хочешь напрасно терять людей, лучше прежде чем запереть ворота отведи их в дальние части замка, например, в тронный или общий зал.<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=iclose\">Я учту это, спасибо за совет</a>";
            }
            if ($id == 'iclose3') {
                $stmp = "Хе-хе, интересная мысль. К сожалению, если враги уже внутри самого замка, то наш первейший долг разобраться с ними, поэтому закрыть ворота окажется просто некому - все стражники будут заняты боем (или поиском затаившихся лазутчиков). Мы же не можем возиться с засовом, когда в нескольких шагах рядом убивают наших, верно?<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=close\">Верно</a><br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=iclose4\">А если мы отобьем первую атаку, но за воротами будут еще враги, то можем закрыть ворота, чтобы они не попали внутрь, а мы могли отсидеться в замке?</a>";
            }
            if ($id == 'iclose4') {
                $stmp = "Разумеется, для того ворота и существуют, чтобы по тревоге их можно было закрыть! Если ваши враги настолько глупы, что не могут организовать массовый штурм, а будут посылать по одному человеку, оповещая этим всю стражу, то это их проблемы. Учти только, что если решишь переждать в замке, то должен стоять на месте (не подходи к воротам, чтобы в суматохе осады не оказаться случайно за ними ;-))<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=iclose\">Ок, все понял</a>";
            }

            if (!empty($stmp)) {
                msg($stmp, $tchar[0]);
            }
        } elseif (!empty([$loc][$speak]["owner"])) {
            // со своим
            $owner = explode("|", $loc_i[$loc][$speak]["owner"]);
            if ($owner[0] == $login) {
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
                                $new = Transliterate::convert($new);
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
                            manageNPC($speak, $loc, $owner[6]);
                        } else {
                            if ($owner[4]) {
                                // remove NPC from location
                                manageNPC($speak, $loc);
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
                if (!isset($dialog[$id])) {
                    msg($tchar[0] . " торговать не будет");
                }
                $user_items = $loc_i[$loc][$login]["items"];
                if ($speak == "n.Natan" && strpos($user_items, "i.q.hagen:") === false &&
                    strpos($user_items, "i.q.ditrih:") === false
                ) {
                    msg("Прости, но я продаю палладинское оружие и броню только стражникам барона Дитриха или тем кто имеет грамоту от Лорда Хагена");
                }
                $trader = explode("|", $dialog[$id]);

                switch (empty($to) ? $id : $id . "to"){
                    case 'buy':
                        require 'f_speakbuy.inc';
                        break;
                    case 'buyto':
                        require 'f_speakbuyto.inc';
                        break;
                    case 'sell':
                        require 'f_speaksell.inc';
                        break;
                    case 'sellto':
                        require 'f_speaksellto.inc';
                        break;
                }
            }

            // разговор с банкиром
            if ($id == 'tobank' || $id == 'frombank') {
                if (!isset($dialog["bank"])) {
                    msg($tchar[0] . " не банкир");
                }
                switch (empty($to) ? $id : $id . "to"){
                    case 'frombank':
                        require 'f_speakfrombank.inc';
                        break;
                    case 'frombankto':
                        require 'f_speakfrombankto.inc';
                        break;
                    case 'tobank':
                        require 'f_speaktobank.inc';
                        break;
                    case 'tobankto':
                        require 'f_speaktobankto.inc';
                        break;
                }
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
                // диалог повышения скилла или аттрибута на 1

                $tmp   = explode("|", $title);
                $skill = $tmp[1];
                $cost  = $tmp[2];
                $min   = $tmp[3];
                $max   = $tmp[4];

                $skills = explode("|", $loc_i[$loc][$login]["skills"]);
                if (substr($skill, 0, 2) == "m.") {
                    // магия
                    // проверки
                    if (strpos($loc_i[$loc][$login]["magic"], $skill . ":") !== false) {
                        msg("<p>У вас уже есть это заклинание", $tchar[0]);
                    }
                    if ($min) {
                        if ($skills[13] < $min) {
                            msg("<p>У вас недостаточный навык магии (надо минимум " . $min . ")", $tchar[0]);
                        }
                    }
                    if ($max) {
                        if ($skills[13] > $max) {
                            msg("<p>У вас слишком высокий навык магии (максимум " . $max . ")", $tchar[0]);
                        }
                    }
                    if ($cost) {
                        $money = intval(preg_replace('/.*i\.money:(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
                        if ($money < $cost) {
                            msg("<p>У вас недостаточно денег (надо " . $cost . " монет)", $tchar[0]);
                        }
                        manageItems($loc, $login, "", "i.money", $cost, "items", "items", 1, -1, 1);
                    }
                    if ( ! $loc_i[$loc][$login]["magic"]) {
                        $loc_i[$loc][$login]["magic"] = $skill . ":1";
                    } else {
                        $loc_i[$loc][$login]["magic"] .= "|" . $skill . ":1";
                    }
                    msg("<p>Вы выучили новое заклинание!", $tchar[0]);
                }
                if (substr($skill, 0, 2) == "p.") {
                    // приемы
                    // проверки
                    if (strpos($loc_i[$loc][$login]["priem"], $skill . ":") !== false) {
                        msg("<p>Вы уже знаете этот прием", $tchar[0]);
                    }
                    if ($cost) {
                        $money = intval(preg_replace('/.*i\.money:(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
                        if ($money < $cost) {
                            msg("<p>У вас недостаточно денег (надо " . $cost . " монет)", $tchar[0]);
                        }
                        manageItems($loc, $login, "", "i.money", $cost, "items", "items", 1, -1, 1);
                    }
                    if ( ! $loc_i[$loc][$login]["priem"]) {
                        $loc_i[$loc][$login]["priem"] = $skill . ":1";
                    } else {
                        $loc_i[$loc][$login]["priem"] .= "|" . $skill . ":1";
                    }
                    msg("<p>Вы выучили новый прием!", $tchar[0]);
                }

                $lev = 0;
                for ($i = 0; $i < count($skills); $i++) {
                    if ($i != 3) {
                        $lev += $skills[$i];
                    }
                }

                $arr_skills = [
                    "str"          => 0,
                    "dex"          => 1,
                    "int"          => 2,
                    "meditation"   => 5,
                    "steal"        => 6,
                    "animaltaming" => 7,
                    "hand"         => 8,
                    "coldweapon"   => 9,
                    "ranged"       => 10,
                    "parring"      => 11,
                    "uklon"        => 12,
                    "magic"        => 13,
                    "magic_resist" => 14,
                    "magic_uklon"  => 15,
                    "regeneration" => 16,
                    "hiding"       => 17,
                    "look"         => 18,
                    "steallook"    => 19,
                    "animallore"   => 20,
                    "spirit"       => 21,
                    "healing"      => 22,
                    "alchemy"      => 23,
                    "mine"         => 24,
                    "smith"        => 25,
                    "lumb"         => 26,
                    "bow"          => 27,
                    "stone"        => 28,
                    "fish"         => 29,
                    "food"         => 30,
                    "necro"        => 31,
                    "currier"      => 32
                ];

                $arr_title = [
                    "str"          => "Сила",
                    "dex"          => "Ловкость",
                    "int"          => "Интеллект",
                    "meditation"   => "Медитация",
                    "steal"        => "Кража",
                    "animaltaming" => "Прир.животных",
                    "hand"         => "Рукопашная",
                    "coldweapon"   => "Холодн.оружие",
                    "ranged"       => "Стрельба",
                    "parring"      => "Парирование",
                    "uklon"        => "Уклон",
                    "magic"        => "Магия",
                    "magic_resist" => "Сопр.магии",
                    "magic_uklon"  => "Уклон от магии",
                    "regeneration" => "Регенерация",
                    "hiding"       => "Скрытность",
                    "look"         => "Осторожность",
                    "steallook"    => "Подглядывание",
                    "animallore"   => "Изуч.животных",
                    "spirit"       => "Спиритизм",
                    "healing"      => "Лечение",
                    "alchemy"      => "Алхимия",
                    "mine"         => "Рудокоп",
                    "smith"        => "Кузнец",
                    "lumb"         => "Лесоруб",
                    "bow"          => "Плотник",
                    "stone"        => "Ювелир",
                    "fish"         => "Рыболов",
                    "food"         => "Повар",
                    "necro"        => "Некромант",
                    "currier"      => "друид",
                ];

                // проверки
                if ( ! isset($arr_skills[$skill])) {
                    msg("<p>Несуществующий скилл: $skill в диалоге с $speak, сообщите разработчику игры.", "Ошибка");
                }
                if ($skills[4] < 1) {
                    msg("<p>Недостаточно очков опыта", $tchar[0]);
                }
                if ($min) {
                    if ($skills[$arr_skills[$skill]] < $min) {
                        msg("<p>Вы должны иметь уровень навыка не ниже " . $min, $tchar[0]);
                    }
                }
                if ($max) {
                    if ($skills[$arr_skills[$skill]] > $max) {
                        msg("<p>Вы и так достаточно опытны, я учу только до уровня " . ($max + 1), $tchar[0]);
                    }
                }

                // надо ли понизить
                $skilldown = Request('skilldown');
                if ($skill == 'str' || $skill == 'dex' || $skill == 'int') {    //АТТР
                    if ($skills[$arr_skills[$skill]] >= $g_attr_one) {
                        msg("<p>Невозможно повысить, т.к. аттрибут уже на максимальном уровне " . $g_attr_one, $tchar[0]);
                    }
                    if ($skilldown && $skilldown != "str" && $skilldown != "dex" && $skilldown != "int") {
                        msg("Неверный аттрибут");
                    }
                    if ($skilldown && $skills[$arr_skills[$skilldown]] <= 1) {
                        msg("<p>Невозможно понизить, т.к. аттрибут уже на минимальном уровне 1, выберите другой", $tchar[0]);
                    }
                    if ( ! $skilldown) {
                        if ($skills[0] + $skills[1] + $skills[2] >= $g_attr) {
                            $stmp = "<p>Превышен предел суммы очков (" . $g_attr . ") для аттрибутов, выберите что уменьшить: ";
                            if ($skill != 'str') {
                                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=str\">Сила: " . $skills[0] .
                                    "</a>";
                            }
                            if ($skill != 'dex') {
                                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=dex\">Ловкость: " .
                                    $skills[1] . "</a>";
                            }
                            if ($skill != 'int') {
                                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=int\">Интеллект: " .
                                    $skills[2] . "</a>";
                            }
                            msg($stmp, $tchar[0]);
                        }
                    }
                } else {    //НАВЫКИ
                    if ($skills[$arr_skills[$skill]] >= $g_skills_one) {
                        msg("<p>Невозможно повысить, т.к. навык уже на максимальном уровне " . $g_skills_one);
                    }
                    if ($skilldown && ($skilldown == "str" || $skilldown == "dex" || $skilldown == "int")) {
                        msg("Неверный навык");
                    }
                    if ($skilldown && $skills[$arr_skills[$skilldown]] <= 0) {
                        msg("<p>Невозможно понизить, т.к. навык уже на минимальном уровне 0, выберите другой");
                    }
                    $sum = 0;
                    foreach (array_keys($arr_skills) as $i) {
                        if ($i != 'str' && $i != 'dex' && $i != 'int') {
                            $sum += $skills[$arr_skills[$i]];
                        }
                    }
                    if ( ! $skilldown) {
                        if ($sum >= $g_skills) {
                            $stmp = "<p>Превышен предел суммы очков (" . $g_skills . ") для навыков, выберите что уменьшить: ";
                            if ( ! $start) {
                                $start = 0;
                            }
                            $keys = array_keys($arr_skills);
                            $listEnd = $start + ViewOptions::getInstance()->getMaxListSize();
                            for ($i = $start; $i < $listEnd && $i < count($keys); $i++) {
                                if ($keys[$i] != 'str' && $keys[$i] != 'dex' && $keys[$i] != 'int') {
                                    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&skilldown=" . $keys[$i] . "\">" .
                                        $arr_title[$keys[$i]] . ": " . $skills[$arr_skills[$keys[$i]]] . "</a>";
                                }
                            }
                            if (count($keys) > 1 && $start) {
                                $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id\">^ </a>";
                            }
                            if ($listEnd < count($keys)) {
                                if ( ! $start) {
                                    $stmp .= "\n<br/>";
                                }
                                $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=" . $listEnd . "\">+ (" .
                                    (count($keys) - $listEnd) . ")</a>";
                            }
                            msg($stmp, $tchar[0]);
                        }
                    }
                }

                // хватит ли денег
                if ($cost) {
                    $twar = explode("|", $loc_i[$loc][$login]["war"]);
                    if ($twar[13] == 0) {
                        calcparam($loc, $login);
                    }
                    if ($lev == 5) {
                        $stmp = "<p>Ладно, так уж и быть, раз ты новичок, то я это сделаю бесплатно.</p>";
                    } else {
                        $money = intval(preg_replace('/.*i\.money:(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
                        if ($money < $cost) {
                            msg("<p>У вас недостаточно денег (надо " . $cost . " монет)");
                        }
                        manageItems($loc, $login, "", "i.money", $cost, "items", "items", 1, -1, 1);
                    }
                }

                // повышаем +1
                $skills[$arr_skills[$skill]] += 1;
                if ($skilldown) {
                    $skills[$arr_skills[$skilldown]] -= 1;
                }
                $skills[4] -= 1;    // очки опыта
                $loc_i[$loc][$login]["skills"] = implode("|", $skills);
                calcparam($loc, $login);
                $stmp .= "<p>" . $arr_title[$skill] . ": +1";
                if ($skilldown) {
                    $stmp .= "<br/>" . $arr_title[$skilldown] . ": -1";
                }
                msg($stmp, $tchar[0]);
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
