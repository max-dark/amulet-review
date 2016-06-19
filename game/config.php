<?php
/**
 * @file game/config.php
 */

/** @var int $starttime */
$starttime = microtime(1);

//=========================
/** @var string $server */
$server = 'localhost';
/** @var string $user */
$user = 'game'; // пользователь базы
/** @var string $dbpass */
$dbpass = 'pass';     // пароль пользователя
/** @var string $dbname */
$dbname = 'amulet'; // имя базы
//=========================
/** @var string $SERVER_URL урл*/
$SERVER_URL = 'http://ip6-localhost:8000/'; //путь к игре
/** @var string $PHP_SELF */
$PHP_SELF = 'g.php'; //   мотор игры
/** @var string $GAME_NAME */
$GAME_NAME = 'Моя игра';
/** @var int $g_max */
$g_max = 100; //   максимум игроков
/** @var string $g_admin */
$g_admin = 'u.user'; //  логин админа
/** @var string $gm_id */
$gm_id = 'hrenvam'; // вход админом

/** @var int $gg_list хз что, вроде не используется */
$gg_list = 5;
//Размер списков (3..30):($g_list)
$g_list = 5;
//Размер страницы (700..15000):($g_size)
$g_size = 2200;
//Тип меню: 0 - полное, 1 - сокращенное, 2 - на отдельной странице,
//3 - в виде ссылок (если не видно обычное меню):($g_menu)
$g_menu = 0;
//Пункты в меню (0-откл,1-магия,2-предмет,3-прием) и кол-во горячих клавиш для каждого пункта (0..9),
//порядок произвольный. Пример: 332110($g_smenu)
$g_smenu = '301021';
//Сообщать о приходящих (1-вкл,0-выкл):($g_j2loc)
$g_j2loc = 1;
//Описание локаций (1-вкл,0-выкл):($g_j2go)
$g_j2go = 1;
//Отключить журнал (1-да,0-нет):($g_joff)
$g_joff = 0;
//Маленький шрифт (1-да,0-нет):($g_smf)
$g_smf = 0;
//Карта: 0 - нет, 1 - ч/б, 2 - цветная JPEG, 3 - цветная PNG:($g_map)
$g_map = 3;
//Звуки рядом с выходами (1-вкл,0-выкл):($g_sounds)
$g_sounds = 0;

/** @var int $g_logout время до логоута */
$g_logout = 300;
/** @var int $g_destroy время */
$g_destroy = 600;
/** @var int $g_crim время */
$g_crim = 1800;
/** @var int $g_exp множитель для экспы */
$g_exp = 10;
$g_attr = 12;
$g_attr_one = 5;
$g_skills = 50;
$g_skills_one = 5;
/** @var array $loc_i предметы в текущей локе */
$loc_i = array();
/** @var array $loc_t таймеры в текущей локе */
$loc_t = array();
/** @var array $loc_tt текущая локация */
$loc_tt = array();
//==================================ПЕРЕХОД В REGISTER_GLOBALS OFF!!!!!!
$cnt_get = count($_GET);
if ($cnt_get) {
    $key = array_keys($_GET);
    $vals = array_values($_GET);
    for ($i = 0; $i < $cnt_get; $i++) {
        if (!$vals[$i])
            continue;
        if (!preg_match('|^[-a-z0-9_\.]+$|i', $vals[$i]))
            exit('Вы используете запрещенные символы ');
        $GLOBALS[$key[$i]] = $vals[$i];
    }
    unset($cnt_get, $key, $vals);
}
//------------------------------
$cnt_post = count($_POST);
if ($cnt_post) {
    $key = array_keys($_POST);
    $vals = array_values($_POST);
    for ($i = 0; $i < $cnt_post; $i++) {
        $GLOBALS[$key[$i]] = $vals[$i];
    }
    unset($cnt_post, $key, $vals);
}
//===============================================================
