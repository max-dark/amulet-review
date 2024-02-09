<?php

use MaxDark\Amulet\OldCode\PageType;

$stmp = "</p><p align='center'><a href='?site=news'>Новости";
$t = filemtime('../news/news.htm');
if ($t) {
    date_default_timezone_set('Europe/Warsaw');
    $stmp .= date("d/m", $t);
}
$stmp .= "</a>
<br/>Ваше имя:
<br/><input name='nn' emptyok='true' type='text' value='' />
<br/>Пароль:
<br/><input name='pass' emptyok='true' type='password' value='' />
<br/><btn id=\"login\" class=\"btn btn-primary m-1\" href='?site=connect&amp;login=$(nn)&amp;p=$(pass)&amp;rnd=" . rand(1, 99) . "'>Войти</btn>
<br/><btn id=\"reg\" class=\"btn btn-outline-primary m-1\" href='?site=gamereg&amp;log=new&amp;nn=$(nn)'>Регистрация</btn>
<br/><a class=\"btn btn-outline-info m-1\" href='?site=stat&r=" . rand(1, 99) . "'>Статистика</a>
<br/><a class=\"btn btn-outline-info m-1\" href='?site=faq'>Помощь</a>";
msg($stmp, $GAME_NAME, 0, PageType::GUEST);
