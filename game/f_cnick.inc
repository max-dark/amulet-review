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
 */

use MaxDark\Amulet\OldCode\ViewOptions;

$char = explode("|", $loc_i[$loc][$login]["char"]);
if (strpos($char[0], " *") === false) {
    $clan = "";
} else {
    $clan    = substr($char[0], strpos($char[0], " *"));
    $char[0] = substr($char[0], 0, strpos($char[0], " *"));
}
$tus = explode("|", $loc_i[$loc][$login]["user"]);

$viewOpt = ViewOptions::getInstance();

if ($cnick == "1") {
    $stmp = "<p>Ваше имя: <br/><input name=\"new\" value=\"" . $char[0] . "\"/>
	<br/>Возраст: <br/><input name=\"age\" value=\"" . intval($tus[3]) . "\" format=\"*N\" maxlength=\"2\"/>
	<br/>Размер списков (3..30): <br/><input name=\"list\" value=\"" . $viewOpt->getListsSize() . "\" format=\"*N\" maxlength=\"2\"/>
	<br/>Размер страницы (700..15000): <br/><input name=\"size\" value=\"" . $viewOpt->getMaxPageSize() . "\"  format=\"*N\" maxlength=\"5\"/>
	<br/>Тип меню: 0 - полное, 1 - сокращенное, 2 - на отдельной странице, 3 - в виде ссылок (если не видно обычное меню): <br/><input name=\"menu\" value=\"" .
        $viewOpt->getMenuMode() . "\" format=\"*N\" maxlength=\"1\"/>
	<br/>Карта: 0 - нет, 1 - ч/б, 2 - цветная JPEG, 3 - цветная PNG: <br/><input name=\"map\" value=\"" .
        $viewOpt->getMapMode() . "\" format=\"*N\" maxlength=\"1\"/>
	<br/>Пункты в меню (0-откл,1-магия,2-предмет,3-прием) и кол-во горячих клавиш для каждого пункта (0..9), порядок произвольный. Пример: 332110<br/><input name=\"smenu\" value=\"" .
            intval($viewOpt->getUserMenu()) . "\" format=\"*N\" maxlength=\"6\"/>
	<br/>Описание локаций (1-вкл,0-выкл): <br/><input name=\"j2go\" value=\"" . $viewOpt->getShowDesc() . "\" format=\"*N\" maxlength=\"1\"/>
	<br/>Сообщать о приходящих (1-вкл,0-выкл): <br/><input name=\"j2loc\" value=\"" . $viewOpt->getReportIncoming() . "\" format=\"*N\" maxlength=\"1\"/>
	<br/>Звуки рядом с выходами (1-вкл,0-выкл): <br/><input name=\"sounds\" value=\"" . $viewOpt->getSoundsMode() . "\" format=\"*N\" maxlength=\"1\"/>
	<br/>Отключить журнал (1-да,0-нет): <br/><input name=\"joff\" value=\"" . $viewOpt->getJournalDisabled() . "\" format=\"*N\" maxlength=\"1\"/>
	<br/>Маленький шрифт (1-да,0-нет): <br/><input name=\"smf\" value=\"" . $viewOpt->getUseSmallFont() . "\" format=\"*N\" maxlength=\"1\"/>
	<br/><a href=\"$PHP_SELF?sid=$sid&cnick=$(new)&list=$(list)&size=$(size)&j2loc=$(j2loc)&j2go=$(j2go)&menu=$(menu)&smenu=$(smenu)&sounds=$(sounds)&joff=$(joff)&map=$(map)&smf=$(smf)&age=$(age)\">Сохранить</a>
	<br/><a href=\"?site=faq&id=err\">Помощь</a>";

} else {
    // get vars from request
    $age = Request('age');
    $size = Request('size');
    $list = Request('list');
    $j2loc = Request('j2loc');
    $j2go = Request('j2go');
    $menu = Request('menu');
    $sounds = Request('sounds');
    $joff = Request('joff');
    $smenu = Request('smenu');
    $map = Request('map');
    $smf = Request('smf');

    if (strtolower($cnick) != strtolower($char[0])) {
        msg("<p>Нельзя поменять имя на другое (было имя: " . $char[0] . ", вы ввели новое: " . $cnick .
            "), можно только изменить его написание (например, сделать первую букву заглавной).");
    }
    if (intval($age) < 5 || intval($age) > 99) {
        msg("Неправдоподобный возраст");
    }
    if (intval($list) < 3 || intval($list) > 30) {
        msg("Диапазон размера списков от 3 до 30");
    }
    if (intval($size) < 700 || intval($size) > 15000) {
        msg("Диапазон размера страницы от 700 до 15000");
    }
    if (intval($j2loc) > 1) {
        msg("Допустимые значения для инфы о пришедших 0 и 1");
    }
    if (intval($j2go) > 1) {
        msg("Допустимые значения для инфы о локации 0 и 1");
    }
    if (intval($joff) > 1) {
        msg("Допустимые значения для журнала 0 и 1");
    }
    if (intval($smf) > 1) {
        msg("Допустимые значения для шрифта 0 и 1");
    }
    if (intval($menu) > 3) {
        msg("Допустимые значения для меню 0, 1, 2 и 3");
    }
    if (intval($map) > 3) {
        msg("Допустимые значения для карты 0,1,2 и 3");
    }

    $viewOpt->setListsSize($list);
    $viewOpt->setMaxPageSize($size);
    $viewOpt->setReportIncoming($j2loc);
    $viewOpt->setShowDesc($j2go);
    $viewOpt->setMenuMode($menu);
    $viewOpt->setSoundsMode($sounds);
    $viewOpt->setJournalDisabled($joff);
    $viewOpt->setUserMenu($smenu);
    $viewOpt->setMapMode($map);
    $viewOpt->setUseSmallFont($smf);

    $loc_i[$loc][$login]["o"]    = $viewOpt->toString();
    $char[0]                     = $cnick . $clan;
    $loc_i[$loc][$login]["char"] = implode("|", $char);
    $tus[3]                      = intval($age);
    $loc_i[$loc][$login]["user"] = implode("|", $tus);
    $stmp                        = "Настройки сохранены.";
}

msg($stmp, "Настройки", 0);