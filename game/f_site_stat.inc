<?php

$rnd = rand(1, 99);
$stmp = <<<TAG
<p align="center">
<a href="$PHP_SELF?site=online&r=$rnd">Кто онлайн?</a>
<br/><a href="$PHP_SELF?site=flag&r=$rnd">Флаг лидерства</a>
<br/><a href="$PHP_SELF?site=clans&r=$rnd">Список кланов</a>
<br/><a href="$PHP_SELF?site=castle&=$rnd">Захват замков</a>
</p><p><anchor>Назад<prev/></anchor>
TAG;
msg($stmp, "Статистика", 0, 'none');
