<?php

header("Expires: Thu, 01 Jan 2010 00:00:01 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-cache, no-store, must-revalidate, max-age=0");
header("Pragma: no-cache");
header("Content-type:text/vnd.wap.wml;charset=utf-8");
//header( "Content-type:text/vnd.wap.wml" );
echo "<?xml version='1.0'?><!DOCTYPE wml PUBLIC '-//WAPFORUM//DTD WML 1.3//EN' 'http://www.wapforum.org/DTD/wml13.dtd'>";

$starttime = microtime(1);

//=========================
$server = 'localhost';
$user = 'game'; // пользователь базы
$dbpass = 'pass';     // пароль пользователя
$dbname = 'amulet'; // имя базы
//=========================

$SERVER_URL = 'http://vostanie.hostik.in/'; //путь к игре
$PHP_SELF = 'g.php'; //   мотор игры
$GAME_NAME = 'Моя игра';
$g_max = 100; //   максимум игроков
$g_admin = 'u.user'; //  логин админа
$gm_id = 'hrenvam'; // вход админом
$gg_list = 5;
$g_list = 5;
$g_size = 2200;
$g_menu = 0;
$g_smenu = '301021';
$g_j2loc = 1;
$g_j2go = 1;
$g_joff = 0;
$g_smf = 0;
$g_map = 3;
$g_sounds = 0;
$g_logout = 300;
$g_destroy = 600;
$g_crim = 1800;
$g_exp = 10;
$g_attr = 12;
$g_attr_one = 5;
$g_skills = 50;
$g_skills_one = 5;
$g_ch = 0;
$loc_i = array();
$loc_t = array();
$loc_tt = array();
//==================================ПЕРЕХОД В REGISTER_GLOBALS OFF!!!!!!
$cnt_get = count($_GET);
if ($cnt_get) {
    $key = array_keys($_GET);
    $vals = array_values($_GET);
    for ($i = 0; $i < $cnt_get; $i++) {
        if (!$vals[$i])
            continue;
        if (!preg_match("|^[-a-z0-9_\.]+$|i", $vals[$i]))
            exit('Вы используете запрещенные символы ' . $v[0]);
        eval('$' . $key[$i] . '="' . $vals[$i] . '";');
    }
    unset($cnt_get, $key, $vals);
}
//------------------------------
$cnt_post = count($_POST);
if ($cnt_post) {
    $key = array_keys($_POST);
    $vals = array_values($_POST);
    for ($i = 0; $i < $cnt_post; $i++) {
        eval('$' . $key[$i] . '="' . $vals[$i] . '";');
    }
    unset($cnt_post, $key, $vals);
}
//===============================================================
