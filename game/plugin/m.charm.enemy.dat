<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 * @global use
 * @global char
 */

// привлечь на свою до 20 сек - 1 минута сторону любого (кроме гардов) атакующего, защищает, но не сопровождает
if (isset($loc_i[$loc][$to]) && substr($to, 0, 2) == 'n.' && ! substr($to, 0, 4) == 'n.g.') {
    if (isset($loc_i[$loc][$to]["owner"])) {
        msg("Этот персонаж принадлежит другому");
    }
    $tchar = explode("|", $loc_i[$loc][$to]["char"]);
    if ($tchar[7]) {
        $tchar[7]                  = "";
        $loc_i[$loc][$to]["char"]  = implode("|", $tchar);
        $loc_i[$loc][$to]["owner"] = $login . "|0|" . $login . "|" . (time() + rand(20, 1 * 60)) . "|0";
    } else {
        addjournal($loc, $login, "Заклинание действует только на дерущихся");
    }
}
addjournal($loc, $login, "Заклинание на него не подействует");