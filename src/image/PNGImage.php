<?php

/* 
 * @license MIT, see license.txt
 * @copyright 2017 Max Dark <maxim.dark@gmail.com>.
 */

namespace MaxDark\Amulet\image;

use MaxDark\Amulet\image\AbstractImage;

/**
 * Class PNGImage
 */
class PNGImage extends AbstractImage
{
    /**
     * загрузка картинки
     *
     * @param $file_name
     *
     * @return resource
     */
    public function loadFile($file_name)
    {
        return imagecreatefrompng($file_name);
    }

    /** запись картинки в $file_name или STDOUT
     *
     * @param string|null $file_name
     *
     * @return bool
     */
    public function writeFile($file_name = null)
    {
        return imagepng($this->getImage(), $file_name);
    }
}
