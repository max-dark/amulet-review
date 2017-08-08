<?php
/**
 * @copyright Copyright (C) 2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */

namespace MaxDark\Amulet\OldCode;


class Params
{
    const NOT_SET = 'NOT_SET';
    private static $Names;
    private static $Values;

    public static function init($Names, $Values)
    {
        self::$Names = $Names;
        self::$Values = $Values;
    }

    public static function getParam($Name)
    {
        $Name  = strtolower($Name);
        $Nlist = explode(":", self::getNames());
        for ($i = 0; $i < count($Nlist); $i++) {
            if ($Nlist[$i] == $Name) {
                break;
            }
        }
        if ($i == count($Nlist)) {
            return self::NOT_SET;
        }
        $Vlist = explode(":", self::getValues());

        return stripslashes(str_replace("!~!", ":", $Vlist[$i]));
    }

    public static function setParam($Name, $Value)
    {
        $Nlist = explode(":", self::$Names);
        $Name  = strtolower($Name);
        $Value = addslashes(str_replace(":", "!~!", $Value));
        for ($i = 0; $i < count($Nlist); $i++) {
            if ($Nlist[$i] == $Name) {
                break;
            }
        }

        if ($i == count($Nlist) and ($Value != self::NOT_SET)) { // Добавляем имя и значение
            self::$Names .= ":$Name";
            self::$Values .= ":$Value";
        } else {
            $Vlist     = explode(":", self::$Values);
            $Vlist[$i] = $Value;
            self::$Values    = implode(":", $Vlist);
            if ($Value == self::NOT_SET) { // Удаление имени и значения
                $Nlist[$i] = self::NOT_SET;
                self::$Names     = implode(":", $Nlist);
                self::$Names     = str_replace(":" . self::NOT_SET, "", self::$Names);
                self:: $Values    = str_replace(":" . self::NOT_SET, "", self::$Values);
            }
        }
    }
    /**
     * @return mixed
     */
    public static function getNames()
    {
        return self::$Names;
    }

    /**
     * @return mixed
     */
    public static function getValues()
    {
        return self::$Values;
    }
}