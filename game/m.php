<?php
/**
 * карта, передается map=x123x456 и для картики img=1 bw=1|2|3, f=x123x456 место флага
 */


if (array_key_exists('info', $_GET)) {
    msg_m("Ваша позиция обозначена белым квадратиком с черной точкой в центре (или черным квадратиком с белой точкой в центре, в зависимости от фона).<br/>Флаг лидерства - для цветных желтым квадратиком с черной точкой, для ч/б - как ваша.<br/>Замки - красные квадратики с белой точкой в центре (только для цветной карты).<br/>В настройках можете поменять тип карты (формат PNG лучший, но не все телефоны поддерживают).<br/><a href=\"http://mag.su/game/f_faq.php?id=map\">Другие карты</a>");
}
if (array_key_exists('flag', $_GET)) {
    msg_m(implode("", file("../story/flag.htm")));
}
if (array_key_exists('img', $_GET)) {
    require_once 'modules/map.php';
    // выведем картинку
    show_map($_GET['l'], $_GET['f'], intval($_GET['bw']));
    exit(0);
}

/**
 * @param string $page
 */
function msg_m($page)
{
    header("Expires: Thu, 01 Jan 1970 00:00:01 GMT");
    header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
    header("Cache-Control: no-cache, no-store, must-revalidate, max-age=0");
    header("Pragma: no-cache");
    header("Content-type: text/vnd.wap.wml;charset=utf-8");

    echo <<<WML
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML 1.1//EN" "https://www.wapforum.org/DTD/wml_1.1.xml">
<wml>
<card title="Карта">
<p>$page</p>
</card>
</wml>
WML;
    exit(0);
}