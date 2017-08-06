<?php
/**
 * Функция вывода страницы
 *
 * @file /game/f_message.inc
 * @lang text
 */

use MaxDark\Amulet\OldCode\MenuMode;

/**
 * Выводит страницу пользователю.
 * 
 * Использование
 * В большинстве случаев вызывается с одним параметром: `msg('some text');`
 * Другие варианты использования
 * f_cnick.inc: `msg($stmp, "Настройки", 0);`
 * f_drop.inc: `msg($stmp, "Бросить", 1, "", "num", $tcount);`
 * f_listinv.inc: `msg($stmp, "[" . date("H:i", time()) . "] Предметы ", 1, 'inv');`
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
 * @param string $page_type    тип страницы. возможные значения (""|none|main|inv)
 *      ""   - "диалоговая" страница(уточнить)
 *      none - "гостевая" страница - без меню и журнала(уточнить)
 *      main - основная страница(уточнить) {@see game/index.php}
 *      inv  - список действий в инвентаре(?) {@see game/f_listinv.inc}
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
    $page_type = '',
    $var_name = '',
    $var_value = ''
) {
    global $game, $g_admin, $gm, $login, $loc, $loc_i, $sid, $gm_id;
    global $page_d, $g_size, $g_list, $g_menu, $g_j2loc, $g_joff, $g_smf, $g_map;
    global $g_query_string;

    $url_base = "?sid=$sid";

    header("Expires: Thu, 01 Jan 2010 00:00:01 GMT");
    header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
    header("Cache-Control: no-cache, no-store, must-revalidate, max-age=0");
    header("Pragma: no-cache");
    header("Content-type:text/vnd.wap.wml;charset=utf-8");

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
     * Как комманда показать меню в виде "действий"(<do></do> tag).
     *
     * is_page_mode_menu = (!$is_page_mode_menu && $page_type == 'main' && $g_menu == Menu::mode_page)
     * is_page_mode_menu = is_page_mode_menu && ! is_links_mode_menu;
     *
     * @var $is_page_mode_menu bool
     */
    $is_page_mode_menu  = Request('fm');
    /**
     * Флаг, что перешли с главной(page_type == 'main') страницы.
     * Как комманда показать меню в виде ссылок(<a></a> tag) на отдельной странице.
     *
     * is_links_mode_menu = (!$is_links_mode_menu && $page_type == 'main' && $g_menu == Menu::mode_links)
     * is_links_mode_menu = is_links_mode_menu && ! is_page_mode_menu;
     *
     * @var $is_links_mode_menu bool
     */
    $is_links_mode_menu = Request('fm2');

    /**
     * @var $wml string код страницы
     */
    $wml = '';

    // для "гостя"
    if (empty($login)) {
        $show_journal = 0;
        $page_type    = 'none';
    }

    // меню в виде "действий"(<do> tag).
    if ($is_page_mode_menu) {
        $show_journal = 0;
        $page_d       = 0;
        $message      = "Нажмите кнопку Меню";
    }
    // меню в виде ссылок(<a> tag) на отдельной странице
    if ($is_links_mode_menu) {
        $show_journal = 0;
        $page_d       = 0;
        $message = menu_links($url_base, $loc_i[$loc][$login], $g_map);
    }
    
    #start: подготовка журнала
    if ($skip_journal) {
        $show_journal = 0;
    } else {
        if ($g_joff) {
            /* 
             * Данная регулярка удаляет строки не содержащие ':' или '!'
             * несколько '|' идущих подряд заменяет на один
             * Например после обработки 'del0|del1|keep0:|keep1!|del2'
             * останется '|keep0:|keep1!|'
             * */
            $loc_i[$loc][$login]['journal'] = preg_replace('/(\||^)[^:!]*(\||$)/u', '|',
                $loc_i[$loc][$login]['journal']);
        }
    }
    $page_journal = "";
    // добавить глобальное оповещение в начало журнала
    if (have_key($game, 'journal') && $login != $g_admin && $gm != $gm_id) {
        $page_journal = $game['journal'];
    }

    /// FIXME: PHP Notice:  Undefined index: ''
    if ($show_journal == 1 && $loc_i[$loc][$login]['journal']) {
        $page_journal                   = str_replace('|', '<br/>', $loc_i[$loc][$login]['journal']);
        $loc_i[$loc][$login]['journal'] = '';
        if ( ! $g_j2loc) {
            $page_journal = preg_replace('/<br\/>(Пришел|Пришла) [^<]+/u', '', $page_journal);
        }
    }
    #end: подгоновка журнала

    savegame();
    //#start: формирование журнала
    if ($page_journal && $show_journal) {
        // урезать количество записей в журнале
        $page_journal = explode('<br/>', $page_journal);
        if (count($page_journal) > $g_list) {
            array_splice($page_journal, 0, count($page_journal) - $g_list);
        }
        $page_journal = implode('<br/>', $page_journal);
    }
    // добавить описание локации
    if ($page_d && file_exists('loc_f/' . $loc)) {
        $page_journal .= '<br/>' . file_get_contents('loc_f/' . $loc);
    }

    // проверить размер страницы
    $bsize = strlen($message . $page_journal) < $g_size;
    if ($page_journal && substr($page_journal, 0, 5) == '<br/>') {
        $page_journal = substr($page_journal, 5);
    }
    // если журнал не пуст и задано его отображение
    if ($page_journal && $show_journal) {
        // страница влезает в ограничения?
        if ($bsize) {
            // отобразить через <card></card>
            $tmp_url = "#g";
        } else {
            // FIXME: костыль
            // иначе вывести продолжение на отдельную страницу
            $tmp_url = '?' . preg_replace('/(ci|use|say|ca|drop|take|to|adm|cm|go)=/u', 'c1=', $g_query_string);
        }
        $wml .= '<card title="Журнал">' .
                '<do type="accept" label="Дальше">' .
                '<go href="' . $tmp_url . '"/>' .
                '</do>' .
                '<p>' . $page_journal .
                '<br/><a href="' . $tmp_url . '">Дальше</a>';
        if ($tmp_url != "#g") {
            // BUG? бесполезное условие, т.к. $show_journal = !$skip_journal
            $tmp_url .= ($skip_journal ? '' : '&cj=1');
            $wml .= '/<a href="' . $tmp_url . '">к меню</a>/';
        }
        $wml .= '</p></card>';
    }
    #end: формирование журнала

    #start: формирование страницы
    if ($bsize || ! $show_journal || ! $page_journal) {
        $wml .= '<card id="g" title="' . $page_title . '"';
        // включить автообновление для главной страницы
        if ($page_type == 'main') {
            $wml .= " ontimer=\"$url_base\"><timer value=\"600\"/";
        }
        $wml .= ">";
        if ($var_name) {
            $wml .= '<onevent type="onenterforward"><refresh>' .
                    "<setvar name=\"$var_name\" value=\"$var_value\"/>" .
                    '</refresh></onevent>';
        }

        #start: формирование меню
        if ($page_type == '' || ($page_type == 'inv' && $g_menu != MenuMode::SHORT)) {
            $wml .= '<do name="b1" type="options" label="В игру">' .
                    "<go href=\"$url_base\"/></do>";
            $wml .= '<do name="b2" type="accept" label="Назад"><prev/></do>';
        }
        $o = 4;

        // только для $page_type == 'main':
        // добавить ссылку на меню в режиме Menu::mode_page
        if ($page_type == 'main' && $g_menu == MenuMode::PAGE && ! $is_page_mode_menu) {
            $wml .= '<do name="o2" type="options" label="Меню">' .
                    "<go href=\"$url_base&fm=1&cj=1\"/></do>";
            // дальше выводим как "диалоговую"
            $page_type = '';
        }

        // только для $page_type == 'main':
        // добавить ссылку на меню в режиме Menu::mode_links
        if ($page_type == 'main' && $g_menu == MenuMode::LINKS && ! $is_links_mode_menu) {
            // FIXME: тот еще костыль...
            // добавляет на главной странице ссылку на страницу с меню.
            $message   = str_replace("</p></card><card id=\"m\"",
                "<br/><a href=\"$url_base&fm2=1&cj=1\">Меню</a></p></card><card id=\"m\"", $message);
            // дальше выводим как "диалоговую"
            $page_type = '';
        }

        // только для $page_type == 'main':
        // доп условие: ($g_menu != Menu::mode_links)
        // добавляет список макросов
        if (($page_type == 'main' || $is_page_mode_menu) && ! $is_links_mode_menu) {
            $wml .= '<do name="b1" type="options" label="Пeрcoнaж">' .
                    "<go href=\"$url_base&cl=i&cj=1\"/></do>";
            if (array_key_exists('macro', $loc_i[$loc][$login])) {
                $m = explode("/", $loc_i[$loc][$login]["macro"]);
            } else {
                $m = [];
            }
            for ($i = 1; $i < 9; $i++) {
                if (!empty($m[$i - 1])) {
                    $mn = explode("|", $m[$i - 1]);
                    $wml .= "<do name=\"b$o\" type=\"options\" label=\"" . $mn[4] . "\">" .
                            "<go href=\"$url_base&cm=$i\"/></do>";
                    $o++;
                }
            }
        }
        
        if ($page_type == 'inv' && $g_menu == MenuMode::SHORT) {
            $wml .= '<do name="b1" type="options" label="В игру">' .
                    "<go href=\"$url_base\"/></do>";
        }
        /// FIXME: не нравится мне это условие, баг в нем чую...
        if ($page_type == 'inv' && $g_menu == MenuMode::SHORT ||
            $g_menu == MenuMode::FULL && $page_type == 'main' ||
            $is_page_mode_menu
        ) {
            $wml .= "<do name=\"b2\" type=\"options\" label=\"Cкaзaть\">" .
                    "<go href=\"$url_base&cs=1&cj=1\"/></do>" . // чат
                    "<do name=\"b3\" type=\"options\" label=\"Koнтaкты\">" .
                    "<go href=\"$url_base&msg=1&cj=1\"/></do>" . // список контактов
                    "<do name=\"b$o\" type=\"options\" label=\"мaкpocы\">" .
                    "<go href=\"$url_base&cm=new\"/></do>" // управление макросами
                    ;
            // Карта включена?
            if ($g_map) {
                $o++;
                $wml .= "<do name=\"b$o\" type=\"options\" label=\"Kapтa\">" .
                        "<go href=\"$url_base&map=$g_map\"/></do>";
            }
            $o++;
            $wml .= "<do name=\"b$o\" type=\"options\" label=\"Coxpaнить\">" .
                    "<go href=\"$url_base&ce=1\"/></do>";
        }
        #end: формирование меню

        if (substr($message, 0, 2) != '<p') {
            $message = "<p>" . $message;
        }
        if (substr($message, strlen($message) - 4) != '</p>') {
            $message .= '</p>';
        }
        $wml .= $message . '</card>';
    }
    #end: формирование страницы
    // Используется "маленикий шрифт"?
    if ($g_smf && strpos($wml, '<input') === false) {
        $wml = preg_replace([
            "'(<p [^>]*>)'u",
            "'<p>'u",
            "'</p>'u"
        ], ['\\1<small>', '<p><small>', '</small></p>'], $wml);
    }

    // wml не позволяет использовать '&' отдельно, заменяем на "&amp;"
    $wml = str_replace("&amp;", "&", $wml);
    $wml = str_replace("&", "&amp;", $wml);

    $page = <<<WML
<?xml version="1.0"?>
<!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML 1.3//EN" "http://www.wapforum.org/DTD/wml13.dtd">
<wml>$wml</wml>
WML;
    echo $page;
    exit;
}

/**
 * @param string $url_base
 * @param array  $current_user
 * @param        $map_type
 *
 * @return string
 */
function menu_links($url_base, $current_user, $map_type)
{
    $message = sprintf('Главное меню:<br/><a href="%s&cl=i&cj=1">Пepcoнaж</a>', $url_base);
    if (array_key_exists('macro', $current_user)) {
        $macro_list = explode("/", $current_user['macro']);
    } else {
        $macro_list = [];
    }
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
