<?php
// карта, передается map=x123x456 и для картики img=1 bw=1|2|3
$tmp = $QUERY_STRING;
if ($tmp == '') $tmp = $_SERVER["QUERY_STRING"];
$tmp = urldecode($tmp);
parse_str($tmp);

if ($info) msg("<p>Ваша позиция обозначена белым квадратиком с черной точкой в центре (или черным квадратиком с белой точкой в центре, в зависимости от фона).<br/>Замки - красные квадратики с белой точкой в центре (только для цветной карты).<br/>В настройках можете поменять тип карты на черно-белую (самое плохое качество), цветную JPEG (среднее качество) или PNG (самое лучшее качество, но не все телефоны поддерживают).");

// >1650
if ($loc == "_begin") $loc = "x1158x523";
if ($loc == "arena") $loc = "x1086x501";
if (substr($loc, 0, 4) == "c.1.") $loc = "x1429x168";
if (substr($loc, 0, 4) == "c.2.") $loc = "x781x429";
if (substr($loc, 0, 4) == "c.3.") $loc = "x1129x369";
if (substr($loc, 0, 4) == "c.4.") $loc = "x2320x348";

$tc = explode("x", $loc);
if ($tc[1] > 1650) {
    $b = 1;
    $tc[1] = round(($tc[1] - 450 - 1200) / 15);
    $tc[2] = round($tc[2] / 15);
} else {
    $b = 0;
    $tc[1] = round(($tc[1] - 450) / 12);
    $tc[2] = round($tc[2] / 12);
}

if ($img) {                        // выведем картинку
    if ($bw == 1) $t = "wbmp"; else if ($bw == 2) $t = "jpg"; else $t = "png";
    if ($b) $img = "map2." . $t; else $img = "map1." . $t;
    $size = getimagesize($img);
    if ($bw == 1) $im = imagecreatefromwbmp($img); else if ($bw == 2) $im = imagecreatefromjpeg($img); else $im = imagecreatefrompng($img);
    if (!$im) die("err");
    $col = imagecolorat($im, $tc[1] + 1, $tc[2] + 1);
    if ($col == 0 || $col == "16777180") {
        $cb = 1;
        $cc = imagecolorallocate($im, 255, 255, 255);
    } else {
        $cb = imagecolorallocate($im, 255, 255, 255);
        $cc = 1;
    }
    imagefilledrectangle($im, $tc[1], $tc[2], $tc[1] + 2, $tc[2] + 2, $cb);
    imagesetpixel($im, $tc[1] + 1, $tc[2] + 1, $cc);
    header("Content-type: {$size['mime']}");    //;charset=utf-8
    if ($bw == 1) imagewbmp($im); else if ($bw == 2) imagejpeg($im); else imagepng($im);
    imagedestroy($im);
    die("");
}//if $img

srand((float)microtime() * 10000000);
$stmp = "<p align=\"center\"><img alt=\"map\" src=\"f_map.php?loc=$loc&amp;img=1&amp;r=" . rand(99, 999) . "&amp;bw=$bw\"/><br/><anchor>[назад]<prev/></anchor></p><p>Вы на ";
if ($b) $stmp .= " территории Ансалона."; else $stmp .= "основной территории.";
$stmp .= "<br/><a href=\"f_map.php?info=1\">Помощь</a>";

msg($stmp);

function msg($s)
{
    header("Expires: Thu, 01 Jan 1970 00:00:01 GMT");
    header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
    header("Cache-Control: no-cache, no-store, must-revalidate, max-age=0");
    header("Pragma: no-cache");
    header("Content-type:text/vnd.wap.wml;charset=utf-8");
    echo "<?xml version=\"1.0\"?>\n<!DOCTYPE wml PUBLIC \"-//WAPFORUM//DTD WML 1.1//EN\" \"http://www.wapforum.org/DTD/wml_1.1.xml\">";
    echo "
<wml>
<card title=\"Карта\">";
    echo "
$s
</p>
</card>
</wml>";
    die("");
}