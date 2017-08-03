<?php

use MaxDark\Amulet\OldCode\WordFilter;

function GetBadWord($s) // Возвращает первое плохое слово или пустую строку

{
    return WordFilter::getAbuse($s);
}