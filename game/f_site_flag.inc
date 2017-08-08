<?php

use MaxDark\Amulet\OldCode\PageType;

function get_flag_page()
{
    global $game;
    $id = Request('id');

    if ($id == "info") {
        $page = file_get_contents("../story/flag.htm");
    } else {
        if (have_key($game, "fid")) {
            $page = 'Флаг лидерства принадлежит:<br/>' . $game['fchar'];
        } else {
            $page = 'Сейчас никто не владеет флагом лидерства';
        }
        $page .= <<<'PAGE'
<br/>
<br/>Точную позицию флага и имя его текущего обладателя вы всегда можете посмотреть на игровой карте
(в основном меню пункт Карта).<br/>
<a href="?site=flag&id=info">Что такое флаг лидерства?</a>

PAGE;
    }

    return $page;
}

msg(get_flag_page(), "Амулет дракона", 0, PageType::GUEST);