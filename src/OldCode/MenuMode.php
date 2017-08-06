<?php
/**
 * @copyright Copyright (C) 2016-2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */


namespace MaxDark\Amulet\OldCode;

interface MenuMode
{
    const FULL = 0; // полное
    const SHORT = 1; // сокращенное
    const PAGE = 2; // на отдельной странице
    const LINKS = 3; // в виде ссылок (если не видно обычное меню)
}