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
 * @global trader
 * @global up
 * @global down
 * @global to
 */

// список
use MaxDark\Amulet\OldCode\ViewOptions;

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

$listEnd = $start + ViewOptions::getInstance()->getListsSize();
for ($i = $start; $i < $listEnd && $i < $xF; $i++) {
    $itemId = $regF[1][$i];
    if ($itemId) {
        $itemCount = $regF[2][$i];
        $item = findItemByFullId($itemId);
        $tmp = $item[0];
        if (strpos($itemId, "..") !== false) {
            $tmp .= " *";
        }
        if ($itemCount > 1) {
            $tmp .= " (" . $itemCount . ")";
        }
        $tmp .= " за " . round($item[1] * $trader[0]) . " монет";
        $stmp .= "\n<br/><anchor>$tmp<go href=\"#menu\"><setvar name=\"to\" value=\"{$itemId}\"/></go></anchor>";
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
if ($listEnd < $xF - 1) {
    if ( ! $start) {
        $stmp .= "\n<br/>";
    }
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=" . ($listEnd) . "\">+ (" .
             ($xF - $listEnd) . ")</a>";
}
$stmp .= "\n</p>\n</card>\n<card id=\"menu\" title=\"Меню\">\n";
$stmp .= "<p>\n<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start&to=$(to)\">Продать</a>\n";
$stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=$(to)\">Инфо</a>";
msg($stmp, $tchar[0]);
