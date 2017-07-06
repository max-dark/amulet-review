<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global id
 * @global locai
 * @global tchar
 * @global g_list
 * @global trader
 * @global up
 * @global down
 * @global to
 */

// список
if ($trader[1]) {
    $filter = explode(":", $trader[1]);
} else {
    $filter = "";
}
$items = $loc_i[$loc][$login]["items"];
// удалим что не в фильтре
if ($filter) {
    $items = explode("|", $items);
    foreach (array_keys($items) as $i) {
        $b = 0;
        foreach ($filter as $j) {
            if (strpos($items[$i], $j) !== false) {
                $b = 1;
                break;
            }
        }
        if ( ! $b) {
            unset($items[$i]);
        }
    }
    $items = implode("|", $items);
}
$items = preg_replace('/\|?i\.money:(\d+)/', "", $items);
$stmp  = "";
$xF    = preg_match_all("'([^|]+):(\d+)'", $items, $regF);
if ( ! $start) {
    $start = 0;
}
for ($i = $start; $i < $start + $g_list && $i < $xF; $i++) {
    if ($regF[1][$i]) {
        if (strpos($regF[1][$i], "_") !== false) {
            $ti = substr($regF[1][$i], 0, strpos($regF[1][$i], "_"));
        } else {
            $ti = $regF[1][$i];
        }
        if (substr($regF[1][$i], 0, 5) == "i.rr.") {
            $item = explode("|", "руна|50");
        } else {
            if (file_exists("items/" . $ti)) {
                $item = explode("|", (file_get_contents("items/" . $ti)));
            } else {
                die("err: no items/" . $regF[1][$i]);
            }
        }
        $tmp = $item[0];
        if (strpos($regF[1][$i], "..") !== false) {
            $tmp .= " *";
        }
        if ($regF[2][$i] > 1) {
            $tmp .= " (" . $regF[2][$i] . ")";
        }
        $tmp .= " за " . round($item[1] * $trader[0]) . " монет";
        $stmp .= "\n<br/><anchor>" . $tmp . "<go href=\"#menu\"><setvar name=\"to\" value=\"" . $regF[1][$i] .
                 "\"/></go></anchor>";
    }
}

if ($stmp) {
    $stmp = "<p>Я могу купить:" . $stmp;
} else {
    msg("Сожалею, но у вас нет интересующих меня товаров.", $tchar[0]);
}
if ($start) {
    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id\">^ </a>";
}
if ($start + $g_list < $xF - 1) {
    if ( ! $start) {
        $stmp .= "\n<br/>";
    }
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=" . ($start + $g_list) . "\">+ (" .
             ($xF - $start - $g_list) . ")</a>";
}
$stmp .= "\n</p>\n</card>\n<card id=\"menu\" title=\"Меню\">\n<p>\n<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start&to=$(to)\">Продать</a>\n<br/><a href=\"$PHP_SELF?sid=$sid&look=$(to)\">Инфо</a>";
msg($stmp, $tchar[0]);