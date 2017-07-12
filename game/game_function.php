<?php
require_once 'f_message.inc';
require_once "f_rndname.inc";
require_once "f_ressurect.inc";
require_once "f_calcparam.inc";
require_once "f_docrim.inc";
require_once "f_additem.inc";
require_once "f_addtimer.inc";
require_once "f_addexp.inc";
require_once 'f_attackf.inc';

/**
 * алиас для array_key_exists
 *
 * @param $arr array Массив
 * @param $key mixed Ключ
 *
 * @return bool
 */
function have_key($arr, $key)
{
    return array_key_exists($key, $arr);
}

/**
 * Загружает указанный файл
 *
 * @param string $file_name
 *
 * @return mixed
 */
function load_file($file_name)
{
    $file_name = BASE_DIR . $file_name;

    return file_exists($file_name) ? (require $file_name) : null;
}

/**
 * получить значение из массива
 *
 * @param array  $arr
 * @param string $key
 *
 * @return mixed
 */
function get_value($arr, $key)
{
    return (array_key_exists($key, $arr) ? $arr[$key] : false);
}

/**
 * Получение значения из $_GET
 *
 * @param string $key
 *
 * @return mixed
 */
function Get($key)
{
    return get_value($_GET, $key);
}

/**
 * Получение значения из $_POST
 *
 * @param string $key
 *
 * @return mixed
 */
function Post($key)
{
    return get_value($_POST, $key);
}

/**
 * Получение значения из $_REQUEST
 *
 * @param string $key
 *
 * @return mixed
 */
function Request($key)
{
    return get_value($_REQUEST, $key);
}

/**
 * Запись состояния.
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
            $arr      = [];
            // переходы
            $arr["d"] = $loc_tt[$i]["d"];
            // предметы, NPC и пользователи
            if (!empty($loc_i[$i])) {
                $arr["i"] = $loc_i[$i];
            }
            // таймеры
            if (!empty($loc_t[$i])) {
                $arr["t"] = $loc_t[$i];
            }
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
        if ($login && $game["fid"] == $login) {
            $game["floc"] = $loc;
        }
        fputs($file_save, serialize($game));
        //flock($file_save,3);
        fclose($file_save);
    };
    @ignore_user_abort(false);
}

/**
 * Добавление записи в журнал событий.
 *
 * @param string $loc  локация
 * @param string $to   кому сообщение. all - отправка всем
 * @param string $msg  сообщение
 * @param string $no1  исключение из доставки
 * @param string $no2  исключение из доставки
 * @param string $cont разделитель
 */
function addjournal($loc, $to, $msg, $no1 = "", $no2 = "", $cont = "|")
{
    global $loc_i, $login;
    if ( ! $loc_i[$loc]) {
        return;
    }
    $msg = preg_replace('/ \*.*?\*/', '', $msg);
    foreach ($loc_i[$loc] as $i => $val) {
        if (substr($i, 0, 2) == "u." && ($i == $to || $to == "all") && $i != $no1 && $i != $no2) {
            $loc_i[$loc][$i]["journal"] .= $cont . $msg;
            if (strlen($loc_i[$loc][$i]["journal"]) > 800) {
                $loc_i[$loc][$i]["journal"] = ".." . substr($loc_i[$loc][$i]["journal"], -800);
            }
        }
    }
}

/**
 * "ИИ". Обновление состояния в локации.
 *
 * "искусственный интеллект", проверяем локацию с именем $i
 *
 * @param string $i локация
 */
function doai($i)
{
    global $game, $loc, $loc_i, $loc_t, $loc_tt, $g_list, $start, $g_destroy, $g_crim, $g_logout, $login;
    $g_regen = 30;

    /**
     * тип локи и переходы
     *
     * @var string[] $locai
     */
    $locai = explode("|", $loc_tt[$i]["d"]);

    // таймеры
    if (isset($loc_t[$i])) {
        foreach ($loc_t[$i] as $j => $val) {
            if (time() > $j) {
                // респ НПС
                if (is_array($loc_t[$i][$j]) || substr($loc_t[$i][$j], 0, 2) == "n.") {
                    require "f_timernpc.inc";
                    continue;
                }
                // респ предметов
                if (substr($loc_t[$i][$j], 0, 2) == "i.") {
                    require "f_timeritem.inc";
                    continue;
                }
                // скриптовые таймеры - использования не найдено
                // FIXME: избавтся от eval
                $loct = $i;
                $curr = $j;
                eval($loc_t[$i][$j]);
            }
        }
    }
    $crim  = [];
    $users = [];
    $guard = 0;
    $ti    = explode("x", $i != '_begin' ? $i : 'x1158x523');
    // получить список кримов и наличие гварда на локе
    if ($loc_i[$i]) {
        foreach ($loc_i[$i] as $j => $val) {
            if ($j != "u.qv") {
                if (substr($j, 0, 2) == 'u.') {
                    $uc = explode("|", $loc_i[$i][$j]["char"]);
                    // пропускаем призраков
                    if ( ! $uc[8]) {
                        $us = explode("|", $loc_i[$i][$j]["skills"]);
                        // попытка спрятаться
                        if (rand(0, 100) > $us[17] * 6) {
                            $users[] = $j;
                        }
                        /*
                         * кримом считаются:
                         * преступники - если он не на пиратской локе($locai[1] != 3)
                         * пират на земле тамплиеров($locai[1] == 2 && $uc[14] == "p")
                         * тамплиер на земле пиратов($locai[1] == 3 && $uc[14] == "t")
                         */
                        if ($locai[1] != 3 && $uc[9] ||
                            $ti[2] >= 1099 && ($locai[1] == 2 && $uc[14] == "p" || $locai[1] == 3 && $uc[14] == "t")
                        ) {
                            $crim[] = $j;
                        }
                    }
                }
                if (substr($j, 0, 4) == 'n.c.') {
                    $crim[] = $j;
                }
                if (substr($j, 0, 4) == "n.g.") {
                    $guard = 1;
                }
            }
        }
    }

    // добавляем гварда если нужно
    // на безопасной територии гварды охотятся за преступниками
    if ($locai[1] == 1 && count($crim) > 0 && ! $guard) {
        require "f_addguard.inc";
    }

    // по всем объектам
    if ($loc_i[$i]) {
        foreach ($loc_i[$i] as $j => $val) {
            // FIXME: странное условие - как может быть не установлено значение, если индекс установлен?
            if (isset($loc_i[$i][$j])) {
                // удаление "протухших" предметов
                if (substr($j, 0, 2) == 'i.') {
                    // похоже на костыль: удалить флаг, если он есть на другой локе
                    if ($j == "i.flag" && $game["floc"] != $i) {
                        unset($loc_i[$i][$j]);
                        continue;
                    }
                    // пропуск предметов в замках
                    if (substr($i, 0, 2) == "c." && substr($j, 0, 4) != "i.s.") {
                        continue;
                    }
                    // удалить предмет с локи, если истекло его время
                    $tmp = explode("|", $loc_i[$i][$j]);
                    if ($tmp[2] && time() > $tmp[2]) {
                        unset($loc_i[$i][$j]);
                    }
                    continue;
                }
                // для игроков и НПС
                if (substr($j, 0, 2) == 'u.' || substr($j, 0, 2) == 'n.') {
                    $char = explode("|", $loc_i[$i][$j]["char"]);
                    // реген ХП/МП.
                    $tm   = time() - intval($char[5]);
                    if ($tm > $g_regen && ($char[1] != $char[2] || $char[3] != $char[4]) &&
                        (substr($j, 0, 2) == 'n.' || (substr($j, 0, 2) == 'u.' && ! $char[8]))
                    ) {
                        if (substr($j, 0, 2) == 'u.') {
                            $skills = explode("|", $loc_i[$i][$j]["skills"]);
                        } else {
                            $skills[5]  = 0;
                            $skills[16] = 0;
                        }
                        // скорость восстановления зависит от умений:
                        // ХП - от "регенерации"
                        $char[1] = min($char[1] += round($tm / ($g_regen - $skills[16] * 4)), $char[2]);
                        // МП - от "медетации"
                        $char[3] = min($char[3] += round($tm / ($g_regen - $skills[5] * 4)), $char[4]);
                        $char[5] = time();
                    }
                    // для игрока
                    if (substr($j, 0, 2) == 'u.') {
                        // сброс преступлений по сроку давности
                        if ($char[9] && time() > $char[10]) {
                            $char[9]  = 0;
                            $char[10] = "";
                        }
                        if ($j == $login) {
                            $char[11] = time(); // обновить таймер последнего действия
                        }
                        // удалить покинувших игру персонажей
                        if ($char[11] && time() > $char[11] + $g_logout * 5 && ! file_exists("online/" . $j)) {
                            unset($loc_i[$i][$j]);
                            continue;
                        }
                    }
                    // для НПС
                    if (substr($j, 0, 2) == 'n.') {
                        // с шансом 50% НПС попробует убежать с текущей локи, если ХП меньше 1/4
                        // не станут убегать: охранники(n.o.*), зомби(n.z.*) и призванные магией(n.s.*)
                        if ($loc == $i && time() > $char[6] && $char[1] < $char[2] / 4 && rand(0, 100) < 50 &&
                            substr($j, 0, 4) != 'n.s.' && substr($j, 0, 4) != 'n.o.' && substr($j, 0, 4) != 'n.z.'
                        ) {
                            $b = 0;
                            require "f_run.inc";
                            if ($b) {
                                continue;
                            }
                        }
                        // прекращаем атаковать призраков
                        if ($char[7] && isset($loc_i[$i][$char[7]]) && substr($char[7], 0, 2) == "u.") {
                            $tc = explode("|", $loc_i[$i][$char[7]]["char"]);
                            if ($tc[8]) {
                                $char[7] = "";
                            }
                        }
                        // жар-птица убегает от игроков
                        if ($j == "n.a.b.jarpt.1") {
                            $b = 0;
                            foreach ($loc_i[$i] as $k => $v) {
                                if (substr($k, 0, 2) == "u.") {
                                    addnpc($j, $i, $locai[2 + 2 * rand(0, (count($locai) - 2) / 2 - 1) + 1]);
                                    $b = 1;
                                    break;
                                }
                            }
                            if ($b) {
                                continue;
                            }
                        }
                        // отпустить гварда, если он давно бездействует(?)
                        if (substr($j, 0, 4) == "n.g." && time() > $char[11]) {
                            addnpc($j, $i, "");
                            continue;
                        }
                        // обработка подчиненных НПС
                        if (isset($loc_i[$i][$j]["owner"])) {
                            require "f_owner.inc";
                            if ($b) {
                                continue;
                            }
                        } else {
                            $owner[1] = "";
                        }
                        // преследование цели атаки
                        if ($char[7] && ! $owner[1] && ! isset($loc_i[$i][$char[7]])) {
                            $b = 0;
                            if (substr($j, 0, 4) != "n.o." && $j != "n.a.b.jarpt.1") {
                                require "f_goto.inc";
                            }
                            if ($b) {
                                continue;
                            } else {
                                $char[7] = "";
                            }
                        }
                        // установить цель атаки
                        if ( ! $char[7]) {
                            // гварды атакуют кримов
                            if (count($crim) > 0 &&
                                (substr($j, 0, 4) == "n.g." || substr($j, 0, 4) == "n.t." || substr($j, 0, 4) == "n.p.")
                            ) {
                                $char[7] = $crim[rand(0, count($crim) - 1)];
                            }
                            // кримы атакуют пользователей
                            if (($char[9] || substr($j, 0, 4) == 'n.c.') && count($users) > 0) {
                                $char[7] = $users[rand(0, count($users) - 1)];
                            }
                        }
                        // охрана замка
                        if (substr($j, 0, 4) == "n.o." && substr($i, 0, 2) == "c." && substr($i, 3) != ".in" &&
                            ( ! $char[7] || ! isset($loc_i[$i][$char[7]]))
                        ) {
                            require "f_no.inc";
                        }
                        // если нет цели и хозяина, то случайное перемещение НПС
                        // охрана замка остается на месте
                        if ( ! $char[7] && ! $owner[1] && ($char[10] || ( ! $char[10] && $char[12])) &&
                             substr($j, 0, 4) != "n.o."
                        ) {
                            require "f_na.inc";
                            if ($b) {
                                continue;
                            }
                        }
                    }
                    $loc_i[$i][$j]["char"] = implode("|", $char);
                    // НПС атакует, если выбрана цель
                    if ($char[7] && substr($j, 0, 2) != "u.") {
                        attack($i, $j, $char[7]);
                    }
                } else {
                    // какой то мусор на локе - удалить
                    // по идее сюда попадать не должно
                    unset($loc_i[$i][$j]);
                    continue;
                }
            }
        }
    }
}

/**
 * Подгружает локацию $loc.
 * ВНИМАНИЕ: изменяет глобальные переменные $loc_i, $loc_t, $loc_tt
 *
 * @param string $loc
 */
function loadloc($loc)
{
    global $loc_i, $loc_t, $loc_tt;
    if ($loc == ".") {
        return;
    }
    if ( ! isset($loc_tt[$loc])) {
        if ( ! $loc || ! file_exists("loc_i/" . $loc)) {
            return;
        }
        $tmp          = (file_get_contents("loc_i/" . $loc));
        $loc_tt[$loc] = unserialize($tmp);
        if ( ! $loc_tt[$loc]["d"]) {
            $tmp          = preg_replace('/s:(?:\d+):"(.*?)";/e', "calcser('\\1')", $tmp);
            $loc_tt[$loc] = unserialize($tmp);
        }
        if ( ! $loc_tt[$loc]["d"]) {
            $loc_tt[$loc] = unserialize((file_get_contents("loc_t/" . $loc)));
        }
        if ( ! $loc_tt[$loc]["d"]) {
            die("err: loadloc($loc)");
        }
        if (isset($loc_tt[$loc]["i"])) {
            $loc_i[$loc] = $loc_tt[$loc]["i"];
        } else {
            $loc_i[$loc] = [];
        }
        if (isset($loc_tt[$loc]["t"])) {
            $loc_t[$loc] = $loc_tt[$loc]["t"];
        }
    }
}

/**
 * Перемещение НПС(?)
 *
 * @param string $id   индификатор
 * @param string $from откуда
 * @param string $to   куда
 * @param int    $gal  флаг перемещения галопом
 * @param int    $hide флаг скрытного перемещения
 */
function addnpc($id, $from = "", $to = "", $gal = 0, $hide = 0)
{
    global $loc_i, $loc, $login, $page_d, $loc_tt, $g_j2go, $game;

    if ($from == $to) {
        return;
    }
    loadloc($from);
    loadloc($to);
    if ($from && $to && ( ! isset($loc_i[$from]) || ! isset($loc_i[$to]))) {
        return;
    }
    $ars = ["Появился", "исчез", "Пришел", "ушел", "прискакал", "поскакал", "пронесся"];

    /// FIXME: PHP Notice:  Undefined index: ''
    if (substr($id, 0, 2) == "u." &&
        (strpos($loc_i[$from][$id]["user"], "|f|") !== false || strpos($loc_i[$to][$id]["user"], "|f|") !== false)
    ) {
        $ars = [
            "Появилась",
            "исчезла",
            "Пришла",
            "ушла",
            "прискакала",
            "поскакала",
            "пронеслась"
        ];
    }
    $tnpc = "";
    if ($from && isset($loc_i[$from][$id])) {
        $floc  = explode("|", $loc_tt[$from]["d"]);
        $tnpc  = $loc_i[$from][$id];
        $tchar = substr($tnpc["char"], 0, strpos($tnpc["char"], "|"));
        if ( ! $hide) {
            if ($to && array_search($to, $floc)) {
                if ($gal && $gal != 1) {
                    addjournal($from, "all", $tchar . " " . $ars[5] . " галопом " . $gal, $id);
                } else {
                    if ( ! $gal) {
                        addjournal($from, "all", $tchar . " " . $ars[3] . " " . $floc[array_search($to, $floc) - 1],
                            $id);
                    }
                }
            } else {
                addjournal($from, "all", $tchar . " " . $ars[1], $id);
            }
        }
        unset($loc_i[$from][$id]);
    }
    if ($to && isset($loc_i[$to])) {
        if ( ! $tnpc && isset($loc_i[$to][$id])) {
            $tnpc  = $loc_i[$to][$id];
            $tchar = substr($tnpc["char"], 0, strpos($tnpc["char"], "|"));
        }
        if ($tnpc) {
            $tloc = explode("|", $loc_tt[$to]["d"]);
            if ($from && array_search($from, $tloc)) {
                if ($gal && $gal != 1) {
                    addjournal($to, "all", $tchar . " " . $ars[6] . " галопом " . $gal, $id);
                } else {
                    if ($gal == 1) {
                        addjournal($to, "all", $tchar . " " . $ars[4] . " галопом", $id);
                    } else {
                        addjournal($to, "all", $ars[2] . " " . $tchar, $id);
                    }
                }
                if (substr($id, 0, 2) == "n.") { // история следов npc
                    $tchar = explode("|", $tnpc["char"]);
                    $steps = explode(":", $tchar[12]);
                    if (count($steps) == 0) {
                        $steps[] = $from;
                    } else {
                        if ($steps[count($steps) - 1] == $to) {
                            unset($steps[count($steps) - 1]);
                        } else {
                            $steps[] = $from;
                        }
                    }
                    $tchar[12]    = implode(":", $steps);
                    $tnpc["char"] = implode("|", $tchar);
                }
            } else {
                addjournal($to, "all", $ars[0] . " " . $tchar, $id);
            }
            $loc_i[$to][$id] = $tnpc;
            if ($from && substr($id, 0, 2) == "u.") {
                if ($floc[1] == 1 && $tloc[1] != 1) {
                    addjournal($to, $id, "Вы покинули охраняемую территорию");
                }
                if ($floc[1] != 1 && $tloc[1] == 1) {
                    addjournal($to, $id, "Вы на охраняемой территории");
                }
            }
        }
    }
    if ($id == $login && $to && isset($loc_i[$to][$id])) {
        $loc = $to;
        if ($g_j2go) {
            $page_d = 1;
        }
    }
}

/**
 * Вспомогательная функция для перерасчета длины строк в файлах состояния
 *
 * @param string $s строка для перерачета
 *
 * @return string
 */
function calcser($s)
{
    return "s:" . strlen($s) . ":\"" . $s . "\";";
}