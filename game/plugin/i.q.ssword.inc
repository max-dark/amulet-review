<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global to
 * @global char
 */
// действует на цель как оружие

if ($to) {    // атакуем как магией
    if (isset($loc_i[$loc][$to])) {
        $skills = explode("|", $loc_i[$loc][$login]["skills"]);
        if ($skills[0] < 5 || $skills[1] < 4) {
            msg("Необходимо минимум сила 5 и ловкость 4");
        }
        $war     = explode("|", $loc_i[$loc][$login]["war"]);
        $war[0]  = 100;
        $war[1]  = 0;
        $war[2]  = 90;
        $war[3]  = 7;
        $war[4]  = 0;
        $war[12] = "стекл.мечом";
        $war[14] = "";
        $war     = implode("|", $war);

        manageItems($loc, $login, "", "i.q.ssword", 1, "items", "items", 1, 0, 1);
        attack($loc, $login, $to, $war);
    } else {
        addjournal($loc, $login, "Цель недоступна");
    }
} else {
    $list = 'all';
}    // выведем ниже список целей