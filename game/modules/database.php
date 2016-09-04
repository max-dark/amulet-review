<?php

/**
 * Class DB
 */
class DB
{
    /** @var \PDO $link */
    private static $link = null;

    /**
     * @param array|null $config database config(server, dbname, login, password)
     *
     * @return \PDO link to database
     * @throws \PDOException if connect fail
     */
    public static function link($config = null)
    {
        if (is_null(self::$link)) {
            if (is_array($config)) {
                self::$link = new \PDO(sprintf('mysql:host=%s;dbname=%s;charset=utf8', $config['server'],
                    $config['dbname']), $config['login'], $config['password'], [
                        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION
                    ]);
            }
        }

        return self::$link;
    }
}