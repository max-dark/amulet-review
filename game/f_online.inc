<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global to
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global id
 * @global skills
 */
// проверка всех онлайн и удаление в оффлайн
require_once("datafunc.php");

$count = 0;

$excluded = [
    ".",
    "..",
    ".htaccess",
    "1.htaccess"
];

$dh = opendir("online/");
while (($fname = readdir($dh)) !== false) {
    if (in_array($fname, $excluded)) {
        continue;
    }

    $count++;
    $tmp = file("online/" . $fname);
    if ($tmp[0] == ".") {
        if (filemtime("online/" . $fname) < time() - 60) {
            @unlink("online/" . $fname);
        }
        continue;
    }
    $tmp[0] = trim($tmp[0]);
    $tmp[1] = intval(trim($tmp[1]));

    /** @var array $tavern_rooms комнаты в таверне */
    $tavern_rooms = [
        "x1087x543",
        "x1080x539",
        "x1080x532",
        "x1087x528"
    ];
    if (time() > $tmp[1] + $g_logout || $f_all == 1 || $f_all == $tmp[0]) {
        loadloc($tmp[0]);
        $res = "";
        if (isset($loc_i[$tmp[0]][$fname])) {
            if ($game["floc"] == $tmp[0] && $game["fid"] == $fname) {
                $loc1                   = (in_array($tmp[0], $tavern_rooms)) ? "x1095x532" : $tmp[0];
                $loc_i[$loc1]["i.flag"] = "флаг лидерства|1|0";
                $game["floc"]           = $loc1;
                $game["fid"]            = "";
                calcparam($tmp[0], $fname);
            }
            $auser            = $loc_i[$tmp[0]][$fname];
            $auser["journal"] = "";
            $auser["loc"]     = $tmp[0];
            $auser["time"]    = time();
            unset($auser["msgt"]);
            unset($auser["macrol"]);
            unset($auser["trade"]);
            $tp = explode("|", $auser["user"]);
            if ($auser["user"] && $auser["char"] && $auser["skills"] && $auser["war"]) {
                $res = SetData(substr($fname, 2), $tp[0], serialize($auser));
                if ( ! $res) {
                    addnpc($fname, $tmp[0], "");
                }
            }
            if ( ! $res || $f_all == 1 || $f_all == $tmp[0]) {
                unset($loc_i[$tmp[0]][$fname]);
            }
        }
        if ( ! $res || $f_all == 1 || $f_all == $tmp[0]) {
            @unlink("online/" . $fname);
            $count--;
        }
    }
}
closedir($dh);

if ($count > $g_max) {
    $count = $g_max;
}
$file = fopen("data/count.dat", "w");
if ($file !== false) {
    fputs($file, $count . " из " . $g_max);
    fclose($file);
}

$game["lastai"] = time();