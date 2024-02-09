<?php

use MaxDark\Amulet\OldCode\DBOperations;
use MaxDark\Amulet\OldCode\PageType;

$nn = Request('nn');
if (!preg_match('|^[-0-9a-z_]+$|i', $nn)) {
    msg("Неверный синтаксис в логине<br /><button class=\"btn btn-outline-secondary m-1\" name=\"b2\" type=\"accept\" label=\"Назад\" onclick=\"history.back()\"><prev>Назад</prev></button>", 'Ошибка', 0, PageType::GUEST);
}

if (DBOperations::nicknameExists($nn)) {
    msg('Этот логин уже занят.<br /><button class=\"btn btn-outline-secondary m-1\" name=\"b2\" type=\"accept\" label=\"Назад\" onclick=\"history.back()\"><prev>Назад</prev></button>', 'Ошибка', 0, PageType::GUEST);
}
$stmp = <<<PAGE
</p><p align="center">
<form action="?site=reg2" method="post">
<b>Логин:</b>
<br /><b>$nn</b><br />--------
<br /><b>Пароль</b>:
<br />(a-Z,0-9)
<br /><input class="form-control" name="pass" emptyok="true" type="password" value="" maxlength="15" size="15"/>
<br /><b>Ваш Email адрес:</b>
<br /><input class="form-control" name="email" emptyok="true" type="text" maxlength="25" size="25"/> 
<br /><b>Пол персонажа</b>
<br /><select class="form-control" name="sex" value="m">
<option value="m">Муж.</option>
<option value="f">Жен.</option>
</select>
<br /><b>Возраст</b>
<br /><input class="form-control" name="age" format="*N" maxlength="2"/>
<input type="hidden" name="nn" value="$nn"/>
<br /><button type="submit" class="btn btn-primary m-1">Продолжить</button>
</form>
<br /><button class="btn btn-outline-secondary m-1" name="b2" type="accept" label="Назад" onclick="history.back()"><prev>Назад</prev></button>
PAGE;
msg($stmp, "Регистрация", 0, PageType::GUEST);
