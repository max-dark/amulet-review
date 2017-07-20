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
if (!empty($to)) {
    $stmp .= "<input name=\"to\" emptyok=\"true\" type=\"text\" value=\"$to\"/><br />";
}
$stmp .= "<input name=\"say\" emptyok=\"true\" type=\"text\" maxlength=\"250\" value=\"\"/>\n";
$stmp .= "<br/>\n";
$stmp .= "<anchor>Сказать\n";
$stmp .= "<go href=\"?sid=$sid\" method=\"post\">\n";
$stmp .= "<postfield name=\"to\" value=\"$(to)\"/>\n";
$stmp .= "<postfield name=\"say\" value=\"$(say)\"/>\n";
$stmp .= "</go></anchor>";

if (!empty($tt)) {
    $stmp .= "<br/><a href=\"?sid=$sid&trade=$tt\">Обмен</a>";
}

msg($stmp, "Сказать", 1);
