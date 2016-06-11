<?php

/** Выводит страницу пользователю
 * Требует писец как много глобальных значений
 * Вызывает savegame
 * Завершает выполнение скрипта(exit в конце)
 * @param string $msg Сообщение
 * @param string $title название игры
 * @param int $journal флаг, что нужно выводить журнал
 * @param string $menu тип меню. возможные значения (""|none|main|to|inv)
 * @param string $vname имя переменной для вставки в страницу
 * @param string $vval значение переменной vname
 */
function msg(
    $msg, $title = 'Амулет Дракона',
    $journal = 1, $menu = '',
    $vname = '', $vval = ''
)
{
    global $game, $g_admin, $gm, $login, $loc, $loc_i, $page_d, $PHP_SELF, $sid,
           $gm_id, $g_size, $g_list, $fskipj, $cj, $t_g, $g_menu, $fm, $fm2, $g_j2loc, $g_joff, $g_smf,
           $g_map, $g_tmp;

    $wml = "<wml>";
    if (!$login) {
        $journal = 0;
        $menu = "none";
    }
    if ($fm) {
        $journal = 0;
        $page_d = 0;
        $msg = "Нажмите кнопку Меню";
    }
    if ($fm2) {
        $journal = 0;
        $page_d = 0;
        require("f_menu.dat");
    }
    if ($cj)
        $fskipj = 1;
    if ($fskipj)
        $journal = 0;
    else
        if ($g_joff)
            $loc_i[$loc][$login]["journal"] = preg_replace('/(\||^)[^:!]*(\||$)/', "|", $loc_i[$loc][$login]["journal"]);
    $page_j = "";
    if (get($game, "journal") && $login != $g_admin && $gm != $gm_id)
        $page_j = $game["journal"];
    if ($journal == 1 && $loc_i[$loc][$login]["journal"]) {
        $page_j = str_replace("|", "<br/>", $loc_i[$loc][$login]["journal"]);
        $loc_i[$loc][$login]["journal"] = "";
        if (!$g_j2loc)
            $page_j = preg_replace('/<br\/>(Пришел|Пришла) [^<]+/', "", $page_j);
    }

    $t_g1 = sscanf(microtime(), "%s %s");
    $t_g1 = $t_g1[1] + $t_g1[0];
    $game["tmid"] = ($game["tmid"] + $t_g1 - $t_g) / 2;
    savegame();
    if ($page_j && $journal) {
        $page_j = explode("<br/>", $page_j);
        if (count($page_j) > $g_list)
            array_splice($page_j, 0, count($page_j) - $g_list);
        $page_j = implode("<br/>", $page_j);
    }
    if ($page_d && file_exists("loc_f/" . $loc))
        $page_j .= "<br/>" . implode("", file("loc_f/" . $loc));
    strlen($wml . $msg . $page_j) < $g_size ? $bsize = 1 : $bsize = 0;
    if ($page_j && substr($page_j, 0, 5) == "<br/>")
        $page_j = substr($page_j, 5);
    if ($page_j && $journal) {
        if ($bsize)
            $tu = "#g";
        else {
            $tu = "$PHP_SELF?" . preg_replace("/(ci|use|say|ca|drop|take|to|adm|cm|go)=/", "c1=", $g_tmp);
        }
        $wml .= "<card title=\"Журнал\"><do type=\"accept\" label=\"Дальше\"><go href=\"" . $tu .
            "\"/></do><p>" . $page_j . "<br/><a href=\"" . $tu . "\">Дальше</a>";
        if ($tu != "#g")
            $wml .= "/<a href=\"" . $tu . ($fskipj ? "" : "&cj=1") . "\">к меню</a>/";
        $wml .= "</p></card>";
    }

    if ($bsize || !$journal || !$page_j) {
        $wml .= "<card id=\"g\" title=\"" . $title . "\"";
        if ($menu == 'main')
            $wml .= " ontimer=\"$PHP_SELF?sid=$sid\"><timer value=\"600\"/";
        $wml .= ">";
        if ($vname)
            $wml .= "<onevent type=\"onenterforward\"><refresh><setvar name=\"$vname\" value=\"$vval\"/></refresh></onevent>";

        if ($menu == '' || $menu == 'inv' && $g_menu != 1) {
            $wml .= "<do name=\"b1\" type=\"options\" label=\"В игру\"><go href=\"$PHP_SELF?sid=$sid\"/></do>";
            $wml .= "<do name=\"b2\" type=\"accept\" label=\"Назад\"><prev/></do>";
        }
        $o = 4;
        if ($menu == 'main' && $g_menu == 2 && !$fm) {
            $wml .= "<do name=\"o2\" type=\"options\" label=\"Меню\"><go href=\"$PHP_SELF?sid=$sid&fm=1&cj=1\"/></do>";
            $menu = '';
        }
        if ($menu == 'main' && $g_menu == 3 && !$fm2) {
            $msg = str_replace("</p></card><card id=\"m\"", "<br/><a href=\"$PHP_SELF?sid=$sid&fm2=1&cj=1\">Меню</a></p></card><card id=\"m\"",
                $msg);
            $menu = '';
        }
        if (($menu == 'main' || $fm) && !$fm2) {
            $wml .= "<do name=\"b1\" type=\"options\" label=\"Пeрcoнaж\"><go href=\"$PHP_SELF?sid=$sid&cl=i&cj=1\"/></do>";
            if (!isset($loc_i[$loc][$login]["macro"]))
                $m = array();
            else
                $m = explode("/", $loc_i[$loc][$login]["macro"]);
            for ($i = 1; $i < 9; $i++)
                if ($m[$i - 1]) {
                    $mn = explode("|", $m[$i - 1]);
                    $wml .= "<do name=\"b$o\" type=\"options\" label=\"" . $mn[4] . "\"><go href=\"$PHP_SELF?sid=$sid&cm=$i\"/></do>";
                    $o++;
                }
        }
        if ($menu == 'inv' && $g_menu == 1)
            $wml .= "<do name=\"b1\" type=\"options\" label=\"В игру\"><go href=\"$PHP_SELF?sid=$sid\"/></do>";
        if ($menu == 'inv' && $g_menu == 1 || $g_menu == 0 && $menu == 'main' || $fm) {
            $wml .= "<do name=\"b2\" type=\"options\" label=\"Cкaзaть\"><go href=\"$PHP_SELF?sid=$sid&cs=1&cj=1\"/></do>";
            $wml .= "<do name=\"b3\" type=\"options\" label=\"Koнтaкты\"><go href=\"$PHP_SELF?sid=$sid&msg=1&cj=1\"/></do>";
            $wml .= "<do name=\"b$o\" type=\"options\" label=\"мaкpocы\"><go href=\"$PHP_SELF?sid=$sid&cm=new\"/></do>";
            if ($g_map) {
                $o++;
                $wml .= "<do name=\"b$o\" type=\"options\" label=\"Kapтa\"><go href=\"$PHP_SELF?sid=$sid&map=" .
                    $g_map . "\"/></do>";
            }
            $o++;
            $wml .= "<do name=\"b$o\" type=\"options\" label=\"Coxpaнить\"><go href=\"$PHP_SELF?sid=$sid&ce=1\"/></do>";
        }

        if (substr($msg, 0, 2) != "<p")
            $msg = "<p>" . $msg;
        if (substr($msg, strlen($msg) - 4) != "</p>")
            $msg .= "</p>";
        $wml .= "" . $msg . "</card>";
    };
    if ($g_smf && strpos($wml, "<input") === false)
        $wml = preg_replace(array("'(<p [^>]*>)'", "'<p>'", "'</p>'"), array("\\1<small>",
            "<p><small>", "</small></p>"), $wml);

    $wml .= "</wml>";
    $wml = str_replace("&amp;", "&", $wml);
    $wml = str_replace("&", "&amp;", $wml);
    //$wml = strtr( $wml, "КЕНХВАРОСМТехарос", "KEHXBAPOCMTexapoc" );

    echo $wml;
    exit;
}
