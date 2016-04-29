<?php

// возвращает -1 если есть $login в папке online
$tmp = $QUERY_STRING;
if ($tmp == '')
    $tmp = $_SERVER["QUERY_STRING"];
$tmp = urldecode($tmp);
parse_str($tmp);

if ($login)
    if (file_exists("online/" . $login))
        die("yes");
    else
        die("no");
if ($count)
    die(@implode("", @file("count.dat")));
?>