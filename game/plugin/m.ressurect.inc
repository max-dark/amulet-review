<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global to
 */

$tc = explode("|", $loc_i[$loc][$to]["char"]);
if ($tc[8]) {
    ressurect($loc, $to);
} else {
    addjournal($loc, $login, $tc[0] . " не призрак");
}
