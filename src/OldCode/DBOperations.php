<?php
/**
 * @copyright Copyright (C) 2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */

namespace MaxDark\Amulet\OldCode;

class DBOperations
{
    /** @var \PDO $link */
    private static $link = null;

    /**
     * Init DB connection
     *
     * @return string error message
     */
    private static function openDB()
    {
        // TODO: remove globals
        global $server, $user, $dbpass, $dbname;
        $msg = '';
        try {
            self::db([
                'server' => $server,
                'dbname' => $dbname,
                'login' => $user,
                'password' => $dbpass,
            ]);
        } catch (\PDOException $e) {
            $msg = defined('DEBUG') ? $e->getMessage() : "База данных недоступна. Повторите через 5мин";
        }

        return $msg;
    }

    /**
     * Получение массива параметров для пользователя $nick.
     *
     * FIXME: имя функции не соответствует выполняемым действиям
     *
     * @param string $nick
     * @param string $pass
     * @param string $fields
     * @param bool   $skippass
     *
     * @return array [message, result]
     */
    private static function checkpass($nick, $pass, $fields, $skippass)
    {
        // TODO: remove globals
        global $PassDelay;
        if ($fields == "") {
            $fields = "`pass`,`lastrefr`";
        } else {
            if ($fields !== "*") {
                $fields .= ",`pass`,`lastrefr`";
            }
        }

        $now     = time();
        $result  = [];
        $message = '';
        $sql     = "select $fields from `users` where `nick` = :nickname";
        $query   = self::db()->prepare($sql);
        $query->execute([
            ':nickname' => $nick
        ]);
        if ($query->rowCount() != 1) {
            $message = "Логин не найден";
        } else {
            $result = $query->fetch(\PDO::FETCH_ASSOC);
            $dt     = $PassDelay - $now + intval($result['lastrefr']);
            if ($dt > 0) {
                $message = "Повторите через $dt sec";
            } else {
                if ($result['pass'] != $pass && ! $skippass) {
                    $sql   = "UPDATE `users` SET `lastrefr` = :lastrefr WHERE `nick` = :nickname";
                    $query = self::db()->prepare($sql);
                    $query->execute([
                        ':lastrefr' => $now,
                        ':nickname' => $nick
                    ]);
                    $message = "Неверный пароль";
                }
            }
        }

        return [$message, $result];
    }

    /**
     * Записывает gamedata в БД
     * Возвращает пустую строку в случае успеха или сообщение об ошибке.
     *
     * @param string $login логин
     * @param string $pass пароль
     * @param string $data данные для сохранения
     *
     * @return string error message
     */
    public static function setData($login, $pass, $data)
    {
        if (empty($login)) {
            return "Логин не задан";
        }
        if (empty($pass)) {
            return "Пароль не задан";
        }

        $maxdata = 5000; // Максимальная длина данных
        if (strlen($data) > $maxdata) {
            return "Слишком длинная строка.";
        }

        $error = DBOperations::openDB();
        if ($error != "") {
            return $error;
        }

        list($status, $result) = DBOperations::checkpass($login, $pass, '`names`,`vals`', true);    // сохраняет без пароля!
        if ($status != "") {
            return $status;
        }
        Params::init($result["names"], $result["vals"]);

        Params::setParam('gamedata', $data);

        $sqlUpd = 'UPDATE `users` SET `names` = :vnames, `vals` = :vals WHERE `nick` = :nickname';
        self::db()->prepare($sqlUpd)->execute([
            ':vnames'   => Params::getNames(),
            ':vals'     =>  Params::getValues(),
            ':nickname' => $login
        ]);

        return "";
    }

    /**
     * Возвращает пустую строку в случае успеха (данные возвращаются в $data) или сообщение об ошибке.
     *
     * @param string $login
     * @param string $pass
     *
     * @return array
     */
    public static function getData($login, $pass)
    {
        if (empty($login)) {
            return ["Логин не задан", null];
        }
        if (empty($pass)) {
            return ["Пароль не задан", null];
        }

        $error = DBOperations::openDB();
        if ($error != "") {
            return [$error, null];
        }

        list($message, $result) = DBOperations::checkpass($login, $pass, "`names`,`vals`", false);
        $data = [];
        if ($message == "") {
            Params::init($result["names"], $result["vals"]);
            $data = Params::getParam("gamedata");
            if ($data == Params::NOT_SET) {
                $message = "Данные не найдены";
            }
        }

        return [$message, $data];
    }

    /**
     * @param string $nickname
     * @return bool
     */
    public static function nicknameExists($nickname)
    {
        DBOperations::openDB();
        $sql   = "SELECT `ind` FROM `users` WHERE `nick` = :nickname limit 1";
        $query = self::db()->prepare($sql);
        $query->execute([
            ':nickname' => $nickname
        ]);
        return $query->rowCount() !== 0;
    }

    /**
     * @param int $now
     * @param string $nn
     * @param string $pass
     * @param string $email
     */
    public static function insert($now, $nn, $pass, $email)
    {
        DBOperations::openDB();
        $sqlLogin = 'INSERT INTO `users` (regtime,nick,pass,email) VALUES (?, ?, ?, ?)';
        self::db()->prepare($sqlLogin)->execute([
            $now,
            $nn,
            $pass,
            $email
        ]);
    }

    /**
     * @param array|null $config database config(server, dbname, login, password)
     *
     * @return \PDO link to database
     * @throws \PDOException if connect fail
     */
    private static function db($config = null)
    {
        if (is_null(self::$link)) {
            if (is_array($config)) {
                self::$link = new \PDO(
                    sprintf('mysql:host=%s;dbname=%s;charset=utf8', $config['server'], $config['dbname']),
                    $config['login'],
                    $config['password'],
                    [\PDO::ATTR_ERRMODE => \PDO::ERRMODE_EXCEPTION]
                );
            }
        }

        return self::$link;
    }
}