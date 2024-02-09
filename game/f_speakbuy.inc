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
 */

// время обновления товаров
use MaxDark\Amulet\OldCode\ViewOptions;

if (time() > $tchar[11]) {
    // формат записи шансналичия:минКоличество:максКоличество=идПредмета:текущееКоличество
    $xF = preg_match_all("'((\d+):(\d+):(\d+)=([^:]+):(\d+))'", $loc_i[$loc][$speak]["bank"], $regF);
    for ($i = 0; $i < $xF; $i++) {
        if (rand(0, 100) > $regF[2][$i]) {
            $tmp = 0;
        } else {
            $tmp = rand($regF[3][$i], $regF[4][$i]);
        }
        $loc_i[$loc][$speak]["bank"] = str_replace(
            $regF[1][$i],
            $regF[2][$i] . ":" . $regF[3][$i] . ":" . $regF[4][$i] . "=" . $regF[5][$i] . ":" . $tmp,
            $loc_i[$loc][$speak]["bank"]
        );
    }
    $tchar[11] = time() + round($trader[1] * (rand(70, 130) / 100));
    $loc_i[$loc][$speak]["char"] = implode("|", $tchar);
}

$stmp = "";
$xF = preg_match_all("'=([^:]+):([^0]\d*)'", $loc_i[$loc][$speak]["bank"], $regF);

if (!$start) {
    $start = 0;
}
$listEnd = $start + ViewOptions::getInstance()->getMaxListSize();
for ($i = $start; $i < $listEnd && $i < $xF; $i++) {
    $itemId = $regF[1][$i];
    $itemCount = $regF[2][$i];
    $item = findItemByFullId($itemId);
    $tmp = $item[0];
    if (strpos($itemId, "..") !== false) {
        $tmp .= " *";
    }
    if ($itemCount > 1) {
        $tmp .= " (" . $regF[2][$i] . ")";
    }
    $tmp .= " за " . round($item[1] * $trader[0]) . " монет";
    $stmp .= "\n" . '<br/><a tabindex="0" class="btn btn-secondary m-1" role="button" data-trigger="focus" data-toggle="popover" data-content="template" title="' . $tmp . '" label="' . $itemId . '">' . $tmp . '</a>';
}
if ($stmp) {
    $stmp = "<p>Я продаю:" . $stmp;
} else {
    msg("У меня сейчас нет товаров на продажу, все раскупили. Приходи в другой раз.", $tchar[0]);
}
if ($start) {
    $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id\">^ </a>";
}
if ($listEnd < $xF - 1) {
    if (!$start) {
        $stmp .= "\n<br/>";
    }
    $stmp .= "<a href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=" . ($listEnd) . "\">+ (" .
        ($xF - $listEnd) . ")</a>";
}
$stmp .= "\n</p>\n</div>\n<div id=\"menu\" class=\"d-none\" title=\"Меню\">\n";
$stmp .= "<p>\n<a class=\"btn btn-outline-primary btn-sm m-1 \" href=\"$PHP_SELF?sid=$sid&speak=$speak&id=$id&start=$start&to=$(to)\">Купить</a>\n";
$stmp .= "<br/><a class=\"btn btn-outline-secondary btn-sm m-1\" href=\"$PHP_SELF?sid=$sid&look=$(to)\">Инфо</a>";
msg($stmp, $tchar[0]);