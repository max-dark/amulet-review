<?php
/**
 * 'Движок' игры
 * Входная точка
 */

use MaxDark\Amulet\OldCode\MapPage;
use MaxDark\Amulet\OldCode\PageType;
use MaxDark\Amulet\OldCode\ViewOptions;

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
 * @global to
 * @global use
 * @global id
 */

require_once('config.php'); // настройки игры
require_once('game_function.php'); // игровые функции

const DEBUG = true;

$QUERY_STRING = get_value($_SERVER, 'QUERY_STRING');

$g_query_string = $QUERY_STRING;
$tmp = urldecode($QUERY_STRING);

/** @var array $loc_i предметы, НПС и игроки в загруженных локациях */
$loc_i = [];
/** @var array $loc_t таймеры в загруженных локациях */
$loc_t = [];
/** @var array $loc_tt загруженные локации */
$loc_tt = [];
/** @var array $game Массив с состоянием игры */
$game = [];

$gm = Request('gm');
// ИД пользователя
$sid = Request('sid');
/** @var string $site */
$site = Request('site');
$login = Request('login');
$p = Request('p');
$cnick = Request('cnick');
$go = Request('go');
$gal = Request('gal');
$ctele = Request('ctele');
$stele = Request('stele');
// "Атаковать"
$ca = Request('ca');
// "Сохранить"
$ce = Request('ce');
// "Инфо"
$ci = Request('ci');
// Макросы
$cm = Request('cm');
// Чат
$cs = Request('cs');

/* @var $cl string "command list"(?) == i|p|m */
$cl = Request('cl');
$id = Request('id');
// функции админки
$adm = Request('adm');
$use = Request('use');
$to = Request('to');
$list = Request('list');
$speak = Request('speak');
$start = Request('start');
$map = Request('map');
$take = Request('take');
$look = Request('look');
$trade = Request('trade');
$msg = Request('msg');
$say = Request('say');
$drop = Request('drop');
$pass = Request('pass');
$nn = Request('nn');
$num = Request('num');

// Если строка запроса пуста
if (
    empty($tmp) || // или не заданы логин и страница перехода
    (empty($sid) && empty($site))
) {
    // установить страницу перехода на форму логина
    $site = 'main';
}
/*
 * Если индификатор пользователя установлен
 * */
if ($sid) {
    if (substr($sid, 0, 2) != "u.") {
        // добавить префикс
        $sid = "u." . $sid;
    }
    $sid = explode(".", $sid);
    $sid[1] = strtolower($sid[1]);
    $login = "u." . $sid[1];
    if (empty($p)) {
        $p = $sid[2];
    }
    $sid = $sid[1] . "." . $p . "." . chr(rand(97, 122));
    if ($gm == $gm_id) {
        $sid .= "&gm=" . $gm_id;
    }
    // и этот пользователь в сети...
    if (file_exists("online/" . $login)) {
        usleep(300000); // задержка выполнения скрипта на 0.3 секунды
    }
}

// если есть сохраненное состояние
if (file_exists("data/game.dat")) {
    // загрузить состояние
    $file_save = fopen("data/game.dat", "r+");
    if (false === $file_save) {
        msg("Ошибка загрузки game.dat");
    }
    if (flock($file_save, LOCK_EX)) {
        rewind($file_save);
        $game = unserialize(
            fread($file_save, 65535)
        );
        if (!is_array($game)) {
            $game = [];
        }
    } else {
        $file_save = false;
        msg("Ошибка блокировки game.dat");
    }
} else {
    // создать файл состояния
    $file_save = fopen("data/game.dat", "w+");
    if (is_resource($file_save) && flock($file_save, LOCK_EX)) {
        // выгоняем всех из игры
        $f_all = 1;
        include_once "f_online.inc";
        // сброс состояния локаций
        include_once "f_blank.inc";
    } else {
        $file_save = false;
        msg("Ошибка создания game.dat");
    }
}
// если игра на обслуживании(?) и мы не админ
if (isset($game["msg"]) && $gm != $gm_id) {
    // вывести сообщение
    msg($game["msg"]);
}

if (!empty($site)) { // если задана страница перехода
    /** @var string[] $pages */
    $pages = [
        // форма входа. Так же служит для задания логина нового пользователя
        'main' => 'f_site_main.inc',
        // ссылки на  статистику
        'stat' => 'f_site_stat.inc',
        // инфо о флаге
        'flag' => 'f_site_flag.inc',
        // замки и их владельцы
        'castle' => 'f_site_castle.inc',
        // кланы и их участники
        'clans' => 'f_site_clans.inc',
        // вход в игру
        // TODO: упростить механизм входа
        'connect' => 'f_site_connect.inc',
        'connect2' => 'f_site_connect2.inc',
        // информация
        // ЧаВо
        'faq' => 'f_site_faq.inc',
        // новости
        'news' => 'f_site_news.inc',
        // список игроков онлайн
        'online' => 'f_site_online.inc',
        // регистрация в игре
        // форма ввода информации о новом игроке
        'gamereg' => 'f_site_gamereg.inc',
        // проверяет данные и записывает их в БД
        'reg2' => 'f_site_reg2.inc'
    ];
    if (array_key_exists($site, $pages)) {
        if (file_exists($pages[$site])) {
            // выполнить ее
            require_once $pages[$site];
        }
    }
    die('Oops: ' . $site . ' not found');
}

// Если время вышло
if (time() > $game["lastai"] + 240) {
    // проверка всех онлайн и удаление в оффлайн
    include_once "f_online.inc";
}
// пользователь оффлайн
if (!file_exists("online/" . $login)) {
    $f_c = 1;
    include_once "f_site_connect2.inc";
}
$tmp = file("online/" . $login);
$loc = trim($tmp[0]);
loadloc($loc);
if (!isset($loc_i[$loc][$login])) {
    @unlink("online/" . $login);
    msg("Нет данных");
}
// проверка пароля
$userData = $loc_i[$loc][$login]["user"];
if ($p != substr($userData, 0, strpos($userData, "|"))) {
    include_once("f_npass.inc");
}
unset($userData);

$viewOptions = get_value($loc_i[$loc][$login], "o");
if ($viewOptions) {
    ViewOptions::getInstance()->fromString($viewOptions);
}
unset($viewOptions);

if ($cnick) {
    // перейти к настройкам
    include_once "f_cnick.inc";
}
// переход в другую локу
if ($go) {
    if ($loc == "x927x253" && $go == "x902x254") {
        msg("Стражник: Стой!");
    }
    if ($go == "x1746x545" && strpos($loc_i[$loc][$login]["items"], "i.q.keykrep") === false) {
        msg("Без ключа не пройти");
    }
    if ($loc == "x216x1099" && $go == "x138x1380") {
        msg("между вами и входом в шахту встает грозный гном, с гиганским двухсторонним топором, на плече. Явно запрещая вам пройти в нутрь");
    }
    if ($loc == "x233x2330" && $go == "_begi" && strpos($loc_i[$loc][$login]["items"], "i.q.keykrep1") === false) {
        msg("А вы симпотично смотритесь ;)");
    }
    if ($loc == "x154x1540" && $go == "x155x1540") {
        msg("Ангел сверхестественной силой не дает вам двигаться дальше");
    }
    if ($game["fid"] == $login) {
        if (
            in_array($go, [
                "x393x1167",
                "x33x1252",
                "x435x1167",
                "x287x1252"
            ])
        ) {
            msg("С флагом на борт запрещено!");
        }
    }

    $loc_c = explode("|", $loc_tt[$loc]["d"]);
    $b = array_search($go, $loc_c);
    if ($b) {
        $tgo = $loc_c[$b - 1];
        loadloc($go);
        $b = 0;
        $char = explode("|", $loc_i[$loc][$login]["char"]);
        $skills = explode("|", $loc_i[$loc][$login]["skills"]);
        $hide = (rand(1, 100) <= $skills[17] * 8) ? 1 : 0;
        if ($gal && $char[12]) {
            $loc_c = explode("|", $loc_tt[$go]["d"]);
            $b = array_search($tgo, $loc_c);
        }
        if (!$b) {
            $tgo = "";
        }
        manageNPC($login, $loc, $go, $tgo, $hide);
        if ($b) {
            manageNPC($login, $loc, $loc_c[$b + 1], 1, $hide);
        }
    }
}

if ($game["fid"] == $login) {
    $game["floc"] = $loc;
}
if (
    !$game["floc"] || isset($loc_tt[$game["floc"]]) && !isset($loc_i[$game["floc"]]["i.flag"]) &&
    !isset($loc_i[$game["floc"]][$game["fid"]])
) {
    $loc_i[$loc]["i.flag"] = "флаг лидерства|1|0";
    $game["floc"] = $loc;
    $game["fid"] = "";
}
if ($ctele) {
    include_once "f_castle.inc";
}
if ($stele) {
    include_once "f_stele.inc";
}

// получаем список окружающих локаций
$loc_c = explode("|", $loc_tt[$loc]["d"]);
// подгружаем локи
for ($i = 2; $i < count($loc_c); $i += 2) {
    loadloc($loc_c[$i + 1]);
}
// запускаем обновление в текущей и соседних
doai($loc);
for ($i = 2; $i < count($loc_c); $i += 2) {
    doai($loc_c[$i + 1]);
}

if (!isset($loc_i[$loc][$login]) || !$login) {
    @unlink("online/" . $login);
    msg("Нет данных");
}
$char = explode("|", $loc_i[$loc][$login]["char"]);

// "сохранить"
if ($ce) {
    include_once "f_logout.inc";
}

// макросы
if ($cm) {
    // задан номер - выполнить
    if ($cm > 0 && $cm < 9) {
        $cm--;
        $m = getMacroList($loc_i[$loc][$login]);
        $m = explode("|", $m[$cm]);
        $ml = explode("|", $loc_i[$loc][$login]["macrol"]);

        // TODO: избавиться от eval
        if ($m[0] == "last" && $ml[0]) {
            eval('$' . $ml[0] . "=\"" . $ml[1] . "\";");
        } else {
            if ($m[0]) {
                eval('$' . $m[0] . "=\"" . $m[1] . "\";");
            }
        }
        if ($m[2] == "last" && $m[0] == "ca") {
            $ca = $ml[1];
        } else {
            if ($m[2] == "last" && $ml[2]) {
                eval('$' . $ml[2] . "=\"" . $ml[3] . "\";");
            } else {
                if ($m[2]) {
                    eval('$' . $m[2] . "=\"" . $m[3] . "\";");
                }
            }
        }
    } else {
        // управление макросами
        include_once "f_macro.inc";
    }
}
// админка
if ($adm && file_exists("f_admin.inc")) {
    include_once "f_admin.inc";
}
// говорить/взять
if ($speak || $speak = $cs) {
    if (substr($speak, 0, 2) == "i.") {
        $take = $speak;
    } else {
        include_once "f_speak.inc";
    }
}
// взять предмет
// для записок и книг переходим к look
if ($take) {
    include_once "f_take.inc";
}
// чат
if ($say) {
    include_once "f_say.inc";
}
// атака
if ($ca) {
    $loc_i[$loc][$login]["macrol"] = "ca|$ca||";
    $char[7] = $ca;
    $loc_i[$loc][$login]["char"] = implode("|", $char);
    attack($loc, $login, $ca);
    $char = explode("|", $loc_i[$loc][$login]["char"]);
}
// выбросить
if ($drop) {
    include_once "f_drop.inc";
}
// использовать предмет/умение
// для записок и книг переходим к look
if ($use) {
    $loc_i[$loc][$login]["macrol"] = "use|$use|to|$to";
    if ($char[6] - time() > 120) {
        $char[6] = time() - 1;
    }
    if (time() > $char[6]) {
        if ($char[8] && $login != "u.qv") {
            msg("<p>Вы призрак, найдите лекаря или камень воскрешения");
        }
        if (substr($use, 0, 6) == 'i.note' || substr($use, 0, 6) == 'i.book') {
            $look = $use;
        } else {
            $scroll = 0; // со свитка
            switch (substr($use, 0, 2)) {
                case "i.": {
                    include_once "f_useitem.inc";
                }
                    break;
                case "m.": {
                    include_once "f_usemagic.inc";
                }
                    break;
                case "p.": {
                    include_once "f_usepriem.inc";
                }
                    break;
                default:
                    if (substr($use, 0, 6) == "skill.") {
                        include_once "f_useskill.inc";
                    }
                    break;
            }
            $char = explode("|", $loc_i[$loc][$login]["char"]);
        }
    } else {
        addjournal($loc, $login, "Вы должны отдохнуть " . round($char[6] - time() + 1) . " сек");
    }
}
// Осмотреть/информация
if ($look || $look = $ci) {
    // раньше $list
    // после $take и $use
    // при ci == 1 устанавливается ViewOptions::setDescEnabled(true) - флаг "вывести описание локации"
    // возможен возврат управления
    include_once "f_look.inc";
}
// "почта"
if ($msg) {
    include_once "f_msg.inc";
}
// торговля/обмен
if ($trade) {
    include_once "f_trade.inc";
}
switch ($cl) {
    case "i":
        $cl = "inv";
        break;
    case "m":
        $cl = "magic";
        break;
    case "p":
        $cl = "priem";
        break;
}
// управление списками предметов, умений и тд
if ($list || $list = $cl) {
    $inc_list = "f_list" . $list . ".inc";
    // без возврата управления
    include_once $inc_list;
}
// показать карту и завершить работу скрипта
if (false !== $map) {
    msg(
        MapPage::buildPage(
            $loc,
            $game,
            ViewOptions::getInstance()->getMapMode(),
            $PHP_SELF,
            $sid
        )
    );
}

// MAIN PAGE
$stmp = "";
if (!empty($loc_i[$loc][$login]["msgt"])) {
    // есть почта
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&msg=1\">[msg]</a><br/>";
}
// HP/MP
$stmp .= $char[1] . "/" . $char[2] . " (" . $char[3] . "/" . $char[4] . ")";
$st = "";
if ($char[12]) {
    $st .= " всадник";
}
if ($char[8]) {
    $st .= " призрак";
}
if ($char[9]) {
    // преступник(время до снятия этого "звания")
    $st .= " " . $char[9] . " (" . (round(($char[10] - time()) / 60) + 1) . " мин)";
}
if ($game["fid"] == $login) {
    $st .= " c флагом!";
}
if ($st) {
    $stmp .= ", вы " . $st;
}
// действует защита
if (!empty([$loc][$login]["def"])) {
    $tdef = explode("|", $loc_i[$loc][$login]["def"]);
    if (time() > $tdef[2]) {
        $loc_i[$loc][$login]["def"] = "";
    } else {
        $stmp .= "<br/>" . $tdef[1] . " (" . ($tdef[2] - time()) . " сек)";
    }
}
// мы в замке
if (substr($loc, 3) == ".in" || substr($loc, 3) == ".gate") {
    include_once "f_castle.inc";
}

// SOUNDS
if (!ViewOptions::getInstance()->getSoundsMode()) {
    $st = "";
    for ($i = 2; $i < count($loc_c); $i += 2) {
        if ($loc_c[$i + 1] != $loc) {
            if (count($loc_i[$loc_c[$i + 1]]) > 0) {
                foreach ($loc_i[$loc_c[$i + 1]] as $j => $val) {
                    if ((substr($j, 0, 2) == 'u.') || substr($j, 0, 2) == 'n.') {
                        if ($st == '') {
                            $st = "<br/>Звуки: " . $loc_c[$i];
                        } else {
                            $st .= ", " . $loc_c[$i];
                        }
                        break;
                    }
                }
            }
        }
    }
    $stmp .= $st;
}

// OBJECTS
$ti = explode("x", $loc);
if (!$start) {
    $start = 0;
}
$listEnd = $start + ViewOptions::getInstance()->getMaxListSize();
$keys = array_keys($loc_i[$loc]);
for ($i = $start; $i < $listEnd && $i < count($keys); $i++) {
    if ($keys[$i] != $login) {
        $k = '';
        // предметы
        if (substr($keys[$i], 0, 2) == "i.") {
            $tmp = explode("|", $loc_i[$loc][$keys[$i]]);
            $k = $tmp[0];
            // есть самоцветы?(уточнить)
            if (strpos($keys[$i], "..") !== false) {
                $k .= " *";
            }
            // для нестационарных вывести количество
            if (substr($keys[$i], 0, 4) != "i.s." && $tmp[1] > 1) {
                $k .= " (" . $tmp[1] . ")";
            }
        }
        // НПС и игроки
        if (substr($keys[$i], 0, 2) == "n." || substr($keys[$i], 0, 2) == "u.") {
            $tmp = explode("|", $loc_i[$loc][$keys[$i]]["char"]);
            $k = $tmp[0];
            // на коне
            if (substr($keys[$i], 0, 2) == "u." && $tmp[12]) {
                $k .= " (всадник)";
            }
            $st = '';
            if ($tmp[1] != $tmp[2]) {
                // ХП не полное, вывести %
                if (round($tmp[1] * 100 / $tmp[2]) < 100) {
                    $st .= round($tmp[1] * 100 / $tmp[2]) . "%";
                }
            }
            if ($game["floc"] == $loc && $game["fid"] == $keys[$i]) {
                $st .= " с флагом!";
            }
            // для игроков
            if (substr($keys[$i], 0, 2) == "u.") {
                if ($tmp[8]) {
                    $st .= " призрак";
                }
                if ($ti[2] >= 1099 && $ti[2] <= 1370 && $tmp[14] == "t") {
                    $st .= " тамплиер";
                }
                if ($ti[2] >= 1099 && $ti[2] <= 1370 && $tmp[14] == "p") {
                    $st .= " пират";
                }
            }
            if ($tmp[9]) {
                $st .= " " . $tmp[9];
            }
            // есть цель атаки и цель находится в текущей локе
            // является НПС или лока не "безопасного" типа
            if (
                $tmp[7] && isset($loc_i[$loc][$tmp[7]]) &&
                (
                    substr($keys[$i], 0, 2) == "n." ||
                    (substr($keys[$i], 0, 2) == "u." && $loc_c[1] != 1)
                )
            ) {
                $tmp1 = explode("|", $loc_i[$loc][$tmp[7]]["char"]);
                if (substr($tmp[7], 0, 2) == "n." || (substr($tmp[7], 0, 2) == "u." && !$tmp1[8])) {
                    $st .= " атакует ";
                    if ($tmp[7] == $login) {
                        $st .= "вас!";
                    } else {
                        $st .= preg_replace("/ \\*.*?\\*/", "", $tmp1[0]);
                    }
                }
            }
            if ($st) {
                $k .= " [" . trim($st) . "]";
            }
        }
        // добавить кнопку вызова  "контекстного меню" с установкой переменной $to
        $stmp .= '<br/><a tabindex="0" class="btn btn-secondary m-1" role="button" data-trigger="focus" data-toggle="popover" data-content="template" title="' . $k . '" label="' . $keys[$i] . '">' . $k . '</a>';
    }
}
#start: "пагинация" списка объектов
if (count($keys) > 1 && $start) {
    // к началу списка
    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid\">^ </a>";
}
if ($listEnd < count($keys)) {
    if (!$start) {
        $stmp .= "<br/>";
    }
    // к следующей части списка
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&start=" . ($listEnd) . "\">+ (" . (count($keys) - $listEnd) .
        ")</a>";
}
#end: "пагинация" списка объектов

$stmp .= "<br/>---";

// Переходы к соседним локациям
for ($i = 2; $i < count($loc_c); $i += 2) {
    $stmp .= "<br/><a class=\"btn btn-outline-primary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&go=" . $loc_c[$i + 1] . "\">" . $loc_c[$i] . "</a>";
    // TODO: странное условие, нужно разобраться
    if ($char[12] && strpos($loc_tt[$loc_c[$i + 1]]["d"], $loc_c[$i] . "|") !== false) {
        // галопом на лошади
        $stmp .= "<a class=\"btn btn-primary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&gal=1&go=" . $loc_c[$i + 1] . "\">*</a>";
    }
    if (ViewOptions::getInstance()->getSoundsMode() && count($loc_i[$loc_c[$i + 1]]) > 0) {
        // выводим признак наличия персов/НПС в локе
        foreach ($loc_i[$loc_c[$i + 1]] as $j => $val) {
            if ((substr($j, 0, 2) == 'u.') || substr($j, 0, 2) == 'n.') {
                $stmp .= " !";
                break;
            }
        }
    }
}

$stmp .= "<br/><a class=\"btn btn-outline-info btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid\">обновить</a>";

// Добавить ссылку на описание локи
if (file_exists("loc_f/" . $loc)) {
    // переход по ссылке устанавливает ViewOptions::setDescEnabled(true)(смотри в f_look.inc)
    // что используется в функции msg для добавления описания локации
    $stmp .= "<br/><a class=\"btn btn-outline-secondary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&ci=1\">Инфo</a>";
}

if ($game["fid"] == $login && $game["floc"] == $loc) {
    $stmp .= "<br/><a class=\"btn btn-outline-primary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&drop=f\">Бросить флаг</a>";
}
// переход в админку и воскрешение
if ($login == $g_admin || ($gm_id && $gm == $gm_id)) {
    $stmp .= "<br/><a class=\"btn btn-outline-primary m-1\" href=\"$PHP_SELF?sid=$sid&adm=rsn\">res</a><br/><a class=\"btn btn-outline-primary m-1\" href=\"$PHP_SELF?sid=$sid&adm=smp&fmust=1\">admin</a>";
}

// MENU
$stmp .= "</p></div><div id=\"menu\" title=\"Меню\"><p>";
$stmp .= "<a class=\"btn btn-outline-primary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&cs=$(to)\">Говорить/Взять</a><br/>";
$stmp .= "<a class=\"btn btn-outline-danger btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&ca=$(to)\">Атаковать</a>";
$b = "<br/>";
// кнопки быстрого доступа к умениям и предметам
$ts = ["", "", "m", "магия", "i", "предмет", "p", "прием"];
$userMenu = strval(ViewOptions::getInstance()->getUserMenu());
for ($i = 0; $i < strlen($userMenu); $i += 2) {
    if ($ts[$userMenu { $i} * 2]) {
        // просмотр списка и управление порядком
        $stmp .= $b . "<a class=\"btn btn-outline-warning btn-sm m-1\" href=\"$PHP_SELF?sid=$sid&to=$(to)&cl=" . $ts[$userMenu { $i} * 2] . "\">" .
            $ts[$userMenu { $i} * 2 + 1] . "</a>";
        $b = ", ";
        for ($j = 1; $j <= $userMenu { $i + 1}; $j++) {
            // кнопка доступа к элементу с номером $j
            $stmp .= "<a class=\"btn btn-sm btn-warning m-1\" href=\"$PHP_SELF?sid=$sid&to=$(to)&use=" . $ts[$userMenu { $i} * 2] . "." . $j . "\">" . $j .
                "</a>";
        }
    }
}
$stmp .= "<br/><a class=\"btn btn-outline-secondary btn-sm m-1\" href=\"$PHP_SELF?sid=$sid&ci=$(to)\">Инфo</a>"; // "осмотреть" выбранный объект

// FIXME: похоже на костыль для захваченных замков
// удалить служебную информацию из названия
if (strpos($loc_c[0], "*") !== false) {
    $loc_c[0] = substr($loc_c[0], 0, strpos($loc_c[0], "*"));
}
msg($stmp, $loc_c[0], 1, PageType::MAIN);
exit;