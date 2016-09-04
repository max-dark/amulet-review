<?php

require_once __DIR__ . DIRECTORY_SEPARATOR . 'image.php';

/**
 * @param string $location
 *
 * @return array
 */
function calculateCoordinates($location)
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
            if ($location == "_begin") {
                $location = "x1158x523";
            }
            if ($location == "arena") {
                $location = "x1086x501";
            }
    }

    /** @noinspection PhpUnusedLocalVariableInspection */
    list($type, $x, $y) = explode("x", $location);
    if ($y > 1101) {
        // Волчий остров
        $x    = round(($x - 20) / 6);
        $y    = round(($y - 1101) / 6);
        $type = 2;
    } else {
        if ($x > 1650) {
            // территория Ансалона
            $x    = round(($x - 450 - 1200) / 15);
            $y    = round($y / 15);
            $type = 1;
        } else {
            // основная территория
            $x    = round(($x - 450) / 12);
            $y    = round($y / 12);
            $type = 0;
        }
    }

    return [$type, $x, $y];
}

/**
 * @param string $char       локация персонажа
 * @param string $flag       локация флага
 * @param int    $image_type формат изображения
 */
function show_map($char, $flag, $image_type)
{
    list($char_map, $char_x, $char_y) = calculateCoordinates($char);
    list($flag_map, $flag_x, $flag_y) = calculateCoordinates($flag);

    $image = create_image($image_type, $char_map + 1);

    $white_color = $image->allocateColor(0xff, 0xff, 0xff);
    $black_color = $image->allocateColor(0, 0, 0);
    $color       = $image->colorAt($char_x + 1, $char_y + 1);
    if ($color == 0 || $color == 0xFFFFDC) {
        $bg_color = 1;
        $fg_color = $white_color;
    } else {
        $bg_color = $white_color;
        $fg_color = 1;
    }
    set_mark($image, $char_x, $char_y, $fg_color, $bg_color);
    if ($flag_map == $char_map && $flag != $char) {    // флаг
        if ($color == 0 || $color == 0xFFFFDC) {
            $bg_color = 1;
            $fg_color = $white_color;
        } else {
            $bg_color = $white_color;
            $fg_color = 1;
        }
        if ($image_type != 1) {
            $bg_color = $white_color;
            $fg_color = $black_color;
        }
        set_mark($image, $flag_x, $flag_y, $fg_color, $bg_color);
    }
    header("Content-type: {$image->getInfo('mime')}");
    $image->writeFile();
    unset($image);
}

/**
 * @param Image $image
 * @param int   $x
 * @param int   $y
 * @param int   $fg_color
 * @param int   $bg_color
 */
function set_mark(&$image, $x, $y, $fg_color, $bg_color)
{
    $image->filledRectangle($x, $y, $x + 2, $y + 2, $bg_color);
    $image->setPixel($x + 1, $y + 1, $fg_color);
}

/**
 * @param int $image_type
 * @param int $id
 *
 * @return Image
 */
function create_image($image_type, $id)
{
    switch ($image_type) {
        case 1:
            $image = new WBMPImage(build_name($id, "wbmp"));
        break;
        case 2:
            $image = new JpegImage(build_name($id, "jpg"));
        break;
        default:
            $image = new PNGImage(build_name($id, "png"));
        break;
    }

    return $image;
}

/**
 * @param int    $id
 * @param string $file_ext
 *
 * @return string
 */
function build_name($id, $file_ext)
{
    return "data/map{$id}.{$file_ext}";
}
