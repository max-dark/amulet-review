<?php

use MaxDark\Amulet\OldCode\MapTool;

require_once '../vendor/autoload.php';
require_once 'config.php';

/**
 * карта, передается map=x123x456 и для картики img=1 bw=1|2|3, f=x123x456 место флага
 */

if (array_key_exists('info', $_GET)) {
    msg_m("Ваша позиция обозначена белым квадратиком с черной точкой в центре (или черным квадратиком с белой точкой в центре, в зависимости от фона).<br/>Флаг лидерства - для цветных желтым квадратиком с черной точкой, для ч/б - как ваша.<br/>Замки - красные квадратики с белой точкой в центре (только для цветной карты).<br/>В настройках можете поменять тип карты (формат PNG лучший, но не все телефоны поддерживают).<br/><a href=\"?site=faq&id=map\">Другие карты</a>");
}
if (array_key_exists('flag', $_GET)) {
    msg_m(file_get_contents("../story/flag.htm"));
}
if (array_key_exists('img', $_GET)) {
    // выведем картинку
    MapTool::showMapImage($_GET['l'], $_GET['f'], intval($_GET['bw']));
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
    header("Content-type: text/html;charset=utf-8");

    echo <<<XML
<?xml version="1.0"?>
<!DOCTYPE html>
<head>
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js" integrity="sha384-C6RzsynM9kWDrMNeT87bh95OGNyZPhcTNXj1NW7RuBCsyN/o0jlpcV8Qyq46cDfL" crossorigin="anonymous"></script>
</head>
<html>
<div title="Карта">
<p>$page</p>
</div>
</html>
XML;
    exit(0);
}
