<?php
/**
 * Конфигурация игры
 *
 * @file game/config.php
 */

use MaxDark\Amulet\OldCode\DBConfig;
use MaxDark\Amulet\OldCode\MenuMode;
use MaxDark\Amulet\OldCode\ViewOptions;

// class loader bootstrap
require_once '../vendor/autoload.php';

//=========================
// Настройки БД
//=========================

DBConfig::setConfig([
    'server' => 'localhost',
    'dbname' => 'amulet',
    'login' => 'game',
    'password' => 'pass',
]);

//=========================
// структура каталогов
//=========================

const BASE_DIR = __DIR__ . DIRECTORY_SEPARATOR;
const NPC_DIR = BASE_DIR . 'npc' . DIRECTORY_SEPARATOR;
const ITEMS_DIR = BASE_DIR . 'items' . DIRECTORY_SEPARATOR;
const SPEAK_DIR = BASE_DIR . 'speak' . DIRECTORY_SEPARATOR;

//=========================
// настройки движка
//=========================

/**
 * движок игры
 *
 * @var string $PHP_SELF
 */
$PHP_SELF = 'index.php';

/**
 * Название игры
 *
 * @var string $GAME_NAME
 */
$GAME_NAME = 'Моя игра';

/**
 * максимум игроков на сервере.
 *
 * Недоделано - количество считается, но ограничения нет
 *
 * @var int $g_max
 */
$g_max = 100;

/**
 * логин админа
 *
 * @var string $g_admin
 */
$g_admin = 'u.user';

/**
 * ключ для входа админом
 *
 * @var string $gm_id
 */
$gm_id = 'hrenvam';

/**
 * время бездействия до логоута
 *
 * @var int $g_logout
 */
$g_logout = 300;

/**
 * время до изчезновения предмета
 *
 * @var int $g_destroy
 */
$g_destroy = 600;

/**
 * время на которое игрок считается нарушителем(кримом)
 *
 * @var int $g_crim
 */
$g_crim = 1800;

/**
 * @var int $g_exp множитель для экспы
 */
$g_exp = 10;

/**
 * @var int $g_attr максимальная сумма сила+ловкость+интелект
 */
$g_attr = 12;
/**
 * @var int $g_attr_one максимальное значение силы/ловкости/интелекта
 */
$g_attr_one = 5;

/**
 * @var int $g_skills максимальная сумма очков навыков
 */
$g_skills = 50;

/**
 * @var int $g_skills_one максимальный уровень навыка
 */
$g_skills_one = 5;

//============================
// Пользовательские настройки
//============================

$pageOpt = ViewOptions::getInstance();

/**
 * Количество элементов списка на одной странице (3..30).
 *
 * Если размер списка превышает его, то будет разбит на несколько с возможностью перелистывания.
 *
 * @var int $g_list
 */
$g_list = &$pageOpt->setListsSize(5);

/**
 * Размер страницы (700..15000)
 *
 * @var int $g_size
 */
$g_size = &$pageOpt->setMaxPageSize(2200);

/**
 * Тип меню.
 *
 * 0 - полное,
 * 1 - сокращенное,
 * 2 - на отдельной странице,
 * 3 - в виде ссылок (если не видно обычное меню)
 *
 * @var int $g_menu
 */
$g_menu = &$pageOpt->setMenuMode(MenuMode::FULL);

/*
 * Дополнительные пункты в меню для быстрого доступа к предметам и умениям.
 *
 * (0-откл,1-магия,2-предмет,3-прием)
 * и кол-во горячих клавиш для каждого пункта (0..9),
 * порядок произвольный. Пример: 332110
 *
 */
$pageOpt->setUserMenu('301021');

/**
 * Сообщать о приходящих (1-вкл,0-выкл)
 *
 * @var int $g_j2loc
 */
$g_j2loc = &$pageOpt->setReportIncoming(1);

/*
 * Отображение описания локаций при переходе (1-вкл,0-выкл)
 */
$pageOpt->setShowDesc(1);

/*
 * Отключить отображение журнала (1-да,0-нет)
 */
$pageOpt->setJournalDisabled(0);

/*
 * Использовать маленький шрифт.
 *
 * 1-да, 0-нет
 *
 */
$pageOpt->setUseSmallFont(0);

/*
 * Отображение ссылки на карту и ее тип.
 *
 * 0 - отключена
 * 1 - ч/б
 * 2 - цветная JPEG
 * 3 - цветная PNG
 *
 */
$pageOpt->setMapMode(3);

/*
 * "Звуки".
 *
 * Определяет как показывается наличие пользователей/НПС в соседних локах
 *
 * 0 - в виде списка названий переходов
 * 1 - "!" рядом с переходами
 */
$pageOpt->setSoundsMode(0);

//=========================
// прочие настройки
//=========================

/**
 * флаг "сбросить все" для обновления списка пользователей.
 *
 * 0 - как обычно
 * 1 - выгнать всех
 * "loc" - id локации из которой выгнать пользователей
 *
 * @var int|string $f_all
 */
$f_all = 0;
