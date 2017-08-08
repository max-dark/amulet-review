<?php
/**
 * @copyright Copyright (C) 2016-2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */

namespace MaxDark\Amulet\OldCode;


class UTFTool
{
    /**
     * split utf-8 string to array of chars
     *
     * @param string $str
     *
     * @return string[]
     */
    public static function chars_of($str)
    {
        return \preg_split('/(?<!^)(?!$)/u', $str);
    }

    /**
     * utf-8 analog for strtr function
     *
     * @param string $str
     * @param string $from
     * @param string $to
     *
     * @return string
     */
    public static function strtr($str, $from, $to)
    {
        return \str_replace(
            self::chars_of($from),
            self::chars_of($to),
            $str
        );
    }
}