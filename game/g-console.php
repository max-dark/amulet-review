<?php
$QUERY_STRING = '';
$args = array_slice($argv, 1);
if(count($args)) {
    $QUERY_STRING = implode('&', $args);
}
require_once 'g.php';