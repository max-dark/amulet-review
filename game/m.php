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
    // выведем картинку
    show_map($_GET['l'], $_GET['f'], intval($_GET['bw']));
}

/**
 * @param string $location
 * @return array
 */
function calctc($location)
{

    switch (substr($location, 0, 4)) {
        case "c.1.":
            $location = "x1429x168";
            break;
        case "c.2.":
            $location = "x781x429";
            break;
        case "c.3.":
            $location = "x1129x369";
            break;
        case "c.4.":
            $location = "x2320x348";
            break;
        default:
            if ($location == "_begin") $location = "x1158x523";
            if ($location == "arena")  $location = "x1086x501";
    }

    $tc = explode("x", $location);
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


/**
 * @param string $char локация персонажа
 * @param string $flag локация флага
 * @param int $image_type формат изображения
 */
function show_map($char, $flag, $image_type)
{
    $char_coord = calctc($char);
    $flag_coord = calctc($flag);
    switch ($image_type) {
        case 1:
            $file_type = "wbmp";
            break;
        case 2:
            $file_type = "jpg";
            break;
        default:
            $file_type = "png";
            break;
    }
    $img_file = "map" . ($char_coord[3] + 1) . ".$file_type";
    $size = getimagesize($img_file);
    switch ($image_type) {
        case 1:
            $image = imagecreatefromwbmp($img_file);
            break;
        case 2:
            $image = imagecreatefromjpeg($img_file);
            break;
        default:
            $image = imagecreatefrompng($img_file);
            break;
    }
    if (!$image) die("err");
    $col = imagecolorat($image, $char_coord[1] + 1, $char_coord[2] + 1);
    if ($col == 0 || $col == "16777180") {
        $cb = 1;
        $cc = imagecolorallocate($image, 255, 255, 255);
    } else {
        $cb = imagecolorallocate($image, 255, 255, 255);
        $cc = 1;
    }
    imagefilledrectangle($image, $char_coord[1], $char_coord[2], $char_coord[1] + 2, $char_coord[2] + 2, $cb);
    imagesetpixel($image, $char_coord[1] + 1, $char_coord[2] + 1, $cc);
    if ($flag_coord[3] == $char_coord[3] && $flag != $char) {    // флаг
        if ($col == 0 || $col == "16777180") {
            $cb = 1;
            $cc = imagecolorallocate($image, 255, 255, 255);
        } else {
            $cb = imagecolorallocate($image, 255, 255, 255);
            $cc = 1;
        }
        if ($image_type != 1) {
            $cb = imagecolorallocate($image, 255, 255, 0);
            $cc = imagecolorallocate($image, 0, 0, 0);
        }
        imagefilledrectangle($image, $flag_coord[1], $flag_coord[2], $flag_coord[1] + 2, $flag_coord[2] + 2, $cb);
        imagesetpixel($image, $flag_coord[1] + 1, $flag_coord[2] + 1, $cc);
    }
    header("Content-type: {$size['mime']}");    //;charset=utf-8
    switch ($image_type) {
        case 1:
            imagewbmp($image);
            break;
        case 2:
            imagejpeg($image);
            break;
        default:
            imagepng($image);
            break;
    }
    imagedestroy($image);
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
}