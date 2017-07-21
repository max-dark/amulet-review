<?php
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