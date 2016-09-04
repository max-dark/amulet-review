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
 * @global to
 */

if ($to && isset($loc_i[$loc][$to])) {
    $tt    = $to;
    $tchar = explode("|", $loc_i[$loc][$to]["char"]);
    $to    = $tchar[0] . ", ";
} else {
    $to = "";
}
$stmp = "<p>";
if ($to) {
    $stmp .= "<input name='to' emptyok='true' type='text' value='" . $to . "'/><br />";
}
$stmp .= "
<input name='say' emptyok='true' type='text' maxlength='250' value=''/>
<br/>
<anchor>Сказать
<go href='?sid=" . $sid . "' method='post'>
<postfield name='to' value='$(to)'/>
<postfield name='say' value='$(say)'/>
</go></anchor>";

if ($tt) {
    $stmp .= "<br/><a href='?sid=" . $sid . "&trade=" . $tt . "'>Обмен</a>";
}

msg($stmp, "Сказать", 1);
