<?php
/**
 * Конфигурация игры
 * @file game/config.php
 */

//=========================
/** @var string $server */
$server = 'localhost';
/** @var string $user пользователь базы */
$user = 'game';
/** @var string $dbpass пароль пользователя */
$dbpass = 'pass';
/** @var string $dbname имя базы */
$dbname = 'amulet';

//=========================

/** @var string $PHP_SELF мотор игры */
$PHP_SELF = 'index.php';
/** @var string $GAME_NAME */
$GAME_NAME = 'Моя игра';
/** @var int $g_max максимум игроков */
$g_max = 100;
/** @var string $g_admin логин админа */
$g_admin = 'u.user';
/** @var string $gm_id вход админом для обслуживания */
$gm_id = 'hrenvam';

/** @var int $g_list Размер списков (3..30) */
$g_list = 5;
/**
 * Размер страницы (700..15000)
 *
 * @var int $g_size
 */
$g_size = 2200;
/**
 * Тип меню: 0 - полное, 1 - сокращенное, 2 - на отдельной странице,
 * 3 - в виде ссылок (если не видно обычное меню)
 *
 * @var int $g_menu
 */
$g_menu = 0;
/**
 * Пункты в меню (0-откл,1-магия,2-предмет,3-прием)
 * и кол-во горячих клавиш для каждого пункта (0..9),
 * порядок произвольный. Пример: 332110
 *
 * @var string $g_smenu
 */
$g_smenu = '301021';
/** @var int $g_j2loc Сообщать о приходящих (1-вкл,0-выкл) */
$g_j2loc = 1;
/** @var int $g_j2go Описание локаций (1-вкл,0-выкл) */
$g_j2go = 1;
/** @var int $g_smf Отключить журнал (1-да,0-нет) */
$g_joff = 0;
/** @var int $g_smf Маленький шрифт (1-да,0-нет) */
$g_smf = 0;
/** @var int $g_map Карта: 0 - нет, 1 - ч/б, 2 - цветная JPEG, 3 - цветная PNG */
$g_map = 3;
/** @var int $g_sounds Звуки рядом с выходами (1-вкл,0-выкл) */
$g_sounds = 0;

/** @var int $g_logout время до логоута */
$g_logout = 300;
/** @var int $g_destroy время */
$g_destroy = 600;
/** @var int $g_crim время, на которое игрок считается преступником */
$g_crim = 1800;
/** @var int $g_exp множитель для экспы */
$g_exp = 10;
/** @var int $g_attr максимальная сумма сила+ловкость+интелект */
$g_attr = 12;
/** @var int $g_attr_one максимальное значение силы/ловкости/интелекта */
$g_attr_one = 5;
/** @var int $g_skills максимальная сумма очков навыков */
$g_skills = 50;
/** @var int $g_skills_one максимальный уровень навыка */
$g_skills_one = 5;
