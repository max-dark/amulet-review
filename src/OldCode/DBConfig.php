<?php
/**
 * @copyright Copyright (C) 2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */


namespace MaxDark\Amulet\OldCode;


class DBConfig
{
    /**
     * @var string[]
     */
    private static $config;

    /**
     * @return string[]
     */
    public static function getConfig()
    {
        return self::$config;
    }

    /**
     * @param string[] $config
     */
    public static function setConfig($config)
    {
        self::$config = $config;
    }
}
