<?php
require_once('modules/database.php');

$NOT_SET = "NOT_SET";
require("antimat.inc"); // Антимат-фильтр ( чтобы ники матерные не регистрировали)

function InitParam($N, $V) {
    global $Names, $Values;
    $Names = $N;
    $Values = $V;
}

function GetParam($Name) {
    global $Names, $Values, $NOT_SET;

    $Name = strtolower($Name);
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

function SetParam($Name, $Value) {
    global $Names, $Values, $NOT_SET;
    $Nlist = explode(":", $Names);
    $Name = strtolower($Name);
    $Value = addslashes(str_replace(":", "!~!", $Value));
    for ($i = 0; $i < count($Nlist); $i++) {
        if ($Nlist[$i] == $Name) {
            break;
        }
    }

    if ($i == count($Nlist) and ($Value != $NOT_SET)) { // Добавляем имя и значение
        $Names .= ":$Name";
        $Values .= ":$Value";
    }
    else {
        $Vlist = explode(":", $Values);
        $Vlist[$i] = $Value;
        $Values = implode(":", $Vlist);
        if ($Value == $NOT_SET) { // Удаление имени и значения
            $Nlist[$i] = $NOT_SET;
            $Names = implode(":", $Nlist);
            $Names = str_replace(":$NOT_SET", "", $Names);
            $Values = str_replace(":$NOT_SET", "", $Values);
        }
    }
}

/**
 * Получение массива параметров для пользователя $nick
 *
 * @param string $nick
 * @param string $pass
 * @param string $fields
 * @param bool   $skippass
 * @return array [message, result]
 */
function checkpass($nick, $pass, $fields, $skippass = false) {
    global $PassDelay;
    if ($fields == "") {
        $fields = "`pass`,`lastrefr`";
    }
    else {
        if ($fields !== "*") {
            $fields .= ",`pass`,`lastrefr`";
        }
    }

    $now = time();
    $result = [];
    $message = '';
    $sql = "select $fields from `users` where `nick` = :nickname";
    $query = DB::link()->prepare($sql);
    $query->execute(
        [
            ':nickname' => $nick
        ]
    );
    if ($query->rowCount() != 1) {
        $message = "Логин не найден";
    }
    else {
        $result = $query->fetch(PDO::FETCH_ASSOC);
        $dt = $PassDelay - $now + $result['lastrefr'];
        if ($dt > 0) {
            $message = "Повторите через $dt sec";
        }
        else {
            if ($result['pass'] != $pass && !$skippass) {
                $sql = "UPDATE `users` SET `lastrefr` = :lastrefr WHERE `nick` = :nickname";
                $query = DB::link()->prepare($sql);
                $query->execute(
                    [
                        ':lastrefr' => $now,
                        ':nickname' => $nick
                    ]
                );
                $message = "Неверный пароль";
            }
        }
    }

    return [$message, $result];
}

/**
 * @return string error message
 */
function openDB() {
    global $server, $user, $dbpass, $dbname;
    $msg = '';
    try {
        DB::link(
            [
                'server'   => $server,
                'dbname'   => $dbname,
                'login'    => $user,
                'password' => $dbpass,
            ]
        );
    }
    catch (PDOException $e) {
        $msg = defined('DEBUG') ? $e->getMessage() : "База данных недоступна. Повторите через 5мин";
    }

    return $msg;
}

/**
 * Возвращает пустую строку в случае успеха или сообщение об ошибке.
 *
 * @param string $login
 * @param string $pass
 * @param mixed  $data
 * @return string error message
 */
function SetData($login, $pass, $data) {
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

    list($ok, $result) = checkpass($login, $pass, '`names`,`vals`', true);    // сохраняет без пароля!
    if ($ok != "") {
        return $ok;
    }
    InitParam($result["names"], $result["vals"]);

    SetParam('gamedata', $data);

    $sqlUpd = 'UPDATE `users` SET `names` = :vnames, `vals` = :vals WHERE `nick` = :nickname';
    DB::link()->prepare($sqlUpd)->execute(
        [
            ':vnames'   => $Names,
            ':vals'     => $Values,
            ':nickname' => $login
        ]
    );

    return "";
}

/**
 * Возвращает пустую строку в случае успеха (данные возвращаются в $data) или сообщение об ошибке.
 *
 * @param string $login
 * @param string $pass
 * @return string
 */
function GetData($login, $pass) {
    global $error, $NOT_SET;
    if (empty($login)) {
        return "Логин не задан";
    }
    if (empty($pass)) {
        return "Пароль не задан";
    }

    $error = openDB();
    if ($error != "") {
        return $error;
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

/**
 * Регистрация нового пользователя (oldPass = "") или смена пароля.
 * Возвращает пустую строку в случае успеха или сообщение об ошибке.
 *
 * @param string $login
 * @param string $oldPass
 * @param string $newPass
 * @return string
 */
function SetUser($login, $oldPass, $newPass) {
    global $RegStatus, $DefRefrInt, $DefMessLim, $CommonMode;

    if (empty($login)) {
        return "Логин не задан";
    }
    if (empty($newPass)) {
        return "Пароль не задан";
    }
    //if (!ValidNN($login)) return "Неверный синтаксис в логине";
    //if (!ValidPass($newpass)) return "Неверный синтаксис в пароле";

    $login = substr($login, 0, 10);
    $newPass = substr($newPass, 0, 10);

    $BadWord = GetBadWord($login);
    if ($BadWord != "") {
        return "Логин содержит запрещённое слово";
    }

    $error = openDB();
    if ($error != "") {
        return $error;
    }

    if ($oldPass != "") {
        $ok = checkpass($login, $oldPass, "")[0];
        if ($ok != "") {
            return $ok;
        }
        $sql = 'UPDATE `users` SET `pass` = :newpass WHERE `nick` = :login AND `pass` = :oldpass';
        DB::link()->prepare($sql)->execute(
            [
                ':newpass' => $newPass,
                ':login'   => $login,
                ':oldpass' => $oldPass
            ]
        );
    }
    else {
        $sql = "SELECT * FROM `users` WHERE `nick` = :nickname";
        $query = DB::link()->prepare($sql);
        $query->execute(
            [
                ':nickname' => $login
            ]
        );
        $Count = $query->rowCount();
        if ($Count != 0) {
            return "Такой логин уже зарегистирован";
        }
        $now = time();
        $sql = 'INSERT INTO `users`' . ' (`status`,`sent`,`regtime`,`refrint`,`messlim`,`mode`,`nick`,`pass`)' .
               ' VALUES (?, ?, ?, ?, ?, ?, ?, ?)';
        DB::link()->prepare($sql)->execute(
            [
                $RegStatus,
                '0',
                $now,
                $DefRefrInt,
                $DefMessLim,
                $CommonMode,
                $login,
                $newPass
            ]
        );
    }

    return "";
}
