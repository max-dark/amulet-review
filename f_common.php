<?php

// список у Санчеса
if ($gwanted)
    die(@implode("", @file("wanted.dat")));
if ($swanted) {
    $file = fopen("wanted.dat", "w");
    if ($file !== false) {
        fputs($file, str_replace("\\", "", $swanted));
        fclose($file);
    }
}

// кланы

if ($gclan) {
    if (!file_exists("clans/" . $gclan))
        die("none");
    else
        die(@implode("", @file("clans/" . $gclan)));
}
if ($inclan) {
    $dh = opendir("clans");
    while (($fname = readdir($dh)) !== false)
        if ($fname != "." && $fname != ".." && $fname != "1.htaccess" && $fname != ".htaccess") {
            if (strtolower($fname) == strtolower($inclan)) {
                closedir($dh);
                $tmp = @unserialize(@implode("", @file("clans/" . $fname)));
                if (gettype($tmp) == "array" && !isset($tmp["m"][$login]) && !isset($tmp["g"][$login]))
                    die("no");
                die("yes");
            }
        }
    closedir($dh);
    die("no");
}
if ($sclan) {
    $test = unserialize(str_replace("\\", "", $data));
    if ($test["g"]) {
        $file = fopen("clans/" . $sclan, "w");
        if ($file !== false) {
            fputs($file, str_replace("\\", "", $data));
            fclose($file);
        }
    } else
        die("err:");
}
if ($dclan) {
    unlink("clans/" . $dclan);
    die("ok:");
}
if ($eclan) {
    $dh = opendir("clans");
    while (($fname = readdir($dh)) !== false)
        if ($fname != "." && $fname != ".." && $fname != "1.htaccess" && $fname != ".htaccess") {
            if (strtolower($fname) == strtolower($eclan)) {
                closedir($dh);
                die("yes");
            }
        }
    closedir($dh);
    die("no");
}
