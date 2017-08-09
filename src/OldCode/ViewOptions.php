<?php
/**
 * @copyright Copyright (C) 2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */


namespace MaxDark\Amulet\OldCode;

/**
 * Class ViewOptions
 *
 * Хранит опции отображения страницы.
 *
 * Доступ реализован через Singleton.
 *
 * TODO: В дальнейшем нужно переделать на поле класса User
 *
 * @package MaxDark\Amulet\OldCode
 */
class ViewOptions
{
    /** @var ViewOptions */
    private static $instance = null;

    /** @var  int Количество элементов списка на одной странице (3..30) */
    private $maxListSize;

    /** @var int Размер страницы (700..15000) */
    private $maxPageSize;

    /** @var int|bool флаг, Сообщать о приходящих (1-вкл,0-выкл) */
    private $reportIncoming;

    /** @var int|bool флаг, Отображение описания локаций при переходе (1-вкл,0-выкл) */
    private $showDesc;

    /** @var int Тип меню, варианты значений прописаны в MenuMode */
    private $menuMode;

    /** @var int|bool флаг, Определяет как показывается наличие пользователей/НПС в соседних локах */
    private $soundsMode;

    /** @var int|bool флаг, Отключить отображение журнала (1-да,0-нет) */
    private $journalDisabled;

    /** @var string Дополнительные пункты в меню для быстрого доступа к предметам и умениям. */
    private $userMenu;

    /** @var int Определяет отображение ссылки на карту и ее тип */
    private $mapMode;

    /** @var int|bool флаг, Использовать маленький шрифт (1-да,0-нет) */
    private $useSmallFont;

    /** @var int|bool флаг, определяет будет ли отображено описание локации */
    private $descEnabled;

    public function __construct()
    {
        $this->descEnabled = false;
    }

    /**
     * @return ViewOptions
     */
    public static function getInstance()
    {
        if (is_null(self::$instance)) {
            self::$instance = new self();
        }
        return self::$instance;
    }

    /**
     * заполняет параметры из строки
     *
     * @param string $viewOptions
     * @return $this
     */
    public function fromString($viewOptions)
    {
        $o = explode('|', $viewOptions);

        $this->setMaxListSize($o[0]);
        $this->setMaxPageSize($o[1]);
        $this->setReportIncoming($o[2]);
        $this->setShowDesc($o[3]);
        $this->setMenuMode($o[4]);
        $this->setSoundsMode($o[5]);
        $this->setJournalDisabled($o[6]);
        $this->setUserMenu($o[7]);
        $this->setMapMode($o[8]);
        $this->setUseSmallFont($o[9]);

        return $this;
    }

    /**
     * собирает строку из параметров
     *
     * @return string
     */
    public function toString()
    {
        return implode('|', [
            $this->getMaxListSize(),
            $this->getMaxPageSize(),
            $this->getReportIncoming(),
            $this->getShowDesc(),
            $this->getMenuMode(),
            $this->getSoundsMode(),
            $this->getJournalDisabled(),
            $this->getUserMenu(),
            $this->getMapMode(),
            $this->getUseSmallFont()
        ]);
    }

    /**
     * Количество элементов списка на одной странице (3..30).
     *
     * Если размер списка превышает его, то будет разбит на несколько с возможностью перелистывания.
     *
     * @return int
     */
    public function getMaxListSize()
    {
        return $this->maxListSize;
    }

    /**
     * Количество элементов списка на одной странице (3..30).
     *
     * Если размер списка превышает его, то будет разбит на несколько с возможностью перелистывания.
     *
     * @param int $maxListSize
     */
    public function setMaxListSize($maxListSize)
    {
        $this->maxListSize = intval($maxListSize);
    }

    /**
     * Размер страницы (700..15000)
     *
     * @return int
     */
    public function getMaxPageSize()
    {
        return $this->maxPageSize;
    }

    /**
     * Размер страницы (700..15000)
     *
     * @param int $maxPageSize
     */
    public function setMaxPageSize($maxPageSize)
    {
        $this->maxPageSize = intval($maxPageSize);
    }

    /**
     * Сообщать о приходящих (1-вкл,0-выкл)
     *
     * @return bool|int
     */
    public function getReportIncoming()
    {
        return $this->reportIncoming;
    }

    /**
     * Сообщать о приходящих (1-вкл,0-выкл)
     *
     * @param bool|int $reportIncoming
     */
    public function setReportIncoming($reportIncoming)
    {
        $this->reportIncoming = intval($reportIncoming);
    }

    /**
     * Отображение описания локаций при переходе (1-вкл,0-выкл)
     *
     * @return bool|int
     */
    public function getShowDesc()
    {
        return $this->showDesc;
    }

    /**
     * Отображение описания локаций при переходе (1-вкл,0-выкл)
     *
     * @param bool|int $showDesc
     */
    public function setShowDesc($showDesc)
    {
        $this->showDesc = intval($showDesc);
    }

    /**
     * Тип меню.
     *
     * 0 - полное,
     * 1 - сокращенное,
     * 2 - на отдельной странице,
     * 3 - в виде ссылок (если не видно обычное меню)
     *
     * @return int
     */
    public function getMenuMode()
    {
        return $this->menuMode;
    }

    /**
     * Тип меню.
     *
     * 0 - полное,
     * 1 - сокращенное,
     * 2 - на отдельной странице,
     * 3 - в виде ссылок (если не видно обычное меню)
     *
     * @param int $menuMode
     */
    public function setMenuMode($menuMode)
    {
        $this->menuMode = intval($menuMode);
    }

    /**
     * "Звуки".
     *
     * Определяет как показывается наличие пользователей/НПС в соседних локах
     *
     * 0 - в виде списка названий переходов
     * 1 - "!" рядом с переходами
     *
     * @return bool|int
     */
    public function getSoundsMode()
    {
        return $this->soundsMode;
    }

    /**
     * "Звуки".
     *
     * Определяет как показывается наличие пользователей/НПС в соседних локах
     *
     * 0 - в виде списка названий переходов
     * 1 - "!" рядом с переходами
     *
     * @param bool|int $soundsMode
     */
    public function setSoundsMode($soundsMode)
    {
        $this->soundsMode = intval($soundsMode);
    }

    /**
     * Отключить отображение журнала (1-да,0-нет)
     *
     * @return bool|int
     */
    public function getJournalDisabled()
    {
        return $this->journalDisabled;
    }

    /**
     * Отключить отображение журнала (1-да,0-нет)
     *
     * @param bool|int $journalDisabled
     */
    public function setJournalDisabled($journalDisabled)
    {
        $this->journalDisabled = intval($journalDisabled);
    }

    /**
     * Дополнительные пункты в меню для быстрого доступа к предметам и умениям.
     *
     * (0-откл,1-магия,2-предмет,3-прием)
     * и кол-во горячих клавиш для каждого пункта (0..9),
     * порядок произвольный. Пример: 332110
     *
     * @return int
     */
    public function getUserMenu()
    {
        return $this->userMenu;
    }

    /**
     * Дополнительные пункты в меню для быстрого доступа к предметам и умениям.
     *
     * (0-откл,1-магия,2-предмет,3-прием)
     * и кол-во горячих клавиш для каждого пункта (0..9),
     * порядок произвольный. Пример: 332110
     *
     * @param int $userMenu
     */
    public function setUserMenu($userMenu)
    {
        $this->userMenu = intval($userMenu);
    }

    /**
     * Отображение ссылки на карту и ее тип.
     *
     * 0 - отключена
     * 1 - ч/б
     * 2 - цветная JPEG
     * 3 - цветная PNG
     *
     * @return int
     */
    public function getMapMode()
    {
        return $this->mapMode;
    }

    /**
     * Отображение ссылки на карту и ее тип.
     *
     * 0 - отключена
     * 1 - ч/б
     * 2 - цветная JPEG
     * 3 - цветная PNG
     *
     * @param int $mapMode
     */
    public function setMapMode($mapMode)
    {
        $this->mapMode = intval($mapMode);
    }

    /**
     * Использовать маленький шрифт.
     *
     * 1-да, 0-нет
     *
     * @return bool|int
     */
    public function getUseSmallFont()
    {
        return $this->useSmallFont;
    }

    /**
     * Использовать маленький шрифт.
     *
     * 1-да, 0-нет
     *
     * @param bool|int $useSmallFont
     */
    public function setUseSmallFont($useSmallFont)
    {
        $this->useSmallFont = intval($useSmallFont);
    }

    /**
     * @return bool|int
     */
    public function getDescEnabled()
    {
        return $this->descEnabled;
    }

    /**
     * @param bool|int $descEnabled
     */
    public function setDescEnabled($descEnabled)
    {
        $this->descEnabled = $descEnabled;
    }
}