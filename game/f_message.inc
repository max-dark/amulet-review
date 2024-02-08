<?php
/**
 * Функция вывода страницы
 *
 * @file /game/f_message.inc
 * @lang text
 */

use MaxDark\Amulet\OldCode\MenuMode;
use MaxDark\Amulet\OldCode\PageType;
use MaxDark\Amulet\OldCode\ViewOptions;

/**
 * Выводит страницу пользователю.
 * 
 * Использование
 * В большинстве случаев вызывается с одним параметром: `msg('some text');`
 * Другие варианты использования
 * f_cnick.inc: `msg($stmp, "Настройки", 0);`
 * f_drop.inc: `msg($stmp, "Бросить", 1, PageType::MSG, "num", $tcount);`
 * f_listinv.inc: `msg($stmp, "[" . date("H:i", time()) . "] Предметы ", 1, PageType::INVENTORY);`
 * f_listmagic.inv: `msg($stmp, "Магия", 1);`
 * f_listpriem.inc: `msg($stmp, "Приемы", 1);`
 * f_listskill.inc: `msg($stmp, "Навыки", 1);`
 * f_logout.inc: `msg($logout_msg['text'], $logout_msg['title'], $logout_msg['show_journal']);`
 * 
 * Поведение:
 * Требует писец как много глобальных значений
 * Вызывает savegame
 * Завершает выполнение скрипта(exit в конце)
 *
 * @param string $message      Сообщение
 * @param string $page_title   Заголовок страницы
 * @param bool   $show_journal флаг, что нужно выводить журнал
 * @param string $page_type    тип страницы. возможные значения PageType::(MSG|GUEST|MAIN|INVENTORY)
 *      PageType::MSG - "диалоговая" страница(уточнить)
 *      PageType::GUEST - "гостевая" страница - без меню и журнала(уточнить)
 *      PageType::MAIN - основная страница(уточнить) {@see game/index.php}
 *      PageType::INVENTORY - список действий в инвентаре(?) {@see game/f_listinv.inc}
 * 
 * @param string $var_name     имя переменной для вставки в страницу
 * @param string $var_value    значение переменной var_name
 * 
 * @return void
 */
function msg(
    $message,
    $page_title = 'Амулет Дракона',
    $show_journal = true,
    $page_type = PageType::MSG,
    $var_name = '',
    $var_value = ''
) {
    global $game, $g_admin, $gm, $login, $loc, $loc_i, $sid, $gm_id;
    global $g_query_string;

    $viewOpt = ViewOptions::getInstance();
    $url_base = "?sid=$sid";

    // header("Expires: Thu, 01 Jan 2010 00:00:01 GMT");
    // header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
    // header("Cache-Control: no-cache, no-store, must-revalidate, max-age=0");
    // header("Pragma: no-cache");
    // header("Content-type:text/vnd.wap.wml;charset=utf-8");

    /**
     * флаг пропуска журнала.
     * 
     * Используется совмесно с is_page_mode_menu и is_links_mode_menu
     * 
     * @var $skip_journal bool
     */
    $skip_journal = Request('cj');

    /**
     * Флаг, что перешли с главной страницы.
     * Как комманда показать меню в виде "действий"(<do></button> tag).
     *
     * is_page_mode_menu = (!$is_page_mode_menu && $page_type == PageType::MAIN && $menuMode == MenuMode::PAGE)
     * is_page_mode_menu = is_page_mode_menu && ! is_links_mode_menu;
     *
     * @var $isMenuInPageMode bool
     */
    $isMenuInPageMode = Request('fm');
    /**
     * Флаг, что перешли с главной(page_type == 'main') страницы.
     * Как комманда показать меню в виде ссылок(<a></a> tag) на отдельной странице.
     *
     * is_links_mode_menu = (!$is_links_mode_menu && $page_type == PageType::MAIN && $menuMode == MenuMode::LINKS)
     * is_links_mode_menu = is_links_mode_menu && ! is_page_mode_menu;
     *
     * @var $isMenuInLinksMode bool
     */
    $isMenuInLinksMode = Request('fm2');

    /**
     * @var $wml string код страницы
     */
    $wml = '';

    // для "гостя"
    if (empty($login)) {
        $show_journal = false;
        $page_type = PageType::GUEST;
        $userJournal = '';
        $currentUser = [];
    } else {
        $currentUser = $loc_i[$loc][$login];
        $userJournal = $loc_i[$loc][$login]['journal'];
        $loc_i[$loc][$login]['journal'] = '';
    }

    savegame();

    // меню в виде "действий"(<do> tag).
    if ($isMenuInPageMode) {
        $show_journal = false;
        $viewOpt->setDescEnabled(false);
        $message = "Нажмите кнопку Меню";
    }
    // меню в виде ссылок(<a> tag) на отдельной странице
    if ($isMenuInLinksMode) {
        $show_journal = false;
        $viewOpt->setDescEnabled(false);
        $message = menu_links($url_base, $currentUser, $viewOpt->getMapMode());
    }

    #start: подготовка журнала
    if ($skip_journal) {
        $show_journal = false;
    } else {
        if ($viewOpt->getJournalDisabled()) {
            /* 
             * Данная регулярка удаляет строки не содержащие ':' или '!'
             * несколько '|' идущих подряд заменяет на один
             * Например после обработки 'del0|del1|keep0:|keep1!|del2'
             * останется '|keep0:|keep1!|'
             * */
            $userJournal = preg_replace('/(\||^)[^:!]*(\||$)/u', '|', $userJournal);
        }
    }
    $page_journal = "";
    // добавить глобальное оповещение в начало журнала
    if (have_key($game, 'journal') && $login != $g_admin && $gm != $gm_id) {
        $page_journal = $game['journal'];
    }

    if ($show_journal && $userJournal) {
        $page_journal = str_replace('|', '<br/>', $userJournal);
        if (!$viewOpt->getReportIncoming()) {
            $page_journal = preg_replace('/<br\/>(Пришел|Пришла) [^<]+/u', '', $page_journal);
        }
    }
    #end: подгоновка журнала

    //#start: формирование журнала
    if ($page_journal && $show_journal) {
        // урезать количество записей в журнале
        $pieces = explode('<br/>', $page_journal);
        if (count($pieces) > $viewOpt->getMaxListSize()) {
            array_splice($pieces, 0, count($pieces) - $viewOpt->getMaxListSize());
            $page_journal = implode('<br/>', $pieces);
        }
    }
    // добавить описание локации
    if ($viewOpt->getDescEnabled() && file_exists('loc_f/' . $loc)) {
        $page_journal .= '<br/>' . file_get_contents('loc_f/' . $loc);
    }

    if ($page_journal && substr($page_journal, 0, 5) == '<br/>') {
        $page_journal = substr($page_journal, 5);
    }
    // проверить размер страницы
    $bsize = strlen($message . $page_journal) < $viewOpt->getMaxPageSize();
    // если журнал не пуст и задано его отображение
    if ($page_journal && $show_journal) {
        // страница влезает в ограничения?
        if ($bsize) {
            $tmp_url = "#g";
        } else {
            // FIXME: костыль
            // иначе вывести продолжение на отдельную страницу
            $tmp_url = '?' . preg_replace('/(ci|use|say|ca|drop|take|to|adm|cm|go)=/u', 'c1=', $g_query_string);
        }
        $wml .= buildJournalCard($tmp_url, $page_journal, $skip_journal);
    }
    #end: формирование журнала

    #start: формирование страницы
    if ($bsize || !$show_journal || !$page_journal) {
        $wml .= buildMainCard(
            $page_title,
            $message,
            $page_type,
            $currentUser,
            $var_name,
            $var_value,
            $url_base,
            $isMenuInPageMode,
            $isMenuInLinksMode,
            $viewOpt->getMenuMode(),
            $viewOpt->getMapMode()
        );
    }
    #end: формирование страницы
    // Используется "маленикий шрифт"?
    if ($viewOpt->getUseSmallFont() && strpos($wml, '<input') === false) {
        $wml = preg_replace([
            "'(<p [^>]*>)'u",
            "'<p>'u",
            "'</p>'u"
        ], ['\\1<div>', '<p><div class="visible">', '</div></p>'], $wml);
    }

    // wml не позволяет использовать '&' отдельно, заменяем на "&amp;"
    // $wml = str_replace("&amp;", "&", $wml);
    // $wml = str_replace("&", "&amp;", $wml);

    $page = <<<WML
<?xml version="1.0"?>
<!DOCTYPE html>
<html>
<head>
 <meta charset="utf-8">
 <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
 <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
 <script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
 <script src="https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
 <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
 <script src="/game/functions.js"></script>
</head>
<body>
<div class="container">
<div class="jumbotron">
$wml
</div>
</div>
</body>
</html>
WML;
    echo $page;
    exit;
}

/**
 * @param string $pageTitle
 * @param string $message
 * @param string $pageType
 * @param string[] $userData
 * @param string $var_name
 * @param string $var_value
 * @param string $url_base
 * @param bool $isMenuInPageMode
 * @param bool $isMenuInLinksMode
 * @param int $menuMode
 * @param int $mapMode
 * @return string
 */
function buildMainCard(
    $pageTitle,
    $message,
    $pageType,
    $userData,
    $var_name,
    $var_value,
    $url_base,
    $isMenuInPageMode,
    $isMenuInLinksMode,
    $menuMode,
    $mapMode
) {
    $wml = '<div id="g" title="' . $pageTitle . '"';
    // включить автообновление для главной страницы
    if ($pageType == PageType::MAIN) {
        $wml .= " ontimer=\"$url_base\"><timer value=\"600\"/";
    }
    $wml .= ">";
    // добавить переменную
    // испрользуется только при $pageType == PageType::MSG
    if ($var_name) {
        $wml .= '<onevent type="onenterforward"><refresh>' .
            "<setvar name=\"$var_name\" value=\"$var_value\"/>" .
            '</refresh></onevent>';
    }

    #start: формирование меню
    if ($pageType == PageType::MSG || ($pageType == PageType::INVENTORY && $menuMode != MenuMode::SHORT)) {
        $wml .= "<a class=\"btn btn-outline-primary m-1\" href=\"$url_base\">В игру</a>";
        $wml .= '<button class="btn btn-outline-secondary m-1" name="b2" type="accept" label="Назад" onclick="history.back()"><prev>Назад</prev></button>';
    }
    $o = 4;

    // только для $pageType == PageType::MAIN
    // добавить ссылку на меню в режиме MenuMode::PAGE
    if ($pageType == PageType::MAIN && $menuMode == MenuMode::PAGE && !$isMenuInPageMode) {
        $wml .= '<button name="o2" type="options" label="Меню">' .
            "<a href=\"$url_base&fm=1&cj=1\">Меню</a></button>";
        // дальше выводим как "диалоговую"
        $pageType = PageType::MSG;
    }

    // только для $pageType == PageType::MAIN
    // добавить ссылку на меню в режиме MenuMode::LINKS
    if ($pageType == PageType::MAIN && $menuMode == MenuMode::LINKS && !$isMenuInLinksMode) {
        // FIXME: тот еще костыль...
        // добавляет на главной странице ссылку на страницу с меню.
        $message = str_replace(
            "</p></div><div id=\"m\"",
            "<br/><a href=\"$url_base&fm2=1&cj=1\">Меню</a></p></div><div id=\"m\"",
            $message
        );
        // дальше выводим как "диалоговую"
        $pageType = PageType::MSG;
    }

    // только для $pageType == PageType::MAIN
    // доп условие: ($menuMode != MenuMode::LINKS)
    // добавляет список макросов
    if (($pageType == PageType::MAIN || $isMenuInPageMode) && !$isMenuInLinksMode) {
        $wml .= "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base&cl=i&cj=1\">Пeрcoнaж</a>";
        $m = getMacroList($userData);
        for ($i = 1; $i < 9; $i++) {
            if (!empty($m[$i - 1])) {
                $mn = explode("|", $m[$i - 1]);
                $wml .= "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base&cm=$i\">" . $mn[4] . "</a>";
                $o++;
            }
        }
    }

    if ($pageType == PageType::INVENTORY && $menuMode == MenuMode::SHORT) {
        $wml .= "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base\">В игру</a>";
    }

    if (
        $pageType == PageType::INVENTORY && $menuMode == MenuMode::SHORT ||
        $menuMode == MenuMode::FULL && $pageType == PageType::MAIN ||
        $isMenuInPageMode
    ) {
        $wml .= "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base&cs=1&cj=1\">Cкaзaть</a>" . // чат
            "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base&msg=1&cj=1\">Koнтaкты</a></button>" . // список контактов
            "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base&cm=new\">мaкpocы</a></button>" // управление макросами
        ;
        // Карта включена?
        if ($mapMode) {
            $o++;
            $wml .= "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base&map=$mapMode\">Kapтa</a></button>";
        }
        $o++;
        $wml .= "<a class=\"btn btn-outline-secondary m-1\" href=\"$url_base&ce=1\">Coxpaнить</a></button>";
    }
    #end: формирование меню

    if (substr($message, 0, 2) != '<p') {
        $message = "<p>" . $message;
    }
    if (substr($message, strlen($message) - 4) != '</p>') {
        $message .= '</p>';
    }
    $wml .= $message . '</div>';
    return $wml;
}

/**
 * @param string[] $userData
 * @return string[]
 */
function getMacroList($userData)
{
    if (!empty($userData['macro'])) {
        $m = explode("/", $userData['macro']);
    } else {
        $m = ["", "", "", "", "", "", "", ""];
    }
    return $m;
}

/**
 * @param string $tmp_url
 * @param string $journal
 * @param bool $skip_journal
 * @return string
 */
function buildJournalCard($tmp_url, $journal, $skip_journal)
{

    $journalCard = '<div title="Журнал" class="alert alert-warning alert-dismissible fade show" role="alert">' .
        '<button type="button" class="close" data-dismiss="alert" aria-label="Close">' .
        '<span aria-hidden="true">&times;</span>' .
        '</button>' .
        '<p>' . $journal .
        '<br/>';
    $journalCard .= '</p></div>';
    return $journalCard;
}

/**
 * @param string $url_base
 * @param string[] $userData
 * @param        $map_type
 *
 * @return string
 */
function menu_links($url_base, $userData, $map_type)
{
    $message = sprintf('Главное меню:<br/><a href="%s&cl=i&cj=1">Пepcoнaж</a>', $url_base);
    $macro_list = getMacroList($userData);
    for ($i = 1; $i < 9; $i++) {
        if ($macro_list[$i - 1]) {
            $current_macro = explode("|", $macro_list[$i - 1]);
            $message .= sprintf('<br/><a href="%s&cm=%d">%s</a>', $url_base, $i, $current_macro[4]);
        }
    }
    $message .= sprintf('<br/><a href="%s&cs=1&cj=1">Cкaзaть</a>', $url_base) .
        sprintf('<br/><a href="%s&msg=1&cj=1">Koнтaкты</a>', $url_base) .
        sprintf('<br/><a href="%s&cm=new">мaкpocы</a>', $url_base);
    if ($map_type) {
        $message .= sprintf('<br/><a href="%s&map=%d">Kapтa</a>', $url_base, $map_type);
    }
    $message .= sprintf('<br/><a href="%s&ce=1">Coxpaнить</a>', $url_base) .
        sprintf('<br/><br/><a href="%s">В игру</a>', $url_base);
    return $message;
}
