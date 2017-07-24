<?php
require_once 'f_message.inc';

/**
 * Генерация случайного имени
 *
 * @return string
 */
function rndname()
{
    $arr_rndnames = [
        'cor',
        'ur',
        'ae',
        'li',
        'sim',
        'na',
        'vax',
        'vin',
        'lib',
        'er',
        'mac',
        'cam',
        'is',
        'ta',
        'gor',
        'i',
        'ca',
        'um',
        'mu',
        'og',
        'os',
        'na',
        'tru',
        'ver',
        'vay',
        'an',
        'as',
        'prin',
        'su',
        'oc',
        'dor',
        'a',
        'mor',
        'ia',
        'gon',
        'ar',
        'nor',
        'ang',
        'dai',
        'mar',
        'grace',
        'van',
        'dir',
        'am',
        'va',
        'ber',
        'em',
        'je',
        'tar',
        'she',
        'eru',
        'ilu',
        'rat',
        'gil',
        'do',
        'ge',
        'lad',
        'le',
        'go',
        'bins',
        'las',
        'gim',
        'li',
        'fro',
        'tor',
        'tol',
        'mab',
        'den',
        'va',
        'dag',
        'tir',
        'na',
        'nogt',
        'es',
        'ka',
        'bur',
        'du',
        'ran',
        'dal',
        'ken',
        'vap',
        'dlo',
        'negn',
        'mur',
        'kok',
        'mel',
        'rul',
        'sa',
        'ru',
        'gan',
        'uuk',
        'map',
        'blo',
        'son',
        'eva',
        'nul',
        'eng',
        'zah',
        'vat',
        'obi',
        'no',
        'rip',
        'bi',
        'car',
        'ma',
        'lan',
        'be',
        'ril',
        'log',
        'raf',
        'hill',
        'nart',
        'bosk',
        'ir',
        'gard',
        'is',
        'en',
        'ged',
        'gob',
        'cri',
        'sa',
        'ru',
        'man',
        'shna',
        'god',
        're',
        'vur',
        'ar',
        'tur',
        'el',
        'eri',
        'ker',
        'shed',
        'gae',
        'bol',
        'der',
        'desh',
        'nol',
        'nek',
        'dur',
        'vek',
        'nang',
        'zug',
        'cup',
        'ida',
        'lum',
        'ir',
        'si',
        'jai',
        'kon',
        'nel',
        'jer',
        'lorn',
        'gan',
        'fax',
        'ber',
        'sa',
        'got',
        'vald',
        'lance',
        'der',
        'feld',
        'kay',
        'had',
        'ja',
        'gun',
        'tal',
        'nai',
        'ven',
        'det',
        'nog',
        'aro',
        'kle',
        'vam',
        'dam',
        'sic',
        'erg',
        'unk',
        'ils',
        'dol',
        'dul',
        'gu',
        'arc',
        'jin',
        'shel',
        'chri',
        'chra',
        'gec',
        'apr',
        'anu',
        'al',
        'van',
        'a',
        'e',
        'si',
        'an',
        'na',
        'u',
        'ol',
        'it',
        'du',
        'uv',
        'ai',
        'go',
        'she',
        'zu'
    ];
    $stmp         = "";
    srand((float)microtime() * 10000000);        // FIX: на некоторых версия PHP надо, на нек. нет
    while (strlen($stmp) < rand(4, 6)) {
        $stmp .= $arr_rndnames[rand(0, count($arr_rndnames) - 1)];
    }
    $stmp{0} = strtoupper($stmp{0});

    return $stmp;
}

/**
 * Воскрешение персонажа
 *
 * @param string $loc локация
 * @param string $to  кого
 */
function ressurect($loc, $to)
{

    global $loc_i;

    if (isset($loc_i[$loc][$to]) && substr($to, 0, 2) == "u.") {
        $char = explode("|", $loc_i[$loc][$to]["char"]);
        if ($char[8]) {
            $char[8]                  = 0;
            $char[5]                  = time();
            $loc_i[$loc][$to]["char"] = implode("|", $char);
            addjournal($loc, "all", $char[0] . " воскрес!", $to);
            addjournal($loc, $to, "Вы воскресли!");
        } else {
            addjournal($loc, $to, "Вы не призрак");
        }
    }
}

/**Пересчет характеристик персонажа
 *
 * @param string $loc   локация
 * @param string $login персонаж
 */
function calcparam($loc, $login)
{
    if ($login == "u.qv") {
        return;
    }

    global $loc_i, $game;
    if ( ! isset($loc_i[$loc][$login])) {
        return;
    }

    $auser  = $loc_i[$loc][$login];
    $char   = explode("|", $auser["char"]);
    $skills = explode("|", $auser["skills"]);

    // макс ХП от силы
    $char[2] = 10 + $skills[0] * 10;
    if ($game["fid"] == $login && $game["floc"] == $loc) {
        $char[2] += 10;
    }
    // макс МП от интелекта
    $char[4] = 10 + $skills[2] * 10;

    $twar     = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    // уклон от физ атак от ловкости, уклона и силы
    $twar[6]  = $skills[1] + $skills[12] + ($skills[0] - 1) * 2;
    // парирование физ атак от ловкости, парирования и силы
    $twar[7]  = 2 * ($skills[1] + $skills[11] + ($skills[0] - 1) * 2);
    // уклон от маг атак от инты, маг уклона и силы
    $twar[9]  = 5 * ($skills[2] + $skills[15] - ($skills[0] + 1));
    // сопрот маг атак от инты, сопрота и силы
    $twar[10] = 10 * ($skills[2] + $skills[14] - $skills[0]);
    // шит от маг атак от инты, сопрота и силы
    $twar[11] = 15 * ($skills[2] + $skills[14] - ($skills[0] + 1));

    // считаем exp, нужную для level up:
    // сумма значений skills, кроме текущей експы
    for ($i = 0; $i < count($skills); $i++) {
        if ($i != 3) {
            $twar[13] += $skills[$i];
        }
    }

    // что одето
    $b    = 0; // есть ли в руках оружие
    $hits = 0; // штраф меткости
    $art  = ""; // список самоцветов

    if ($auser["equip"]) {
        $equip = explode("|", $auser["equip"]);
        // по всем одетым предметам
        foreach (array_keys($equip) as $i) {
            if ($equip[$i]) {
                // является самодельным предметом?
                $tid = getBaseItemId($equip[$i]);
                // удалить несуществующий предмет
                if ( ! $equip[$i] || strpos($auser["items"], $equip[$i] . ":") === false ||
                    ! file_exists("items/" . $tid) || ! $tid
                ) {
                    unset($equip[$i]);
                    $auser["equip"] = implode("|", $equip);
                    continue;
                }
                // загружаем предмет
                $item = findItemByBaseId($tid);
                // Считаем штраф от уровня скилов
                $str  = "0:0:0";
                // для брони
                if (substr($equip[$i], 0, 4) == "i.a." && $item[3]) {
                    $str = $item[3];
                }
                // для оружия
                if (substr($equip[$i], 0, 4) == "i.w." && $item[4]) {
                    $str = $item[4];
                }
                // штраф в формате str:dex:int
                $stats = explode(":", $str);

                if ($skills[1] < $stats[1]) {
                    $dex = $stats[1] - $skills[1];
                } else {
                    $dex = 0;
                }
                if ($skills[2] < $stats[2]) {
                    $int = $stats[2] - $skills[2];
                } else {
                    $int = 0;
                }
                if ($skills[0] < $stats[0]) {
                    $str = $stats[0] - $skills[0];
                } else {
                    $str = 0;
                }

                $hits += $dex * 10;

                // броня, кроме щитов
                if (substr($equip[$i], 0, 4) == "i.a." && substr($equip[$i], 0, 6) != "i.a.s.") {
                    $tarm = $item[2] - $str * 2 - $int * 2;
                    if ($tarm > 0) {
                        $twar[5] += $tarm;
                    }
                }
                // щиты
                if (substr($equip[$i], 0, 6) == "i.a.s.") {
                    $tarm = $item[2] - $str * 2 - $int * 2;
                    if ($tarm > 0) {
                        $twar[8] = $tarm;
                    }
                }

                // оружие
                if (substr($equip[$i], 0, 4) == "i.w.") {
                    $b       = 1;
                    $twar[3] = $item[5] - round($skills[1] / 2);
                    // стрелковое/метательное
                    if (substr($equip[$i], 0, 6) == "i.w.r.") {
                        $twar[4] = 1;
                    }
                    $twar[12] = $item[6];
                    if ($twar[4]) {
                        // стрельба
                        $twar[14] = $item[7];    // патроны
                        $twar[0] += 10 * ($skills[1] + $skills[10] - 1);
                        if ($char[12]) {
                            $twar[0] -= 10;
                        }
                    } else {
                        // холодное оружие
                        $twar[14] = $item[7];
                        $twar[0] += 10 * ($skills[1] + $skills[9]);
                    }
                    $twar[1] += $item[2] - $str * 2 - $int * 2;
                    $twar[2] += $item[3] - $str * 2 - $int * 2;
                    // для арбалетов
                    if (substr($equip[$i], 0, 8) != "i.w.r.c.") {
                        $twar[1] += $skills[0];
                        $twar[2] += $skills[0];
                    }
                }
                // Есть "артефакты"?
                if ($tid != $equip[$i] || strpos($equip[$i], "..") !== false) {
                    // получаем список самоцветов в предмете
                    $xF = preg_match_all("/\.\.(\w+)/", $equip[$i], $regF);
                    for ($j = 0; $j < $xF; $j++) {
                        // загрузить инфо о самоцвете
                        $ti = explode("|", implode(file("items/i.i." . $regF[1][$j])));
                        // получить список характеристик
                        $ti = explode(",", $ti[2]);
                        for ($k = 0; $k < count($ti); $k++) {
                            $tir = explode(":", $ti[$k]);
                            // эфект от самоцвета учитывается только 1 раз
                            // т.е. суммируем только эффекты от разных самиоцветов
                            if (strpos($art, "|" . $regF[1][$j]) === false) {
                                if ($tir[0] > 50) {
                                    $char[$tir[0] - 50] += $tir[1];
                                } else {
                                    $twar[$tir[0]] += $tir[1];
                                }
                            }
                        }
                        $art .= "|" . $regF[1][$j];
                    }
                }
            }
        }
    }

    // рукопашная борьба
    if ( ! $b) {
        // мин урон от ловкости и рукопашной
        $twar[1] += $skills[0] + $skills[8] - 1;
        // макс урон от ловкости и рукопашной
        $twar[2] += $skills[0] + $skills[8] + 1;
        // меткость от ловкости и рукопашной
        $twar[0] += 10 * ($skills[1] + $skills[8] + 2);
        if ($twar[0] >= 100) {
            $twar[0] = 95;
        }
        // штраф, если на коне
        if ($char[12]) {
            $twar[0] -= 20;
        }
        // время отката атаки от ловкости
        $twar[3]  = 5 - round($skills[1] / 2);
        $twar[12] = "кулаками";
    }

    // штраф
    // FIXME: ИМХО,костыль, должно учитываться при рассчете эфекта артефактов
    if (strpos($auser["equip"], "..do") !== false) {
        $hits += 20;
    }
    $twar[0] -= $hits;

    // проверки
    if ($twar[0] <= 0) {
        $twar[0] = 5;
    }
    if ($twar[0] > 95) {
        $twar[0] = 95;
    }
    if ($twar[1] < 0) {
        $twar[1] = 0;
    }
    if ($twar[2] < 0) {
        $twar[2] = 0;
    }
    if ($twar[3] < 3) {
        $twar[3] = 3;
    }
    if ($twar[5] < 0) {
        $twar[5] = 0;
    }
    if ($twar[6] < 0) {
        $twar[6] = 0;
    }
    if ($twar[7] < 0) {
        $twar[7] = 0;
    }
    if ($twar[9] < 0) {
        $twar[9] = 0;
    }
    if ($twar[10] < 0) {
        $twar[10] = 0;
    }

    // парировать атаку можно только со щитом
    if ($twar[7] && strpos($auser["equip"], "i.a.s.") === false) {
        $twar[7] = 0;
    }
    // ХП(1)/МП(3) не может быть больше максимума
    if ($char[1] > $char[2]) {
        $char[1] = $char[2];
    }
    if ($char[3] > $char[4]) {
        $char[3] = $char[4];
    }

    // ок, сохраняем...
    $auser["char"] = implode("|", $char);
    if ($auser["war"]) {
        $killd    = explode("|", $auser["war"]);
        $twar[15] = $killd[15];
        $twar[16] = $killd[16];
    }
    $auser["war"]        = implode("|", $twar);
    $loc_i[$loc][$login] = $auser;
}

/**
 * Превращает персонажа в "крима" - нарушителя
 *
 * @param string $loc   локация
 * @param string $login логин
 * @param string $title "звание"
 */
function docrim($loc, $login, $title = "преступник")
{

    global $loc_i, $g_crim;
    if (substr($loc, 0, 2) == "c." && substr($loc, 3) != ".in") {
        return;
    }
    if (isset($loc_i[$loc][$login]) && substr($login, 0, 2) == "u." && $loc != "arena") {
        $char                        = explode("|", $loc_i[$loc][$login]["char"]);
        $char[9]                     = $title;
        $char[10]                    = time() + $g_crim;
        $loc_i[$loc][$login]["char"] = implode("|", $char);
    }
}

/**
 * Обрабатывает атаку
 *
 * @param string $loc    Локация
 * @param string $from   кто атакует
 * @param string $to     кого атакуют
 * @param string $magic  маг атака(?)
 * @param int    $answer флаг что атака в ответ(?)
 * @param int    $rmagic хз
 * @param string $priem  тип приема(?)
 * @param string $ptitle название приема(?)
 */
function attack(
    $loc,
    $from,
    $to,
    $magic = '',
    $answer = 1,
    $rmagic = 0,
    $priem = "",
    $ptitle = ""
) {
    global $loc_i, $loc_tt, $g_destroy, $g_crim, $g_exp, $PHP_SELF, $game;
    // проверки
    if ((substr($from, 0, 2) != 'u.' && substr($from, 0, 2) != 'n.') ||
        (substr($to, 0, 2) != 'u.' && substr($to, 0, 2) != 'n.') || ! isset($loc_i[$loc][$from]) ||
        ! isset($loc_i[$loc][$to]) || $from == $to
    ) {
        return;
    }
    $fchar = explode("|", $loc_i[$loc][$from]["char"]);
    if (substr($from, 0, 2) == 'u.' && $fchar[8]) {
        if ($answer) {
            addjournal($loc, $from, "Вы призрак");
        }

        return;
    }

    $loct  = $loc;
    $aloct = explode("|", $loc_tt[$loc]["d"]);
    $tchar = explode("|", $loc_i[$loct][$to]["char"]);
    if (substr($to, 0, 2) == 'u.' && $tchar[8]) {
        if ($answer) {
            addjournal($loc, $from, "Нельзя атаковать призрака");
        }

        return;
    }
    $twar = explode("|", $loc_i[$loct][$to]["war"]);

    if ($fchar[6] - time() > 300) {
        $fchar[6] = time() - 1;
    }
    if ( ! $rmagic && time() <= $fchar[6]) {
        if ($answer) {
            addjournal($loc, $from, "Вы должны отдохнуть " . round($fchar[6] - time() + 1) . " сек");
        }

        return;
    }
    if ($loc_i[$loc][$to]["def"]) {
        $tdef = explode("|", $loc_i[$loc][$to]["def"]);
    } else {
        $tdef = ["", "", 0];
    }
    if ($tdef[2] && time() > $tdef[2]) {
        $loc_i[$loc][$to]["def"] = "";
        $tdef                    = ["", "", 0];
    }
    if ($ptitle) {
        $ptitle = " (" . $ptitle . ")";
    }
    $tloc = explode("x", $loc);
    if ($magic) {
        $fwar = explode("|", $magic);
    } else {
        $fwar = explode("|", $loc_i[$loc][$from]["war"]);
    }
    if ($answer) {
        $fchar[6]                   = time() + $fwar[3];
        $loc_i[$loc][$from]["char"] = implode("|", $fchar);
    }
    if ($fwar[12] == "мaгиeй") {
        $fwar[12] = "магией"; // FIXME: смесь русских и английских букв в описаниях NPC
    }    //eng a,e
    if ($rmagic || $fwar[12] == "магией" || $fwar[12] == "молнией") {
        if ($tdef[0] == "p.d.z" && rand(0, 100) <= $tdef[3] * 0.10) {
            if (substr($loc_i[$loc][$from]["def"], 0, 5) == "p.d.c") {
                $fdef = explode("|", $loc_i[$loc][$from]["def"]);
                $fdef = $fdef[3];
            } else {
                $fdef = 0;
            }
            if (rand(0, 100) > $fdef) {
                $fwar[0] = 0;
            }
        }
        if (substr($loc_i[$loc][$from]["def"], 0, 5) == "p.d.c") {
            $loc_i[$loc][$from]["def"] = "";
        }
        if ($tdef[0] == "p.d.z") {
            $loc_i[$loc][$to]["def"] = "";
            $t2                      = $tdef[1];
        }
        $uklon   = $twar[9];
        $parring = $twar[10];
        $shield  = $twar[11];
    } else {
        $uklon   = $twar[6];
        $parring = $twar[7];
        $shield  = $twar[8];
        if ($tdef[0] == "p.d.u" && $fwar[4]) {
            if (rand(0, 100) <= $tdef[3]) {
                $uklon += 35;
            }
            $loc_i[$loc][$to]["def"] = "";
            $t2                      = $tdef[1];
        }
        if ($tdef[0] == "p.d.re") {
            if (rand(0, 100) <= $tdef[3]) {
                $uklon += 20;
            }
            $loc_i[$loc][$to]["def"] = "";
            $t2                      = $tdef[1];
        }
        if ($tdef[0] == "p.d.p") {
            if (rand(0, 100) <= $tdef[3]) {
                $parring *= 2;
            }
            $loc_i[$loc][$to]["def"] = "";
            $t2                      = $tdef[1];
        }
    }

    if ($priem == "p.g" && $tdef[0] == "p.d.g") {
        if (rand(0, 100) <= $tdef[3]) {
            $fwar[1] = 0;
            $fwar[2] = 0;
        }
        $loc_i[$loc][$to]["def"] = "";
        $t2                      = $tdef[1];
    }
    if (substr($loc_i[$loc][$from]["def"], 0, 6) == "p.d.re") {
        $fwar[1] = round($fwar[1] * 0.6);
        $fwar[2] = round($fwar[2] * 0.6);
    }
    if ($tdef[0] == "p.d.o" && ! $rmagic) {
        if (rand(0, 100) <= $tdef[3]) {
            $fwar[1] = round($fwar[1] * 0.4);
            $fwar[2] = round($fwar[2] * 0.4);
        }
        $t2 = $tdef[1];
    }
    if ($priem == "p.n" && $tdef[0] == "p.d.n" || $priem == "p.r" && $tdef[0] == "p.d.r" ||
        $priem == "p.vs" && $tdef[0] == "p.d.s" ||
        $priem == "p.vw" && strpos($loc_i[$loc][$to]["equip"], "i.a.s.") !== false
    ) {
        $t2 = $tdef[1];
    }
    if ($t2) {
        $t2 = " (" . $t2 . ")";
    }

    // крим если атакует не крима или животное в городе
    $fstp = strpos($fchar[0], "*");
    $tstp = strpos($tchar[0], "*");
    if ($fstp === false) {
        $clan1 = "";
    } else {
        $clan1 = substr($fchar[0], $fstp + 1, strrpos($fchar[0], "*") - $fstp - 1);
    }
    if ($tstp === false) {
        $clan = "";
    } else {
        $clan = substr($tchar[0], $tstp + 1, strrpos($tchar[0], "*") - $tstp - 1);
    }
    $fcrim = $fchar[9] || substr($from, 0, 4) == "n.c.";

    $tcrim = $tchar[9] || substr($to, 0, 4) == "n.c." || $to == "n.w.Veelzevul" || $to == "n.whitewolf" ||
        $game["floc"] == $loc && $game["fid"] == $to;
    if ($tloc[2] >= 1099) {
        $tcrim = $tcrim || $tchar[14] == "p" || substr($to, 0, 4) == "n.p." || $fchar[14] == "p" && $tchar[14] == "t";
    }
    if ($fchar[13]) {
        $wife = $to == substr($fchar[13], 0, strlen($to));
    } else {
        $wife = 0;
    }
    if ($from != $to && ! $fcrim && $tchar[7] != $from && ! $tcrim && ( ! $clan1 || ($clan1 && $clan1 != $clan)) &&
        ! $wife && $from != "u.qv" && $to != "u.qv"
    ) {
        if (isset($loc_i[$loc][$to]["owner"])) {
            docrim($loc, $from, "живодер");
        }    //$aloct[1] && substr($to,0,4)=="n.a." ||
        else {
            if (substr($to, 0, 4) != "n.a.") {
                docrim($loc, $from, "бандит");
            }
        }
        $fchar = explode("|", $loc_i[$loc][$from]["char"]);
    }

    // патроны
    if ($fwar[14]) {
        if (strpos($loc_i[$loc][$from]["items"], $fwar[14] . ":") !== false) {
            additem($loc, $from, "", $fwar[14], 1, "items", "", 0);
            if (strpos($loc_i[$loc][$from]["items"], $fwar[14] . ":") === false) {
                addjournal($loc, $from, "Боеприпасы кончились");
            }
        } else {
            addjournal($loc, $from, "Нет боеприпасов");

            return;
        }
    }
    // цель конник
    if (substr($to, 0, 2) == "u." && $tchar[12] && ! $rmagic && $fwar[12] != "магией") {
        $fwar[0] -= 10;
    }
    if (substr($to, 0, 4) == "n.c.") {
        if (strpos($loc_i[$loc][$from]["equip"], "i.a.m.vlast") !== false) {
            $fwar[1] = $fwar[1] * 2;
            $fwar[2] = $fwar[2] * 2;
        }
    }

    // заклинание сорвалось?
    if ($fwar[0] || ! $fwar[0] && ! $rmagic && $fwar[12] != "магией") {
        // попадание
        if (rand(0, 100) <= $fwar[0]) {
            // урон
            $damage = round(rand($fwar[1], $fwar[2]));
            // уклон
            if (rand(0, 100) > $uklon) {
                // щит
                if ($parring && $shield) {
                    if (rand(0, 100) <= $parring) {
                        if ( ! $rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией") {
                            $damage -= $shield;
                            $t1 = " (щит " . $shield . ")";
                        } else {
                            $resist = round($damage * $shield / 100);
                            if ($resist) {
                                $tsh = rand(0, $resist);
                            } else {
                                $tsh = 0;
                            }
                            $damage -= $tsh;
                            $t1 = " (сопр. магии " . $tsh . ")";
                        }
                    }
                }
                // броня
                if ( ! $rmagic && $fwar[12] != "магией" && $twar[5] && $fwar[12] != "молнией" && $twar[5]) {
                    $damage -= round(rand(0, $twar[5]));
                } // armor
                if ($damage < 0) {
                    $damage = 0;
                }
                if ($fwar[4]) {
                    $skrit = 5;
                } else {
                    if ($rmagic || $fwar[12] == "магией" || $fwar[12] == "молнией") {
                        $skrit = 1;
                    } else {
                        $skrit = 2;
                    }
                }
                if ($damage && rand(0, 100) < $skrit) {
                    $damage *= 2;
                    $tkrit = " критически";
                } else {
                    $tkrit = "";
                }
                if ($loc_i[$loct][$to]["god"]) {
                    $damage = 0;
                }    // БОГ
                // урон
                $tchar[1] -= $damage;
                $tchar[5] = time();
                if ($tchar[1] < 0) {
                    $tchar[1] = 0;
                }
                if ( ! $answer && ! $rmagic) {
                    addjournal($loc, $from, "вы" . $ptitle . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2, "",
                        "", ", ");
                    addjournal($loc, "all", $fchar[0] . $ptitle . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2,
                        $from, "", ", ");
                } else {
                    addjournal($loc, $from,
                        "Вы" . $ptitle . " по " . $tchar[0] . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2);
                    addjournal($loc, $to,
                        $fchar[0] . $ptitle . " по вам" . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2);
                    addjournal($loc, "all",
                        $fchar[0] . $ptitle . " по " . $tchar[0] . $tkrit . " " . $fwar[12] . " " . $damage . $t1 . $t2,
                        $from, $to);
                }

                // жена/муж
                if (substr($to, 0, 2) == "u." && $tchar[13] && $tchar[1] < $tchar[2]) {
                    $tm = explode(":", $tchar[13]);
                    if (time() > $tm[1] && file_exists("online/" . $tm[0]) && filesize("online/" . $tm[0]) != 1) {
                        $tmf = file("online/" . $tm[0]);
                        $tmf = trim($tmf[0]);
                        if ($tmf != $loc) {
                            loadloc($tmf);
                            $tup = explode("|", $loc_i[$tmf][$tm[0]]["user"]);
                            if (strpos($aloct[0], "*") !== false) {
                                $aloct[0] = substr($aloct[0], 0, strpos($aloct[0], "*"));
                            }
                            if ($tup[2] == "m") {
                                $ts = "Ваша жена (" . $aloct[0] . ") ранена!";
                            } else {
                                $ts = "Ваш муж (" . $aloct[0] . ") ранен!";
                            }
                            addjournal($tmf, $tm[0],
                                "<a href=\"$PHP_SELF?sid=" . $tm[0] . "&p=" . $tup[0] . "&stele=1\">" . $ts . "</a>");
                            $tm[1]     = time() + 300;
                            $tchar[13] = implode(":", $tm);
                        }
                    }
                }
                // если убили, добавим труп
                if ($tchar[1] < 1) {
                    include "f_kill.inc";
                } else {
                    $loc_i[$loct][$to]["char"] = implode("|", $tchar);
                } // иначе сохраним в f_kill.inc

            } else {
                if ( ! $answer) {
                    if ( ! $rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией") {
                        addjournal($loct, $from, "вы" . $ptitle . " мимо (уклон)" . $t2, "", "", ", ");
                        addjournal($loct, "all", $fchar[0] . $ptitle . " мимо (уклон)" . $t2, $from, "", ", ");
                    } else {
                        addjournal($loct, $from, "вы" . $ptitle . " мимо (уклон от магии)" . $t2, "", "", ", ");
                        addjournal($loct, "all", $fchar[0] . $ptitle . " мимо (уклон от магии)" . $t2, $from, "", ", ");
                    }
                } else {
                    if ( ! $rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией") {
                        addjournal($loct, $from, "Вы" . $ptitle . " по " . $tchar[0] . " мимо (уклон)" . $t2);
                        addjournal($loct, $to, $fchar[0] . " по вам мимо (уклон)");
                        addjournal($loct, "all", $fchar[0] . $ptitle . " по " . $tchar[0] . " мимо (уклон)" . $t2,
                            $from, $to);
                    } else {
                        addjournal($loct, $from, "Вы" . $ptitle . " по " . $tchar[0] . " мимо (уклон от магии)" . $t2);
                        addjournal($loct, $to, $fchar[0] . $ptitle . " по вам мимо (уклон от магии)" . $t2);
                        addjournal($loct, "all",
                            $fchar[0] . $ptitle . " по " . $tchar[0] . " мимо (уклон от магии)" . $t2, $from, $to);
                    }
                }
            }
        } else {
            if ( ! $answer && ! $rmagic) {
                addjournal($loc, $from, "вы" . $ptitle . " мимо" . $t2, "", "", ", ");
                addjournal($loc, "all", $fchar[0] . $ptitle . " мимо" . $t2, $from, "", ", ");
            } else {
                addjournal($loc, $from, "Вы" . $ptitle . " по " . $tchar[0] . " мимо" . $t2);
                addjournal($loc, $to, $fchar[0] . $ptitle . " по вам мимо" . $t2);
                addjournal($loc, "all", $fchar[0] . $ptitle . " по " . $tchar[0] . " мимо" . $t2, $from, $to);
            }
        }
    }// заклинание сорвалось

    // если npc свободен, то атакует
    if (isset($loc_i[$loc][$from]) && ($answer || $rmagic)) {
        $fchar[7]                   = $to;
        $loc_i[$loc][$from]["char"] = implode("|", $fchar);
    }
    if (isset($loc_i[$loc][$from]) && isset($loc_i[$loct][$to]) && $from != $to &&
        ($fwar[0] || ! $fwar[0] && ! $rmagic && $fwar[12] != "магией" && $fwar[12] != "молнией")
    ) {
        if (substr($to, 0, 2) == "n." && ! $tchar[7]) {
            $tchar[7]                  = $from;
            $loc_i[$loct][$to]["char"] = implode("|", $tchar);
        }
        if ($answer) {
            attack($loct, $to, $from, 0, 0);
        }
    }
}

/**
 * Управление вещами: добавить, передать, удалить, посчитать количество
 *
 * FIXME: Имя метода не соответствует.
 * FIXME: Выполняет несколько операций и требуется разделение на разные функции.
 *
 * @param string $loc         локация
 * @param string $from        от кого userId|"loc"|""
 * @param string $to          кому userId|"loc"|""
 * @param string $itemId      что
 * @param mixed  $count       количество либо 'count' для авторасчета
 * @param string $ft          откуда брать items|bank
 * @param string $tt          куда добавлять  items|bank
 * @param int    $journal     флаг занесения в журнал
 * @param int    $time_delete время действия(-1 - получить время из конфига)
 * @param int    $msg         флаг вывода сообщения
 *
 * @return int флаг успеха(?)/количество
 */
function additem(
    $loc,
    $from,
    $to,
    $itemId,
    $count = 1,
    $ft = "items",
    $tt = "items",
    $journal = 1,
    $time_delete = -1,
    $msg = 0
) {

    global $g_destroy, $loc_i;

    if ($count != "count") {
        $count = intval($count);
        if (gettype($count) != "integer" || $count == 0) {
            if ($msg) {
                msg("Количество равно " . $count);
            }
            if ($journal && $to) {
                addjournal($loc, $to, "Количество равно " . $count);
            }

            return 0;
        }
    }
    if ($time_delete == -1) {
        $time_delete = $g_destroy;
    }
    $item = findItemByFullId($itemId);
    $title = $item[0];
    if ($from && $from != "loc" && ! isset($loc_i[$loc][$from])) {
        if ($journal && $to) {
            addjournal($loc, $to, "Не от кого забрать " . $title);
        }

        return 0;
    }
    if ($to && $to != "loc" && ! isset($loc_i[$loc][$to])) {
        if ($journal && $from) {
            addjournal($loc, $from, "Некому передать " . $title);
        }

        return 0;
    }

    // получение кол-ва
    if ($count == "count") {
        // из локации
        if ($from == "loc") {
            if ( ! isset($loc_i[$loc][$itemId])) {

                return 0;
            } else {
                $tmp = explode("|", $loc_i[$loc][$itemId]);

                return $tmp[1];
            }
        } else {
            // у НПС/игрока
            $res = intval(preg_replace('/.*' . $itemId . ':(\d+).*/', "\\1", $loc_i[$loc][$from][$ft]));

            return $res;
        }
    }

    if ($itemId == "i.q.pjpt" || $itemId == "i.q.pdeath") {
        if (substr($to, 0, 2) == "u." && (strpos($loc_i[$loc][$to][$tt], $itemId) !== false || $count > 1)) {
            msg("У вас уже есть " . $title . ", нельзя хранить более одного");
        }
    }
    if ($itemId == "i.q.ssword" && substr($to, 0, 2) == "u.") {
        $tcs = intval(preg_replace('/.*' . $itemId . ':(\d+).*/', "\\1", $loc_i[$loc][$to][$tt]));
        if ($tcs > 1 || $count > 2 || $tcs == 1 && $count == 2) {
            msg("Можно держать одновременно не более 2 стеклянных мечей");
        }
    }
    if ($to == $from && ($ft == "bank" || $tt == "bank") && substr($to, 0, 2) == "u." &&
        strlen($loc_i[$loc][$to][$tt]) > 800 && strpos($loc_i[$loc][$to][$tt], $itemId . ":") === false
    ) {
        msg("Нет места для " . $title);
    }
    if (substr($itemId, 0, 4) == "i.s." && (substr($to, 0, 2) == "u." || substr($from, 0, 2) == "u.")) {
        msg("Нельзя передать");
    }

    if ($from) {
        if ($from == "loc") {
            if ( ! isset($loc_i[$loc][$itemId])) {
                if ($journal && $to) {
                    addjournal($loc, $to, $title . " отсутствует");
                }

                return 0;
            } else {
                $tmp = explode("|", $loc_i[$loc][$itemId]);
                if ($tmp[1] < $count) {
                    if ($journal && $to) {
                        addjournal($loc, $to, "Количество " . $title . " меньше, чем " . $count);
                    }

                    return 0;
                }
                $tmp[1] -= $count;
                if ($tmp[1] == 0) {
                    unset($loc_i[$loc][$itemId]);
                } else {
                    $loc_i[$loc][$itemId] = implode("|", $tmp);
                }
            }
        }

        if ($from != "loc") {
            if (substr($from, 0, 6) == "i.s.d.") {
                $d = 1; // забираем с трупа
            } else {
                $d = 0;
            }
            if ( ! $d) {
                $ftitle = explode("|", $loc_i[$loc][$from]["char"]);
                $ftitle = $ftitle[0];
                $items  = $loc_i[$loc][$from][$ft];
            } else {
                $tdied = explode("|", $loc_i[$loc][$from]);
                $items = str_replace(",", "|", $tdied[3]);
            }
            $items = preg_replace('/\\|{2,}/', "|", $items);
            if (substr($items, 0, 1) == "|") {
                $items = substr($items, 1);
            }
            if (substr($items, strlen($items) - 1, 1) == "|") {
                $items = substr($items, 0, strlen($items) - 1);
            }

            $tcount = intval(preg_replace('/.*' . $itemId . ':(\d+).*/', "\\1", $items));
            if ($tcount == 0) {
                if ($msg) {
                    msg("У вас нет " . $title);
                }
                if ($journal && $to) {
                    addjournal($loc, $to, "Количество " . $title . " равно нулю");
                }
                if ($journal && ! $d) {
                    addjournal($loc, $from, "У вас нет " . $title);
                }

                return 0;
            } else {
                if ($tcount < $count) {
                    if ($msg) {
                        msg("У вас недостаточно " . $title . " (надо " . $count . ")");
                    }
                    if ($journal && $to) {
                        addjournal($loc, $to, "Количество " . $title . " меньше, чем " . $count);
                    }
                    if ($journal && ! $d) {
                        addjournal($loc, $from, "У вас количество " . $title . " меньше, чем " . $count);
                    }

                    return 0;
                }
                if ($tcount == $count &&
                    strpos($items, "=" . $itemId . ":" . $tcount) === false
                ) {    // удаляем предмет (кроме торговцев, у кот. ver|min|max=id:count), проверяем equip
                    $items = preg_replace('/\|?' . $itemId . ':(\d+)/', "", $items);
                    $equip = $loc_i[$loc][$from]["equip"];
                    $tbp   = strpos($loc_i[$loc][$from]["equip"], $itemId);
                    if ($tbp === false) {
                        $tb = "a";
                    } else {
                        $tb = $loc_i[$loc][$from]["equip"]{$tbp + strlen($itemId)};
                    }
                    if ( ! $d && $equip && ($tb == "|" || $tb == "")) {
                        $equip = preg_replace('/' . $itemId . '\|?/', "", $equip);
                        $equip = preg_replace("/\|{2,}/", "|", $equip);
                        if (substr($equip, 0, 1) == "|") {
                            $equip = substr($equip, 1);
                        }
                        if (substr($equip, strlen($equip) - 1, 1) == "|") {
                            $equip = substr($equip, 0, strlen($equip) - 1);
                        }
                        $loc_i[$loc][$from]["equip"] = $equip;
                        $loc_i[$loc][$from][$ft]     = $items;
                        calcparam($loc, $from);
                    }
                } else {
                    $tcount -= $count;
                    $items = preg_replace('/' . $itemId . ':(\d+)/', $itemId . ":" . $tcount, $items);
                }
                if ( ! $d) {
                    $loc_i[$loc][$from][$ft] = $items;
                } else {
                    if (strpos($tdied[0], "*") === false) {
                        $clan = "";
                    } else {
                        $clan = substr($tdied[0], strpos($tdied[0], "*") + 1,
                            strrpos($tdied[0], "*") - strpos($tdied[0], "*") - 1);
                    }
                    if ($to) {
                        $tc = explode("|", $loc_i[$loc][$to]["char"]);
                    } else {
                        $tc[0] = "";
                    }
                    if (strpos($tc[0], "*") === false) {
                        $clan1 = "";
                    } else {
                        $clan1 = substr($tc[0], strpos($tc[0], "*") + 1,
                            strrpos($tc[0], "*") - strpos($tc[0], "*") - 1);
                    }
                    if ($tc[13]) {
                        $wife = substr($from, 6, strrpos($from, ".") - 6) ==
                            substr($tc[13], 0, strlen(substr($from, 6, strrpos($from, ".") - 6)));
                    } else {
                        $wife = 0;
                    }
                    if ( ! $tdied[1] && $to && substr($from, 0, strlen("i.s.d." . $to)) != "i.s.d." . $to &&
                        ( ! $clan1 || ($clan1 && $clan1 != $clan)) && ! $wife && substr($from, 0, 11) != "i.s.d.u.qv."
                    ) {
                        docrim($loc, $to, "мародер");
                    }
                    $tdied[3]           = str_replace("|", ",", $items);
                    $loc_i[$loc][$from] = implode("|", $tdied);
                }
                if ($journal && $to == "loc") {
                    if ($journal && ! $d) {
                        addjournal($loc, $from, "Вы бросили " . $count . " " . $title);
                    }
                    if ($journal && ! $d) {
                        addjournal($loc, "all", $ftitle . " бросил " . $count . " " . $title, $from);
                    }
                }
                if ($journal && $to != "loc") {
                    if ($journal && ! $d) {
                        addjournal($loc, $from, "Вы потеряли " . $count . " " . $title);
                    }
                }
            }
        }
    }

    if ($to) {

        if ($to != "loc") {
            if (substr($to, 0, 6) == "i.s.d.") {
                $d = 1;
            } else {
                $d = 0;
            }
            if ( ! $d) {
                $ftitle = explode("|", $loc_i[$loc][$to]["char"]);
                $ftitle = $ftitle[0];
                $items  = $loc_i[$loc][$to][$tt];
            } else {
                $tdied = explode("|", $loc_i[$loc][$to]);
                $items = str_replace(",", "|", $tdied[3]);
            }

            $items = preg_replace("/\|{2,}/", "|", $items);
            if (substr($items, 0, 1) == "|") {
                $items = substr($items, 1);
            }
            if (substr($items, strlen($items) - 1, 1) == "|") {
                $items = substr($items, 0, strlen($items) - 1);
            }

            if (substr($to, 0, 2) == "u.") {
                if (strlen($items) > 800 && strpos($items, $itemId . ":") === false) {
                    addjournal($loc, $to, "Не хватает места, " . $title . " упал вам под ноги");
                    $to = "loc";
                }
            }

            if ($to != "loc") {
                $tcount = intval(preg_replace('/.*' . $itemId . ':(\d+).*/', "\\1", $items));

                if ($tcount == 0) {    // торговцам новый предмет в банк не добавляем
                    if (strpos($items, "=" . $itemId . ":" . $tcount) === false) {
                        if ($items == "") {
                            $items = $itemId . ":" . $count;
                        } else {
                            $items .= "|" . $itemId . ":" . $count;
                        }
                    }
                } else {
                    $tcount += $count;
                    $items = preg_replace('/' . $itemId . ':(\d+)/', $itemId . ":" . $tcount, $items);
                }
                if ( ! $d) {
                    $loc_i[$loc][$to][$tt] = $items;
                } else {
                    $tdied[3]         = str_replace("|", ",", $items);
                    $loc_i[$loc][$to] = implode("|", $tdied);
                }
                if ($journal && $from == "loc") {
                    if ($journal && ! $d) {
                        addjournal($loc, $to, "Вы подняли " . $count . " " . $title);
                    }
                    if ($journal && ! $d) {
                        addjournal($loc, "all", $ftitle . " поднял " . $count . " " . $title, $to);
                    }
                }
                if ($journal && $from != "loc") {
                    if ($journal && ! $d) {
                        addjournal($loc, $to, "Вы получили " . $count . " " . $title);
                    }
                }
            }
        }

        if ($to == "loc") {
            if ( ! isset($loc_i[$loc][$itemId])) {
                $loc_i[$loc][$itemId] = $title . "|" . $count . "|" . (time() + $time_delete);
            } else {
                $tmp = explode("|", $loc_i[$loc][$itemId]);
                $tmp[1] += $count;
                $tmp[2]             = time() + $time_delete;
                $loc_i[$loc][$itemId] = implode("|", $tmp);
            }
        }
    }
    $res = 1;

    //return 1;    // ок

    return $res;
}

/**
 * Добавление/обновление таймера
 *
 * @param string       $location_id локация
 * @param int          $curr        тек. таймер в локации $location_id
 * @param int          $time        период
 * @param string|array $value       описание таймера в виде текста либо массива для установки или old для использования
 *                                  старого
 * @param int          $delete      флаг удаления текущего
 */
function addtimer($location_id, $curr, $time, $value = "old", $delete = 1)
{
    global $loc_t;
    loadloc($location_id);
    $new_time = time() + $time;
    // одновременный респ невозможен - ищем ближайший свободный
    while (isset($loc_t[$location_id][$new_time])) {
        $new_time++;
    }
    if ($value == "old") {
        $value = $loc_t[$location_id][$curr];
    }
    $loc_t[$location_id][$new_time] = $value;
    if ($delete && $curr) {
        unset($loc_t[$location_id][$curr]);
    }
}

/**
 * Добавляет экспу
 *
 * @param string $loc локация
 * @param string $to  логин
 * @param int    $exp количество
 */
function addexp($loc, $to, $exp)
{
    // добавляем экспу
    global $loc_i, $g_exp;
    if (intval($exp) == 0) {
        return;
    }
    if (substr($to, 0, 2) == "u.") {
        $skills = explode("|", $loc_i[$loc][$to]["skills"]);
        $war    = explode("|", $loc_i[$loc][$to]["war"]);
        $skills[3] += $exp;
        addjournal($loc, $to, "Опыт +" . intval($exp));
        if ($skills[3] > $war[13] * $g_exp) {
            $skills[3] = 0;
            $skills[4] += 1;
            addjournal($loc, $to, "Вы получили 1 очко опыта!");
        }
        $loc_i[$loc][$to]["skills"] = implode("|", $skills);
        if ($skills[3] == 0) {
            // level up - нужен пересчет
            calcparam($loc, $to);
        }
    }
}

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
 * @param string $locId  локация
 * @param string $to   кому сообщение. all - отправка всем
 * @param string $msg  сообщение
 * @param string $no1  исключение из доставки
 * @param string $no2  исключение из доставки
 * @param string $cont разделитель
 */
function addjournal($locId, $to, $msg, $no1 = "", $no2 = "", $cont = "|")
{
    global $loc_i;
    if ( ! $loc_i[$locId]) {
        return;
    }
    $msg = preg_replace('/ \*.*?\*/', '', $msg);
    foreach ($loc_i[$locId] as $id => $val) {
        if (substr($id, 0, 2) == "u." && ($id == $to || $to == "all") && $id != $no1 && $id != $no2) {
            $loc_i[$locId][$id]["journal"] .= $cont . $msg;
            if (strlen($loc_i[$locId][$id]["journal"]) > 800) {
                $loc_i[$locId][$id]["journal"] = ".." . substr($loc_i[$locId][$id]["journal"], -800);
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
                    //  загружаем в $npc из папки npc	id|resp_min:resp_max|move_num:time_min:time_max
                    if (is_array($loc_t[$i][$j])) {
                        $npc = $loc_t[$i][$j];
                        $tid = $npc["id"];
                        unset($npc["id"]);
                    } else {
                        $ta  = explode("|", $loc_t[$i][$j]);
                        $tid = $ta[0];
                        if (in_array(substr($tid, 0, 4), ["n.c.", "n.a."])) {
                            $tid = substr($tid, 0, strrpos($tid, "."));
                        }
                        if ( ! file_exists("npc/" . $tid)) {
                            unset($loc_t[$i][$j]);

                            return;//("err: no npc/".$tid);
                        }
                        $npc        = include "npc/" . $tid;
                        $tid        = $ta[0];
                        $twar       = explode("|", $npc["war"]);
                        $twar[15]   = $i . ":" . $ta[1];
                        $npc["war"] = implode("|", $twar);
                        /// FIXME: PHP Notice:  Undefined offset: 2
                        if (!empty($ta[2])) {
                            $tchar       = explode("|", $npc["char"]);
                            $tchar[10]   = $ta[2];
                            $npc["char"] = implode("|", $tchar);
                        }
                    }

                    // случ. предметы
                    if (isset($npc["itemsrnd"])) {
                        // itemsrnd = item_type:chance:min_count:max_count
                        $irnd = explode("|", $npc["itemsrnd"]);
                        foreach (array_keys($irnd) as $k) {
                            if ($irnd[$k]) {
                                $trnd  = explode(":", $irnd[$k]);
                                $trndc = round(rand($trnd[2], $trnd[3]));
                                if (rand(0, 100) <= $trnd[1] && $trndc > 0) {
                                    if (empty($npc["items"])) {
                                        $npc["items"] = $trnd[0] . ":" . $trndc;
                                    } else {
                                        $npc["items"] .= "|" . $trnd[0] . ":" . $trndc;
                                    }
                                }
                            }
                        }
                        unset($npc["itemsrnd"]);
                    }

                    // респавн текущий
                    $loc_i[$i][$tid] = $npc;
                    unset($loc_t[$i][$j]);
                    continue;
                }
                // респ предметов
                if (substr($loc_t[$i][$j], 0, 2) == "i.") {
                    $tmp  = explode("|", $loc_t[$i][$j]);
                    $item = findItemByBaseId($tmp[0]);
                    $tc   = rand($tmp[1], $tmp[2]);
                    if ($tc > 0) {
                        $loc_i[$i][$tmp[0]] = $item[0] . "|" . $tc . "|0";
                    } else {
                        unset($loc_i[$i][$tmp[0]]);
                    }
                    addtimer($i, $j, rand($tmp[3], $tmp[4]), $loc_t[$i][$j], 1);
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
        srand((float)microtime() * 10000000);
        for ($k = 0; $k < 1; $k++) {
            $id             = "n.g." . rand(5, 9999);
            $title          = rndname() . " [стража]";
            $loc_i[$i][$id] = [
                "char"  => $title . "|1000|1000|100|100|" . time() . "1||||||" . (time() + 600),
                "war"   => "100|100|100|2|0|10|20|0|0|10|30|40|алебардой|0||",
                "items" => "i.w.t.alebarda:1",
                "equip" => "i.w.t.alebarda"
            ];
            addjournal($i, "all", "Появился " . $title);
        }
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
                            $b    = 0;
                            $k    = $locai[2 + 2 * rand(0, (count($locai) - 2) / 2 - 1) + 1];
                            $loc1 = explode("|", $loc_tt[$k]["d"]);
                            if ($locai[1] == $loc1[1]) {
                                addjournal($loc, "all", $char[0] . " убегает");
                                if ($char[10]) {
                                    $move     = explode(":", $char[10]);
                                    $move[3]  = time() + rand($move[1], $move[2]);
                                    $char[10] = implode(":", $move);
                                }
                                $char[7]               = "";
                                $loc_i[$i][$j]["char"] = implode("|", $char);
                                addnpc($j, $i, $k);
                                $b = 1;
                            }
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
 *
 * Загружает состояние локации в `$loc_tt[$loc]`
 *
 * Копирует список объектов в `$loc_i[$loc]`
 *
 * Копирует список таймеров в `$loc_t[$loc]`
 *
 * @param string $loc ID локации
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
 * Перемещение НПС/пользователей.
 *
 * Выполняет перемещение между локациями и удаление из локации.
 *
 * FIXME: название не соответствует, так как в основном используется перемещения/удаления
 *
 * @param string $id   ID NPC/пользователя
 * @param string $from откуда, ID локации
 * @param string $to   куда, ID локации
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
    $tnpc = [];
    // уход из локации $from
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
    // появление в локации $to
    if ($to && isset($loc_i[$to])) {
        if ( ! $tnpc && isset($loc_i[$to][$id])) {
            // по сути является костылем
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
                if (substr($id, 0, 2) == "n.") {
                    // история следов npc
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
 * Вспомогательная функция для перерасчета длины строк в файлах состояния.
 *
 * Требуется в основном после ручной правки файлов состояния.
 *
 * @param string $s строка для перерачета
 *
 * @return string
 */
function calcser($s)
{
    return "s:" . strlen($s) . ":\"" . $s . "\";";
}

/**
 * @param string $itemId
 * @return string
 */
function getBaseItemId($itemId)
{
    $position = strpos($itemId, "_");
    // является самодельным предметом?
    if ($position !== false) {
        $baseId = substr($itemId, 0, $position);
    } else {
        $baseId = $itemId;
    }
    return $baseId;
}

/**
 * @param string $baseId
 * @return string[]
 */
function findItemByBaseId($baseId)
{
    if (substr($baseId, 0, 5) == "i.rr.") {
        $item = explode("|", "руна|50");
    } else {
        $filename = BASE_DIR . "items/" . $baseId;
        if (file_exists($filename)) {
            $item = explode("|", file_get_contents($filename));
        } else {
            // TODO: replace with exception
            die("err: no items/" . $baseId);
        }
    }
    return $item;
}

/**
 * @param $itemId
 * @return string[]
 */
function findItemByFullId($itemId)
{
    return findItemByBaseId(getBaseItemId($itemId));
}