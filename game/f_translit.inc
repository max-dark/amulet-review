<?php

use MaxDark\Amulet\OldCode\Transliterate;

function trans($s)
{
    return Transliterate::convert($s);
}
