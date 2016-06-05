<?php
require_once('modules/globals.php');

/**
 * Получить значение по ключу
 * возвращает false, если ключа в массиве нет
 * @param $arr array Массив
 * @param $key mixed Ключ
 * @return mixed
 */
function get($arr, $key)
{
    return (
    array_key_exists($key, $arr) ?
        $arr[$key] : false
    );
}

/** Запись состояния
 *
 */
function savegame()
{
    global $game, $file_save, $login, $loc, $loc_i, $loc_t, $loc_tt;
    @ignore_user_abort(true);
    @set_time_limit(30);
    if ($file_save) {
        // сохраняем измененные локации
        foreach ($loc_tt as $i => $val) {
            $arr = array();
            $arr["d"] = $loc_tt[$i]["d"];
            if (count($loc_i[$i]) > 0)
                $arr["i"] = $loc_i[$i];
            if (count($loc_t[$i]) > 0)
                $arr["t"] = $loc_t[$i];
            if ($arr != $loc_tt[$i] && $arr["d"]) {
                $file = fopen("loc_i/" . $i, "w");
                if ($file !== false) {
                    fputs($file, serialize($arr));
                    fclose($file);
                }
            }
        }
        // сохраняем локацию пользователя
        if ($login && $loc && isset($loc_i[$loc][$login])) {
            $file = fopen("online/" . $login, "w");
            if ($file !== false) {
                fputs($file, $loc . "\n" . time());
                fclose($file);
            }
        }
        // обновление глобального состояния
        rewind($file_save);
        ftruncate($file_save, 0);
        // обновить кому принадлежит флаг
        if ($login && $game["fid"] == $login)
            $game["floc"] = $loc;
        fputs($file_save, serialize($game));
        //flock($file_save,3);
        fclose($file_save);
    };
    @ignore_user_abort(false);
}

/**
 * @brief Отправка сообщения
 * @param string $loc локация
 * @param string $to  кому сообщение. all - отправка всем
 * @param string $msg сообщение
 * @param string $no1 исключение из доставки
 * @param string $no2 исключение из доставки
 * @param string $cont разделитель
 */
function addjournal($loc, $to, $msg, $no1 = "", $no2 = "", $cont = "|")
{
    global $loc_i, $login;
    if (!$loc_i[$loc])
        return;
    $msg = preg_replace('/ \*.*?\*/', '', $msg);
    foreach ($loc_i[$loc] as $i => $val)
        if (substr($i, 0, 2) == "u." && ($i == $to || $to == "all") && $i != $no1 && $i != $no2) {
            $loc_i[$loc][$i]["journal"] .= $cont . $msg;
            if (strlen($loc_i[$loc][$i]["journal"]) > 800)
                $loc_i[$loc][$i]["journal"] = ".." . substr($loc_i[$loc][$i]["journal"], -800);
        }
}

/** Выводит страницу пользователю
 * Требует писец как много глобальных значений
 * Вызывает savegame
 * Завершает выполнение скрипта(exit в конце)
 * @param string $msg Сообщение
 * @param string $title название игры
 * @param int $journal флаг, что нужно выводить журнал
 * @param string $menu тип меню. возможные значения (""|none|main|to)
 * @param string $vname имя переменной для вставки в страницу
 * @param string $vval значение переменной vname
 */
function msg($msg, $title = 'Амулет Дракона', $journal = 1, $menu = '', $vname = '', $vval =
'')
{ //menu=""|none|main|to
    global $game, $g_admin, $gm, $login, $loc, $loc_i, $loc_tt, $page_d, $PHP_SELF, $sid,
           $gm_id, $g_size, $g_list, $fskipj, $cj, $t_g, $g_menu, $fm, $fm2, $g_j2loc, $g_joff, $g_smf,
           $cnick, $g_map, $g_tmp, $fmn, $g_ch, $cnick, $starttime;

    $wml = "<wml>";
    if (!$login) {
        $journal = 0;
        $menu = "none";
    }
    if ($fm) {
        $journal = 0;
        $page_d = 0;
        $msg = "Нажмите кнопку Меню";
    }
    if ($fm2) {
        $journal = 0;
        $page_d = 0;
        require("f_menu.dat");
    }
    if ($cj)
        $fskipj = 1;
    if ($fskipj)
        $journal = 0;
    else
        if ($g_joff)
            $loc_i[$loc][$login]["journal"] = preg_replace('/(\||^)[^:!]*(\||$)/', "|", $loc_i[$loc][$login]["journal"]);
    $page_j = "";
    if (get($game, "journal") && $login != $g_admin && $gm != $gm_id)
        $page_j = $game["journal"];
    if ($journal == 1 && $loc_i[$loc][$login]["journal"]) {
        $page_j = str_replace("|", "<br/>", $loc_i[$loc][$login]["journal"]);
        $loc_i[$loc][$login]["journal"] = "";
        if (!$g_j2loc)
            $page_j = preg_replace('/<br\/>(Пришел|Пришла) [^<]+/', "", $page_j);
    }

    $t_g1 = sscanf(microtime(), "%s %s");
    $t_g1 = $t_g1[1] + $t_g1[0];
    $game["tmid"] = ($game["tmid"] + $t_g1 - $t_g) / 2;
    savegame();
    if ($page_j && $journal) {
        $page_j = explode("<br/>", $page_j);
        if (count($page_j) > $g_list)
            array_splice($page_j, 0, count($page_j) - $g_list);
        $page_j = implode("<br/>", $page_j);
    }
    if ($page_d && file_exists("loc_f/" . $loc))
        $page_j .= "<br/>" . implode("", file("loc_f/" . $loc));
    strlen($wml . $msg . $page_j) < $g_size ? $bsize = 1 : $bsize = 0;
    if ($page_j && substr($page_j, 0, 5) == "<br/>")
        $page_j = substr($page_j, 5);
    if ($page_j && $journal) {
        if ($bsize)
            $tu = "#g";
        else {
            $tu = "$PHP_SELF?" . preg_replace("/(ci|use|say|ca|drop|take|to|adm|cm|go)=/", "c1=", $g_tmp);
        }
        $wml .= "<card title=\"Журнал\"><do type=\"accept\" label=\"Дальше\"><go href=\"" . $tu .
            "\"/></do><p>" . $page_j . "<br/><a href=\"" . $tu . "\">Дальше</a>";
        if ($tu != "#g")
            $wml .= "/<a href=\"" . $tu . ($fskipj ? "" : "&cj=1") . "\">к меню</a>/";
        $wml .= "</p></card>";
    }

    if ($bsize || !$journal || !$page_j) {
        $wml .= "<card id=\"g\" title=\"" . $title . "\"";
        if ($menu == 'main')
            $wml .= " ontimer=\"$PHP_SELF?sid=$sid\"><timer value=\"600\"/";
        $wml .= ">";
        if ($vname)
            $wml .= "<onevent type=\"onenterforward\"><refresh><setvar name=\"$vname\" value=\"$vval\"/></refresh></onevent>";

        if ($menu == '' || $menu == 'inv' && $g_menu != 1) {
            $wml .= "<do name=\"b1\" type=\"options\" label=\"В игру\"><go href=\"$PHP_SELF?sid=$sid\"/></do>";
            $wml .= "<do name=\"b2\" type=\"accept\" label=\"Назад\"><prev/></do>";
        }
        $o = 4;
        if ($menu == 'main' && $g_menu == 2 && !$fm) {
            $wml .= "<do name=\"o2\" type=\"options\" label=\"Меню\"><go href=\"$PHP_SELF?sid=$sid&fm=1&cj=1\"/></do>";
            $menu = '';
        }
        if ($menu == 'main' && $g_menu == 3 && !$fm2) {
            $msg = str_replace("</p></card><card id=\"m\"", "<br/><a href=\"$PHP_SELF?sid=$sid&fm2=1&cj=1\">Меню</a></p></card><card id=\"m\"",
                $msg);
            $menu = '';
        }
        if (($menu == 'main' || $fm) && !$fm2) {
            $wml .= "<do name=\"b1\" type=\"options\" label=\"Пeрcoнaж\"><go href=\"$PHP_SELF?sid=$sid&cl=i&cj=1\"/></do>";
            if (!isset($loc_i[$loc][$login]["macro"]))
                $m = array();
            else
                $m = explode("/", $loc_i[$loc][$login]["macro"]);
            for ($i = 1; $i < 9; $i++)
                if ($m[$i - 1]) {
                    $mn = explode("|", $m[$i - 1]);
                    $wml .= "<do name=\"b$o\" type=\"options\" label=\"" . $mn[4] . "\"><go href=\"$PHP_SELF?sid=$sid&cm=$i\"/></do>";
                    $o++;
                }
        }
        if ($menu == 'inv' && $g_menu == 1)
            $wml .= "<do name=\"b1\" type=\"options\" label=\"В игру\"><go href=\"$PHP_SELF?sid=$sid\"/></do>";
        if ($menu == 'inv' && $g_menu == 1 || $g_menu == 0 && $menu == 'main' || $fm) {
            $wml .= "<do name=\"b2\" type=\"options\" label=\"Cкaзaть\"><go href=\"$PHP_SELF?sid=$sid&cs=1&cj=1\"/></do>";
            $wml .= "<do name=\"b3\" type=\"options\" label=\"Koнтaкты\"><go href=\"$PHP_SELF?sid=$sid&msg=1&cj=1\"/></do>";
            $wml .= "<do name=\"b$o\" type=\"options\" label=\"мaкpocы\"><go href=\"$PHP_SELF?sid=$sid&cm=new\"/></do>";
            if ($g_map) {
                $o++;
                $wml .= "<do name=\"b$o\" type=\"options\" label=\"Kapтa\"><go href=\"$PHP_SELF?sid=$sid&map=" .
                    $g_map . "\"/></do>";
            }
            $o++;
            $wml .= "<do name=\"b$o\" type=\"options\" label=\"Coxpaнить\"><go href=\"$PHP_SELF?sid=$sid&ce=1\"/></do>";
        }

        if (substr($msg, 0, 2) != "<p")
            $msg = "<p>" . $msg;
        if (substr($msg, strlen($msg) - 4) != "</p>")
            $msg .= "</p>";
        $wml .= "" . $msg . "</card>";
    };
    if ($g_smf && strpos($wml, "<input") === false)
        $wml = preg_replace(array("'(<p [^>]*>)'", "'<p>'", "'</p>'"), array("\\1<small>",
            "<p><small>", "</small></p>"), $wml);

    $wml .= "</wml>";
    $wml = str_replace("&amp;", "&", $wml);
    $wml = str_replace("&", "&amp;", $wml);
    //$wml = strtr( $wml, "КЕНХВАРОСМТехарос", "KEHXBAPOCMTexapoc" );

    echo $wml;
    exit;
}

/**"ИИ". Обновление состояния в локации
 * @param string $i локация
 */
function doai($i)
{ // искусственный интеллект, проверяем локацию с именем $i
    global $game, $loc, $loc_i, $loc_t, $loc_tt, $g_list, $start, $g_destroy, $g_crim, $g_logout,
           $login;
    $g_regen = 30;

    $locai = explode("|", $loc_tt[$i]["d"]);

    // таймеры
    if (isset($loc_t[$i])) {
        foreach ($loc_t[$i] as $j => $val)
            if (time() > $j) {
                if (gettype($loc_t[$i][$j]) == "array" || substr($loc_t[$i][$j], 0, 2) == "n.") {
                    require "f_timernpc.inc";
                    continue;
                }
                if (substr($loc_t[$i][$j], 0, 2) == "i.") {
                    require "f_timeritem.inc";
                    continue;
                }
                $loct = $i;
                $curr = $j;
                eval($loc_t[$i][$j]);
            }
    }
    $crim = array();
    $users = array();
    $ti = explode("x", $i);
    if ($loc_i[$i]) {
        foreach ($loc_i[$i] as $j => $val)
            if ($j != "u.qv") {
                if (substr($j, 0, 2) == 'u.') {
                    $uc = explode("|", $loc_i[$i][$j]["char"]);
                    if (!$uc[8]) {
                        $us = explode("|", $loc_i[$i][$j]["skills"]);
                        if (rand(0, 100) > $us[17] * 6)
                            $users[] = $j;
                        if ($locai[1] != 3 && $uc[9] || $ti[2] >= 1099 && ($locai[1] == 2 && $uc[14] == "p" || $locai[1] ==
                                3 && $uc[14] == "t")
                        )
                            $crim[] = $j;
                    }
                }
                if (substr($j, 0, 4) == 'n.c.')
                    $crim[] = $j;
                if (substr($j, 0, 4) == "n.g.")
                    $guard = 1;
            }
    }
    if ($locai[1] == 1 && count($crim) > 0 && !$guard)
        require "f_addguard.inc";

    // по всем объектам
    if ($loc_i[$i])
        foreach ($loc_i[$i] as $j => $val)
            if (isset($loc_i[$i][$j])) {
                if (substr($j, 0, 2) == 'i.') {
                    if ($j == "i.flag" && $game["floc"] != $i) {
                        unset($loc_i[$i][$j]);
                        continue;
                    }
                    if (substr($i, 0, 2) == "c." && substr($j, 0, 4) != "i.s.")
                        continue;
                    $tmp = explode("|", $loc_i[$i][$j]);
                    if ($tmp[2] && time() > $tmp[2])
                        unset($loc_i[$i][$j]);
                    continue;
                }
                if (substr($j, 0, 2) == 'u.' || substr($j, 0, 2) == 'n.') {
                    $char = explode("|", $loc_i[$i][$j]["char"]);
                    $tm = time() - $char[5];
                    if ($tm > $g_regen && ($char[1] != $char[2] || $char[3] != $char[4]) && (substr($j, 0,
                                2) == 'n.' || (substr($j, 0, 2) == 'u.' && !$char[8]))
                    ) {
                        if (substr($j, 0, 2) == 'u.')
                            $skills = explode("|", $loc_i[$i][$j]["skills"]);
                        else {
                            $skills[5] = 0;
                            $skills[16] = 0;
                        }
                        $char[1] = min($char[1] += round($tm / ($g_regen - $skills[16] * 4)), $char[2]);
                        $char[3] = min($char[3] += round($tm / ($g_regen - $skills[5] * 4)), $char[4]);
                        $char[5] = time();
                    }
                    if (substr($j, 0, 2) == 'u.') {
                        if ($char[9] && time() > $char[10]) {
                            $char[9] = 0;
                            $char[10] = "";
                        }
                        if ($j == $login)
                            $char[11] = time();
                        if ($char[11] && time() > $char[11] + $g_logout * 5 && !file_exists("online/" . $j)) {
                            unset($loc_i[$i][$j]);
                            continue;
                        }
                    }
                    if (substr($j, 0, 2) == 'n.') {
                        if ($loc == $i && time() > $char[6] && $char[1] < $char[2] / 4 && rand(0, 100) < 50 &&
                            substr($j, 0, 4) != 'n.s.' && substr($j, 0, 4) != 'n.o.' && substr($j, 0, 4) !=
                            'n.z.'
                        ) {
                            require "f_run.dat";
                            if ($b)
                                continue;
                        }
                        if ($char[7] && isset($loc_i[$i][$char[7]]) && substr($char[7], 0, 2) == "u.") {
                            $tc = explode("|", $loc_i[$i][$char[7]]["char"]);
                            if ($tc[8])
                                $char[7] = "";
                        }
                        if ($j == "n.a.b.jarpt.1") {
                            $b = 0;
                            foreach ($loc_i[$i] as $k => $v)
                                if (substr($k, 0, 2) == "u.") {
                                    addnpc($j, $i, $locai[2 + 2 * rand(0, (count($locai) - 2) / 2 - 1) + 1]);
                                    $b = 1;
                                    break;
                                }
                            if ($b)
                                continue;
                        }
                        if (substr($j, 0, 4) == "n.g." && time() > $char[11]) {
                            addnpc($j, $i, "");
                            continue;
                        }
                        if (isset($loc_i[$i][$j]["owner"])) {
                            require "f_owner.dat";
                            if ($b)
                                continue;
                        } else {
                            $owner[1] = "";
                        }
                        if ($char[7] && !$owner[1] && !isset($loc_i[$i][$char[7]])) {
                            $b = 0;
                            if (substr($j, 0, 4) != "n.o." && $j != "n.a.b.jarpt.1")
                                require "f_goto.inc";
                            if ($b)
                                continue;
                            else
                                $char[7] = "";
                        }
                        if (!$char[7]) {
                            if (count($crim) > 0 && (substr($j, 0, 4) == "n.g." || substr($j, 0, 4) == "n.t." ||
                                    substr($j, 0, 4) == "n.p.")
                            )
                                $char[7] = $crim[rand(0, count($crim) - 1)];
                            if (($char[9] || substr($j, 0, 4) == 'n.c.') && count($users) > 0)
                                $char[7] = $users[rand(0, count($users) - 1)];
                        }
                        if (substr($j, 0, 4) == "n.o." && substr($i, 0, 2) == "c." && substr($i, 3) !=
                            ".in" && (!$char[7] || !isset($loc_i[$i][$char[7]]))
                        )
                            require "f_no.dat";
                        if (!$char[7] && !$owner[1] && ($char[10] || (!$char[10] && $char[12])) && substr($j,
                                0, 4) != "n.o."
                        ) {
                            require "f_na.dat";
                            if ($b)
                                continue;
                        }
                    }
                    $loc_i[$i][$j]["char"] = implode("|", $char);
                    if ($char[7] && substr($j, 0, 2) != "u.")
                        attack($i, $j, $char[7]);
                } else {
                    unset($loc_i[$i][$j]);
                    continue;
                }
            }
}

/**
 * @param string $loc Локация
 * @param string $from кто атакует
 * @param string $to кого атакуют
 * @param string $magic тип магии(?)
 * @param int $answer флаг что атака в ответ(?)
 * @param int $rmagic хз
 * @param string $priem тип приема(?)
 * @param string $ptitle название приема(?)
 */
function attack($loc, $from, $to, $magic = '', $answer = 1, $rmagic = 0, $priem = "", $ptitle =
"")
{
//    global $attackf;
//    if (!$attackf)
//        $attackf = implode('', file("f_attackf.inc"));
//    eval($attackf);
    require 'f_attackf.inc';
}

/**Подгружает локацию $loc
 * ВНИМАНИЕ: изменяет глобальные переменные $loc_i, $loc_t, $loc_tt
 * @param string $loc
 */
function loadloc($loc)
{
    global $loc_i, $loc_t, $loc_tt;
    if ($loc == ".")
        return;
    if (!isset($loc_tt[$loc])) {
        if (!$loc || !file_exists("loc_i/" . $loc))
            return;
        $tmp = implode("", file("loc_i/" . $loc));
        $loc_tt[$loc] = unserialize($tmp);
        if (!$loc_tt[$loc]["d"]) {
            $tmp = preg_replace('/s:(?:\d+):"(.*?)";/e', "calcser('\\1')", $tmp);
            $loc_tt[$loc] = unserialize($tmp);
        }
        if (!$loc_tt[$loc]["d"])
            $loc_tt[$loc] = unserialize(implode("", file("loc_t/" . $loc)));
        if (!$loc_tt[$loc]["d"])
            die("err: loadloc($loc)");
        if (isset($loc_tt[$loc]["i"]))
            $loc_i[$loc] = $loc_tt[$loc]["i"];
        else
            $loc_i[$loc] = array();
        if (isset($loc_tt[$loc]["t"]))
            $loc_t[$loc] = $loc_tt[$loc]["t"];
    }
}

function addnpc($id, $from = "", $to = "", $gal = 0, $hide = 0)
{
    global $loc_i, $loc, $login, $page_d, $loc_tt, $g_j2go, $game;

    if ($from == $to)
        return;
    loadloc($from);
    loadloc($to);
    if ($from && $to && (!isset($loc_i[$from]) || !isset($loc_i[$to])))
        return;
    $ars = array("Появился", "исчез", "Пришел", "ушел", "прискакал", "поскакал", "пронесся");
    if (substr($id, 0, 2) == "u." && (strpos($loc_i[$from][$id]["user"], "|f|") !== false ||
            strpos($loc_i[$to][$id]["user"], "|f|") !== false)
    )
        $ars = array("Появилась", "исчезла", "Пришла", "ушла", "прискакала", "поскакала",
            "пронеслась");
    $tnpc = "";
    if ($from && isset($loc_i[$from][$id])) {
        $floc = explode("|", $loc_tt[$from]["d"]);
        $tnpc = $loc_i[$from][$id];
        $tchar = substr($tnpc["char"], 0, strpos($tnpc["char"], "|"));
        if (!$hide)
            if ($to && array_search($to, $floc)) {
                if ($gal && $gal != 1)
                    addjournal($from, "all", $tchar . " " . $ars[5] . " галопом " . $gal, $id);
                else
                    if (!$gal)
                        addjournal($from, "all", $tchar . " " . $ars[3] . " " . $floc[array_search($to, $floc) -
                            1], $id);
            } else
                addjournal($from, "all", $tchar . " " . $ars[1], $id);
        unset($loc_i[$from][$id]);
    }
    if ($to && isset($loc_i[$to])) {
        if (!$tnpc && isset($loc_i[$to][$id])) {
            $tnpc = $loc_i[$to][$id];
            $tchar = substr($tnpc["char"], 0, strpos($tnpc["char"], "|"));
        }
        if ($tnpc) {
            $tloc = explode("|", $loc_tt[$to]["d"]);
            if ($from && array_search($from, $tloc)) {
                if ($gal && $gal != 1)
                    addjournal($to, "all", $tchar . " " . $ars[6] . " галопом " . $gal, $id);
                else
                    if ($gal == 1)
                        addjournal($to, "all", $tchar . " " . $ars[4] . " галопом", $id);
                    else
                        addjournal($to, "all", $ars[2] . " " . $tchar, $id);
                if (substr($id, 0, 2) == "n.") { // история следов npc
                    $tchar = explode("|", $tnpc["char"]);
                    $steps = explode(":", $tchar[12]);
                    if (count($steps) == 0)
                        $steps[] = $from;
                    else {
                        if ($steps[count($steps) - 1] == $to)
                            unset($steps[count($steps) - 1]);
                        else
                            $steps[] = $from;
                    }
                    $tchar[12] = implode(":", $steps);
                    $tnpc["char"] = implode("|", $tchar);
                }
            } else
                addjournal($to, "all", $ars[0] . " " . $tchar, $id);
            $loc_i[$to][$id] = $tnpc;
            if ($from && substr($id, 0, 2) == "u.") {
                if ($floc[1] == 1 && $tloc[1] != 1)
                    addjournal($to, $id, "Вы покинули охраняемую территорию");
                if ($floc[1] != 1 && $tloc[1] == 1)
                    addjournal($to, $id, "Вы на охраняемой территории");
            }
        }
    }
    if ($id == $login && $to && isset($loc_i[$to][$id])) {
        $loc = $to;
        if ($g_j2go)
            $page_d = 1;
    }
}

/**Генерация случайного имени
 * @return string
 */
function rndname()
{
    require "f_rndname.inc";
    return $stmp;
}

/** Воскрешение персонажа
 * @param string $loc локация
 * @param string $to кого
 */
function ressurect($loc, $to)
{
    require "f_ressurect.inc";
}

/**Превращает персонажа в преступника
 * @param string $loc локация
 * @param string $login логин
 * @param string $title "звание"
 */
function docrim($loc, $login, $title = "преступник")
{
    require "f_docrim.inc";
}

/**Пересчет характеристик персонажа
 * @param string $loc локация
 * @param string $login персонаж
 */
function calcparam($loc, $login)
{
    if ($login != "u.qv")
        require "f_calcparam.inc";
}

/** Добавление вещи в инвентарь
 * @param string $loc локация
 * @param string $from от кого
 * @param string $to кому
 * @param string $item что
 * @param mixed $count количество либо 'count' для авторасчета
 * @param string $ft откуда брать items|bank
 * @param string $tt куда добавлять  items|bank
 * @param int $journal флаг занесения в журнал
 * @param int $time_delete время действия
 * @param int $msg флаг вывода сообщения
 * @return int флаг успеха(?)
 */
function additem($loc, $from, $to, $item, $count = 1, $ft = "items", $tt = "items", $journal =
1, $time_delete = -1, $msg = 0)
{
    require "f_additem.inc";
    return $res;
}

/**Добавление/обновление таймера
 * @param string $loct локация
 * @param int $curr текущий таймер
 * @param int $time период
 * @param string $text текст для установки или old для использования старого
 * @param int $delete флаг удаления текущего
 */
function addtimer($loct, $curr, $time, $text = "old", $delete = 1)
{
    require "f_addtimer.inc";
}

/**Добавляет экспу
 * @param string $loc локация
 * @param string $to логин
 * @param int $exp количество
 */
function addexp($loc, $to, $exp)
{
    require "f_addexp.inc";
}

/**Вспомогательная функция для перерасчета длины строк в файлах состояния
 * @param string $s строка для перерачета
 * @return string
 */
function calcser($s)
{
    return "s:" . strlen($s) . ":\"" . $s . "\";";
}