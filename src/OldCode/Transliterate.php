<?php
/**
 * @copyright Copyright (C) 2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */


namespace MaxDark\Amulet\OldCode;


class Transliterate
{
    /** @var string[][] */
    private static $letters = [];

    /** @var bool */
    private static $flag = true;

    /**
     * @param string $s
     * @return string
     */
    public static function convert($s)
    {
        self::$flag = true;
        return \preg_replace_callback('/(.*)##??/Uu', function (array $m) {
            return self::alt($m[1]);
        }, "$s#");
    }

    private static function initLetters()
    {
        if (empty(self::$letters)) {
            $letters[0] = [
                "jo" => "ё",
                "je" => "э",
                "ju" => "ю",
                "ja" => "я",
                "zh" => "ж",
                "ch" => "ч",
                "sh" => "ш",
                "JO" => "Ё",
                "JE" => "Э",
                "JU" => "Ю",
                "JA" => "Я",
                "ZH" => "Ж",
                "CH" => "Ч",
                "SH" => "Ш"
            ];
            $letters[1] = array_combine(
                UTFTool::chars_of("wxq'ertyuiopasdfghjklzcvbnmWQXERTYUIOPASDFGHJKLZCVBNM"),
                UTFTool::chars_of('щъььертыуиопасдфгхйклзцвбнмЩЬЪЕРТЫУИОПАСДФГХЙКЛЗЦВБНМ')
            );
        }
    }

    private static function alt($s)
    {
        if (self::$flag) {
            self::initLetters();
            $s = \strtr($s, self::$letters[0]);
            $s = \preg_replace('/(j|J)_(a|A|u|U|o|O|e|E)/u', "\\1\\2", $s);
            $s = \preg_replace('/(z|Z|c|C|s|S)_(h|H)/u', "\\1\\2", $s);
            $s = \strtr($s, self::$letters[1]);
        }
        self::$flag = ! self::$flag;
        return $s;
    }
}