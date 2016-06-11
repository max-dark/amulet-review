<?php
// карта, передается map=x123x456 и для картики img=1 bw=1|2|3, f=x123x456 место флага
$QUERY_STRING = (
array_key_exists('QUERY_STRING', $_SERVER) ?
    $_SERVER["QUERY_STRING"]
    :
    ''
);
$tmp = $QUERY_STRING;
$tmp = urldecode($tmp);
parse_str($tmp);

if ($info) msg("<p>Ваша позиция обозначена белым квадратиком с черной точкой в центре (или черным квадратиком с белой точкой в центре, в зависимости от фона).<br/>Флаг лидерства - для цветных желтым квадратиком с черной точкой, для ч/б - как ваша.<br/>Замки - красные квадратики с белой точкой в центре (только для цветной карты).<br/>В настройках можете поменять тип карты (формат PNG лучший, но не все телефоны поддерживают).<br/><a href=\"http://mag.su/game/f_faq.php?id=map\">Другие карты</a>");
if ($flag) msg("<p>" . implode("", file("../story/flag.htm")));
$loc = $l;

// >1650
function calctc($loc)
{
    if ($loc == "_begin") $loc = "x1158x523";
    if ($loc == "arena") $loc = "x1086x501";
    if (substr($loc, 0, 4) == "c.1.") $loc = "x1429x168";
    if (substr($loc, 0, 4) == "c.2.") $loc = "x781x429";
    if (substr($loc, 0, 4) == "c.3.") $loc = "x1129x369";
    if (substr($loc, 0, 4) == "c.4.") $loc = "x2320x348";

    $tc = explode("x", $loc);
    if ($tc[2] > 1101) {
        $tc[1] = round(($tc[1] - 20) / 6);
        $tc[2] = round(($tc[2] - 1101) / 6);
        $tc[3] = 2;
    } else
        if ($tc[1] > 1650) {
            $tc[1] = round(($tc[1] - 450 - 1200) / 15);
            $tc[2] = round($tc[2] / 15);
            $tc[3] = 1;
        } else {
            $tc[1] = round(($tc[1] - 450) / 12);
            $tc[2] = round($tc[2] / 12);
            $tc[3] = 0;
        }
    return $tc;
}

$tc = calctc($l);
$tcf = calctc($f);

if ($img) {                        // выведем картинку
    if ($bw == 1) $t = "wbmp"; else if ($bw == 2) $t = "jpg"; else $t = "png";
    if ($tc[3] == 2) $img = "map3." . $t; else if ($tc[3]) $img = "map2." . $t; else $img = "map1." . $t;
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
    if ($tcf[3] == $tc[3] && $f != $l) {    // флаг
        if ($col == 0 || $col == "16777180") {
            $cb = 1;
            $cc = imagecolorallocate($im, 255, 255, 255);
        } else {
            $cb = imagecolorallocate($im, 255, 255, 255);
            $cc = 1;
        }
        if ($bw != 1) {
            $cb = imagecolorallocate($im, 255, 255, 0);
            $cc = imagecolorallocate($im, 0, 0, 0);
        }
        imagefilledrectangle($im, $tcf[1], $tcf[2], $tcf[1] + 2, $tcf[2] + 2, $cb);
        imagesetpixel($im, $tcf[1] + 1, $tcf[2] + 1, $cc);
    }
    header("Content-type: {$size['mime']}");    //;charset=utf-8
    if ($bw == 1) imagewbmp($im); else if ($bw == 2) imagejpeg($im); else imagepng($im);
    imagedestroy($im);
    die("");
}//if $img

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
}