<?php

use MaxDark\Amulet\OldCode\PageType;

/**
 * @return string
 */
function get_stat_page()
{
    $rnd = rand(1, 99);

    return <<<PAGE
<p align="center">
<a href="?site=online&r=$rnd">Кто онлайн?</a><br/>
<a href="?site=flag&r=$rnd">Флаг лидерства</a><br/>
<a href="?site=clans&r=$rnd">Список кланов</a><br/>
<a href="?site=castle&r=$rnd">Захват замков</a>
</p><p><button class="btn btn-outline-secondary m-1" name="b2" type="accept" label="Назад" onclick="history.back()"><prev>Назад</prev></button>
PAGE;
}

msg(get_stat_page(), "Статистика", 0, PageType::GUEST);
