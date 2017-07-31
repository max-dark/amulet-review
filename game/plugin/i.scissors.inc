<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_tt
 * @global to
 * @global char
 */
// разрезает первое в инвентори платье на 3 бинта, брюки, рубашку и юбку на 2. навык не нужен

if ($to) {
    $item = findItemByBaseId($to);
    if (intval(preg_replace('/.*' . $to . ':(\d+).*/', "\\1", $loc_i[$loc][$login]["items"])) < 1) {
        msg("У вас нет " . $item[0] . ", чтобы разрезать его ножницами");
    }
    $c = 0;
    if ($to == 'i.polotno') {
        $c = 6;
    } else {
        if (substr($to, 0, 6) == 'i.a.w.') {
            $c = 2;
        } else {
            if (substr($to, 0, 6) == 'i.a.r.') {
                $c = 3;
            } else {
                if (substr($to, 0, 6) == 'i.a.e.') {
                    $c = 3;
                } else {
                    if (substr($to, 0, 6) == 'i.a.b.') {
                        $c = 3;
                    }
                }
            }
        }
    }

    if ($c > 0) {
        addjournal($loc, $login, "Вы разрезали " . $item[0] . " на $c бинта");
        addjournal($loc, "all", $char[0] . " разрезал " . $item[0] . " на $c бинта", $login);
        additem($loc, $login, "", $to, 1);
        additem($loc, "", $login, "i.bint", $c);
    } else {
        addjournal($loc, $login, "Ножницами резать можно только ткань или одежду");
    }

} else {
    $list = 'all';
}    // выведем ниже список целей

