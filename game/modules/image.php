<?php

/**
 * Class Image
 * Обертка для GD
 */
abstract class Image {
    /** @var resource $image */
    private $image;
    /** @var string $file_name */
    private $file_name;
    /** @var array $info */
    private $info;
    /** @var array $ext_info */
    private $ext_info;
    /**
     * Image constructor.
     * @param string $file_name
     */
    public function __construct($file_name) {
        $this->file_name = $file_name;
        $this->info = getimagesize($file_name, $this->ext_info);
        $this->image = $this->loadFile($file_name);
    }

    /**
     * @param $file_name
     * @return resource
     */
    public abstract function loadFile($file_name);

    /** запись картинки в $file_name или STDOUT
     * @param string|null $file_name
     * @return bool
     */
    public abstract function writeFile($file_name = null);

    /**
     *  подчистить ресурсы
     */
    public function __destruct()
    {
        imagedestroy($this->image);
    }

    /**
     * @param int $red
     * @param int $green
     * @param int $blue
     * @return int
     */
    public function allocateColor($red, $green, $blue) {
        return imagecolorallocate($this->image, $red, $green, $blue);
    }

    /**
     * @param int $x
     * @param int $y
     * @param int $color
     * @return bool
     */
    public function setPixel($x, $y, $color) {
        return imagesetpixel($this->image, $x, $y, $color);
    }

    public function filledRectangle($x1, $y1, $x2, $y2, $color) {
        return imagefilledrectangle($this->image, $x1, $y1, $x2, $y2, $color);
    }

    /**
     * @param int $x
     * @param int $y
     * @return int
     */
    public function colorAt($x, $y) {
        return imagecolorat($this->image, $x, $y);
    }

    /**
     * @return mixed
     */
    public function getInfo($that) {
        return $this->info[$that];
    }

    /**
     * @return mixed
     */
    public function getExtInfo($that) {
        return $this->ext_info[$that];
    }
}