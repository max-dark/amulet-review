<?php

/** Выводит страницу пользователю
 * Требует писец как много глобальных значений
 * Вызывает savegame
 * Завершает выполнение скрипта(exit в конце)
 * @param string $message Сообщение
 * @param string $page_title название игры
 * @param int $show_journal флаг, что нужно выводить журнал
 * @param string $menu_type тип меню. возможные значения (""|none|main|to|inv)
 * @param string $var_name имя переменной для вставки в страницу
 * @param string $var_value значение переменной vname
 */
function msg(
    $message, $page_title = 'Амулет Дракона',
    $show_journal = 1, $menu_type = '',
    $var_name = '', $var_value = ''
)
{
    global $game, $g_admin, $gm, $login, $loc, $loc_i, $page_d, $PHP_SELF, $sid,
           $gm_id, $g_size, $g_list, $skip_journal, $cj, $t_g, $g_menu, $fm, $fm2, $g_j2loc, $g_joff, $g_smf,
           $g_map, $g_tmp;

    $wml = "<wml>";
    if (!$login) {
        $show_journal = 0;
        $menu_type = 'none';
    }
    if ($fm) {
        $show_journal = 0;
        $page_d = 0;
        $message = "Нажмите кнопку Меню";
    }
    if ($fm2) {
        $show_journal = 0;
        $page_d = 0;
        // меню в виде ссылок на отдельной странице

        $message = 'Главное меню:' .
            "<br/><a href=\"$PHP_SELF?sid=$sid&cl=i&cj=1\">Пepcoнaж</a>";
        if (!isset($loc_i[$loc][$login]["macro"]))
            $m = [];
        else
            $m = explode("/", $loc_i[$loc][$login]["macro"]);
        for ($i = 1; $i < 9; $i++) {
            if ($m[$i - 1]) {
                $mn = explode("|", $m[$i - 1]);
                $message .= "<br/><a href=\"$PHP_SELF?sid=$sid&cm=$i\">" . $mn[4] . "</a>";
            }
        }
        $message .= "<br/><a href=\"$PHP_SELF?sid=$sid&cs=1&cj=1\">Cкaзaть</a>" .
            "<br/><a href=\"$PHP_SELF?sid=$sid&msg=1&cj=1\">Koнтaкты</a>" .
            "<br/><a href=\"$PHP_SELF?sid=$sid&cm=new\">мaкpocы</a>";
        if ($g_map) {
            $message .= "<br/><a href=\"$PHP_SELF?sid=$sid&map=" . $g_map . "\">Kapтa</a>";
        }
        $message .= "<br/><a href=\"$PHP_SELF?sid=$sid&ce=1\">Coxpaнить</a>" .
            "<br/><br/><a href=\"$PHP_SELF?sid=$sid\">В игру</a>";
    }
    if ($cj)
        $skip_journal = 1;
    if ($skip_journal)
        $show_journal = 0;
    else
        if ($g_joff)
            $loc_i[$loc][$login]["journal"] = preg_replace('/(\||^)[^:!]*(\||$)/', "|", $loc_i[$loc][$login]["journal"]);
    $page_journal = "";
    if (get($game, "journal") && $login != $g_admin && $gm != $gm_id)
        $page_journal = $game["journal"];
    if ($show_journal == 1 && $loc_i[$loc][$login]["journal"]) {
        $page_journal = str_replace("|", "<br/>", $loc_i[$loc][$login]["journal"]);
        $loc_i[$loc][$login]["journal"] = "";
        if (!$g_j2loc)
            $page_journal = preg_replace('/<br\/>(Пришел|Пришла) [^<]+/', "", $page_journal);
    }

    $t_g1 = sscanf(microtime(), "%s %s");
    $t_g1 = $t_g1[1] + $t_g1[0];
    $game["tmid"] = ($game["tmid"] + $t_g1 - $t_g) / 2;
    savegame();
    if ($page_journal && $show_journal) {
        $page_journal = explode("<br/>", $page_journal);
        if (count($page_journal) > $g_list)
            array_splice($page_journal, 0, count($page_journal) - $g_list);
        $page_journal = implode("<br/>", $page_journal);
    }
    if ($page_d && file_exists("loc_f/" . $loc))
        $page_journal .= "<br/>" . file_get_contents("loc_f/" . $loc);
    $bsize = strlen($wml . $message . $page_journal) < $g_size;
    if ($page_journal && substr($page_journal, 0, 5) == "<br/>")
        $page_journal = substr($page_journal, 5);
    if ($page_journal && $show_journal) {
        if ($bsize)
            $tmp_url = "#g";
        else {
            $tmp_url = "$PHP_SELF?" . preg_replace("/(ci|use|say|ca|drop|take|to|adm|cm|go)=/", "c1=", $g_tmp);
        }
        $wml .= '<card title="Журнал">' .
            '<do type="accept" label="Дальше">' .
                '<go href="' . $tmp_url . '"/>' .
            '</do>' .
            '<p>' .
                $page_journal .
                '<br/><a href="' . $tmp_url . '">Дальше</a>';
        if ($tmp_url != "#g") {
            $tmp_url .= ($skip_journal ? "" : "&cj=1");
            $wml .= '/<a href="' . $tmp_url . '">к меню</a>/';
        }
        $wml .= "</p></card>";
    }

    if ($bsize || !$show_journal || !$page_journal) {
        $wml .= '<card id="g" title="' . $page_title . '"';
        if ($menu_type == 'main')
            $wml .= " ontimer=\"$PHP_SELF?sid=$sid\"><timer value=\"600\"/";
        $wml .= ">";
        if ($var_name)
            $wml .= "<onevent type=\"onenterforward\"><refresh><setvar name=\"$var_name\" value=\"$var_value\"/></refresh></onevent>";

        if ($menu_type == '' || $menu_type == 'inv' && $g_menu != 1) {
            $wml .= "<do name=\"b1\" type=\"options\" label=\"В игру\"><go href=\"$PHP_SELF?sid=$sid\"/></do>";
            $wml .= "<do name=\"b2\" type=\"accept\" label=\"Назад\"><prev/></do>";
        }
        $o = 4;
        if ($menu_type == 'main' && $g_menu == 2 && !$fm) {
            $wml .= "<do name=\"o2\" type=\"options\" label=\"Меню\"><go href=\"$PHP_SELF?sid=$sid&fm=1&cj=1\"/></do>";
            $menu_type = '';
        }
        if ($menu_type == 'main' && $g_menu == 3 && !$fm2) {
            $message = str_replace(
                "</p></card><card id=\"m\"",
                "<br/><a href=\"$PHP_SELF?sid=$sid&fm2=1&cj=1\">Меню</a></p></card><card id=\"m\"",
                $message);
            $menu_type = '';
        }
        if (($menu_type == 'main' || $fm) && !$fm2) {
            $wml .= "<do name=\"b1\" type=\"options\" label=\"Пeрcoнaж\"><go href=\"$PHP_SELF?sid=$sid&cl=i&cj=1\"/></do>";
            if (!isset($loc_i[$loc][$login]["macro"]))
                $m = [];
            else
                $m = explode("/", $loc_i[$loc][$login]["macro"]);
            for ($i = 1; $i < 9; $i++)
                if ($m[$i - 1]) {
                    $mn = explode("|", $m[$i - 1]);
                    $wml .= "<do name=\"b$o\" type=\"options\" label=\"" . $mn[4] . "\"><go href=\"$PHP_SELF?sid=$sid&cm=$i\"/></do>";
                    $o++;
                }
        }
        if ($menu_type == 'inv' && $g_menu == 1)
            $wml .= "<do name=\"b1\" type=\"options\" label=\"В игру\">" .
                "<go href=\"$PHP_SELF?sid=$sid\"/></do>";
        if (
            ($menu_type == 'inv'  && $g_menu == 1) ||
            ($menu_type == 'main' && $g_menu == 0) ||
            $fm
        ) {
            $wml .= "<do name=\"b2\" type=\"options\" label=\"Cкaзaть\">" .
                "<go href=\"$PHP_SELF?sid=$sid&cs=1&cj=1\"/></do>" .
                "<do name=\"b3\" type=\"options\" label=\"Koнтaкты\">" .
                "<go href=\"$PHP_SELF?sid=$sid&msg=1&cj=1\"/></do>" .
                "<do name=\"b$o\" type=\"options\" label=\"мaкpocы\">" .
                "<go href=\"$PHP_SELF?sid=$sid&cm=new\"/></do>";
            if ($g_map) {
                $o++;
                $wml .= "<do name=\"b$o\" type=\"options\" label=\"Kapтa\">" .
                    "<go href=\"$PHP_SELF?sid=$sid&map=" . $g_map . "\"/></do>";
            }
            $o++;
            $wml .= "<do name=\"b$o\" type=\"options\" label=\"Coxpaнить\">" .
                "<go href=\"$PHP_SELF?sid=$sid&ce=1\"/></do>";
        }

        if (substr($message, 0, 2) != "<p")
            $message = "<p>" . $message;
        if (substr($message, strlen($message) - 4) != "</p>")
            $message .= "</p>";
        $wml .= "" . $message . "</card>";
    };
    if ($g_smf && strpos($wml, "<input") === false)
        $wml = preg_replace(
            ["'(<p [^>]*>)'",      "'<p>'",       "'</p>'"],
            [   "\\1<small>", "<p><small>", "</small></p>"],
            $wml
        );

    $wml .= "</wml>";
    $wml = str_replace("&amp;", "&", $wml);
    $wml = str_replace("&", "&amp;", $wml);

    echo $wml;
    exit;
}