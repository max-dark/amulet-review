<?php

use MaxDark\Amulet\OldCode\PageType;

$stmp = "</p><p align='center'><a href='?site=news'>Новости";
$t    = filemtime('../news/news.htm');
if ($t) {
    $stmp .= date("d/m", $t);
}
$stmp .= "</a>
<br/>Ваше имя:
<br/><input name='nn' emptyok='true' type='text' value='' />
<br/>Пароль:
<br/><input name='pass' emptyok='true' type='password' value='' />
<br/><a href='?site=connect&amp;login=$(nn:escape)&amp;p=$(pass:escape)&amp;rnd=" . rand(1, 99) . "'>Войти</a>
<br/><a href='?site=gamereg&amp;log=new&amp;nn=$(nn:escape)'>Регистрация</a>
<br/><a href='?site=stat&r=" . rand(1, 99) . "'>Статистика</a>
<br/><a href='?site=faq'>Помощь</a>";
msg($stmp, $GAME_NAME, 0, PageType::GUEST);
