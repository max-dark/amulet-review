<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global num
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global $look
 * @global obj
 * @global to
 */

$macro = $cm;
if ($macro) { //macro=obj|objv|to|tov|name/..., macrol=obj|objv|to|tov
    if ($macro == "save") { // сохраняем
        $name = Request('name');
        $num = intval(Request('num'));
        if (!$name) {
            msg("Не указано имя макроса, используйте англ. буквы.", "Макросы", 0);
        }
        if ($num < 1 || $num > 8) {
            msg("Диапазон номера от 1 до 8", "Макросы", 0);
        }
        $m = getMacroList($loc_i[$loc][$login]);
        if (!isset($loc_i[$loc][$login]["macrol"])) {
            $ml = [];
        } else {
            $ml = explode("|", $loc_i[$loc][$login]["macrol"]);
        }
        $objv = '';
        $tov = '';
        if ($obj == "curr") {
            $obj = $ml[0];
            $objv = $ml[1];
        }
        if ($to == "curr") {
            $to = $ml[2];
            $tov = $ml[3];
        }
        if ($to == "none") {
            $to = "";
            $tov = "";
        }
        $name = substr(preg_replace('/\W/', "", $name), 0, 9);
        $m[$num - 1] = "$obj|$objv|$to|$tov|$name";
        if (strlen($loc_i[$loc][$login]["macro"]) > 300) {
            msg("Слишком много или слишком длинные макросы");
        }
        $loc_i[$loc][$login]["macro"] = implode("/", $m);
        msg("Макрос с именем $name сохранен.", "Макросы", 0);

    }

    if ($macro == "del") { // удаляем
        if ($num < 1 || $num > 8) {
            msg("Диапазон номера от 1 до 8");
        }
        $m = getMacroList($loc_i[$loc][$login]);
        unset($m[$num - 1]);
        $loc_i[$loc][$login]["macro"] = implode("/", $m);
        msg("Макрос с номером $num удален.", "Макросы", 0);
    }

    if ($macro == "new") { // форма для нового макроса
        $stmp = "Действие:<br/>
		<select class=\"form-control\" name=\"obj\" value=\"curr\">\n<option value=\"curr\">Текущее</option>\n<option value=\"last\">Последнее</option></select>
		Цель:<br/>
		<select class=\"form-control\" name=\"to\" value=\"curr\">\n<option value=\"curr\">Текущая</option>\n<option value=\"last\">Последняя</option>\n<option value=\"none\">Вручную</option></select>
		Имя макроса:<br/>
		<input class=\"form-control\" name=\"name\" value=\"macro\" />
		<br/>Номер:
		<select class=\"form-control\" name=\"num\" value=\"1\">
		";
        $m = getMacroList($loc_i[$loc][$login]);
        for ($i = 1; $i < 9; $i++) {
            if (!empty($m[$i - 1])) {
                $mn = explode("|", $m[$i - 1]);
                $mn = $mn[4];
            } else {
                $mn = "Пусто";
            }
            $stmp .= "\n<option value=\"$i\">$i: $mn</option>";
        }
        $stmp .= "</select>
		<a class=\"btn btn-primary m-1\" href=\"$PHP_SELF?sid=$sid&cm=save&obj=$(obj)&to=$(to)&name=$(name)&num=$(num)\">Сохранить</a>
		<br/>
		<a class=\"btn btn-danger m-1\" href=\"$PHP_SELF?sid=$sid&cm=del&num=$(num)\"/>Удалить</a>
		<br/>
		<a class=\"btn btn-secondary m-1\" href=\"$PHP_SELF?sid=$sid&cm=help\"/>Помощь</a>
		";

        msg($stmp, "Макросы", 0);
    }

    if ($macro == "help") {
        msg(
            "Чтобы записать новый макрос, совершите любое действие, например, используйте навык или заклинание и выберите в меню \"макросы\".<br/>\"Текущее\" - это совершенное действие и текущая цель.<br/>\"Последнее\" - последнее действие или цель перед вызовом макроса.<br/>\"Вручную\" - при вызове макроса надо будет вручную указать на кого применить.",
            "Макросы",
            0
        );
    }

    if ($macro < 1 || $macro > 8) {
        msg("Макрос может быть от 1 до 8", "Макросы", 0);
    }
}
