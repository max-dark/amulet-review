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
 * @global id
 * @global locai
 * @global tchar
 * @global g_list
 * @global trader
 * @global rclose
 * @global who
 * @global to
 */

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

if ($id == 'del' && $who) { // удалить гостя $who
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
    additem($loc, $login, "", "i.money", $tc * $val, "items", "items", 1, 0, 1);
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
    addnpc($speak, $loc, substr($loc, 0, 4) . $to);
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