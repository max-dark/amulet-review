<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global game
 * @global char
 */
// i.log + 5 перьев = 5 стрел

$cp = intval(preg_replace('/.*' . "i.h.p." . '[^:]+:(\d+).*/', "\\1", $loc_i[$loc][$login]["items"]));
if (intval(preg_replace('/.*' . "i.log" . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["items"])) > 0 && $cp >= 5) {

    $skills = explode("|", $loc_i[$loc][$login]["skills"]);
    if ($skills[27] > 0) {
        $char[6]                     = time() + 6;
        $loc_i[$loc][$login]["char"] = implode("|", $char);

        manageItems($loc, $login, "", preg_replace('/.*(i\.h\.p\.[^:]+):\d+.*/', "\\1", $loc_i[$loc][$login]["items"]), 5,
            "items", "items", 1, 0, 1);        // удаляем 5 перьев

        if (rand(1, 100) < $skills[27] * 20) {
            manageItems($loc, $login, "", "i.log", 1);
            //		addjournal($loc,$login,"Вы сделали 5 стрел"); 
            addjournal($loc, "all", $char[0] . " сделал 5 стрел", $login);
            manageItems($loc, "", $login, "i.arrow", 5);
        } else {
            addjournal($loc, $login, "Вы не смогли сделать стрелы");
            addjournal($loc, "all", $char[0] . " пытался сделать стрелы", $login);
        }

    } else {
        addjournal($loc, $login, "Ваш навык плотника равен 0, вы не умеете делать стрелы");
    }
} else {
    addjournal($loc, $login, "Чтобы делать стрелы, нужна ветка от дерева и 5 перьев");
}

