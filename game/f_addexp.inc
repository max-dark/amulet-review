<?php

/**Добавляет экспу
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
            calcparam($loc, $to);
        } //levelup
    }
}
