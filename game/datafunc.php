<?php
/**
 * Функции для работы с БД
 */

use MaxDark\Amulet\database\DB;

$NOT_SET = "NOT_SET";

// Антимат-фильтр ( чтобы ники матерные не регистрировали)
require("antimat.inc");

function InitParam($N, $V)
{
    global $Names, $Values;
    $Names  = $N;
    $Values = $V;
}

function GetParam($Name)
{
    global $Names, $Values, $NOT_SET;

    $Name  = strtolower($Name);
    $Nlist = explode(":", $Names);
    for ($i = 0; $i < count($Nlist); $i++) {
        if ($Nlist[$i] == $Name) {
            break;
        }
    }
    if ($i == count($Nlist)) {
        return $NOT_SET;
    }
    $Vlist = explode(":", $Values);

    return stripslashes(str_replace("!~!", ":", $Vlist[$i]));
}

function SetParam($Name, $Value)
{
    global $Names, $Values, $NOT_SET;
    $Nlist = explode(":", $Names);
    $Name  = strtolower($Name);
    $Value = addslashes(str_replace(":", "!~!", $Value));
    for ($i = 0; $i < count($Nlist); $i++) {
        if ($Nlist[$i] == $Name) {
            break;
        }
    }

    if ($i == count($Nlist) and ($Value != $NOT_SET)) { // Добавляем имя и значение
        $Names .= ":$Name";
        $Values .= ":$Value";
    } else {
        $Vlist     = explode(":", $Values);
        $Vlist[$i] = $Value;
        $Values    = implode(":", $Vlist);
        if ($Value == $NOT_SET) { // Удаление имени и значения
            $Nlist[$i] = $NOT_SET;
            $Names     = implode(":", $Nlist);
            $Names     = str_replace(":$NOT_SET", "", $Names);
            $Values    = str_replace(":$NOT_SET", "", $Values);
        }
    }
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
function checkpass($nick, $pass, $fields, $skippass = false)
{
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
    $query   = DB::link()->prepare($sql);
    $query->execute([
            ':nickname' => $nick
        ]);
    if ($query->rowCount() != 1) {
        $message = "Логин не найден";
    } else {
        $result = $query->fetch(PDO::FETCH_ASSOC);
        $dt     = $PassDelay - $now + intval($result['lastrefr']);
        if ($dt > 0) {
            $message = "Повторите через $dt sec";
        } else {
            if ($result['pass'] != $pass && ! $skippass) {
                $sql   = "UPDATE `users` SET `lastrefr` = :lastrefr WHERE `nick` = :nickname";
                $query = DB::link()->prepare($sql);
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
 * Init DB connection
 *
 * @return string error message
 */
function openDB()
{
    global $server, $user, $dbpass, $dbname;
    $msg = '';
    try {
        DB::link([
                'server'   => $server,
                'dbname'   => $dbname,
                'login'    => $user,
                'password' => $dbpass,
            ]);
    } catch (PDOException $e) {
        $msg = defined('DEBUG') ? $e->getMessage() : "База данных недоступна. Повторите через 5мин";
    }

    return $msg;
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
function SetData($login, $pass, $data)
{
    global $error, $Names, $Values;
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

    $error = openDB();
    if ($error != "") {
        return $error;
    }

    list($status, $result) = checkpass($login, $pass, '`names`,`vals`', true);    // сохраняет без пароля!
    if ($status != "") {
        return $status;
    }
    InitParam($result["names"], $result["vals"]);

    SetParam('gamedata', $data);

    $sqlUpd = 'UPDATE `users` SET `names` = :vnames, `vals` = :vals WHERE `nick` = :nickname';
    DB::link()->prepare($sqlUpd)->execute([
            ':vnames'   => $Names,
            ':vals'     => $Values,
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
function GetData($login, $pass)
{
    global $error, $NOT_SET;
    if (empty($login)) {
        return ["Логин не задан", null];
    }
    if (empty($pass)) {
        return ["Пароль не задан", null];
    }

    $error = openDB();
    if ($error != "") {
        return [$error, null];
    }

    list($message, $result) = checkpass($login, $pass, "`names`,`vals`");
    $data = [];
    if ($message == "") {
        InitParam($result["names"], $result["vals"]);
        $data = GetParam("gamedata");
        if ($data == $NOT_SET) {
            $message = "Данные не найдены";
        }
    }

    return [$message, $data];
}
