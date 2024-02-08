<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global to
 * @global desc
 * @global exits
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 */
// сылка воскрешение для админа
if ($login == $g_admin || $gm == $gm_id) {
    switch ($adm) {
        case 'smp':
            msg(
                "<p><input name=\"inp\" emptyok=\"true\"/>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=$(inp)\">teleport (loc)</a> <a href=\"#tele\"> [select]</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=mnn&inp=$(inp)\">money (count)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=filter&inp=$(inp)\">additem (filter)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=gdn\">god (" . intval($loc_i[$loc][$login]["god"]) . ")</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=getibm\">[items,bank,magic]</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=view&inp=$(inp)\">view (field|login|loc|t)</a>
	<br/><input name=\"val\" emptyok=\"true\"/>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=adni&inp=$(inp)&val=$(val)&new=$(new)\">add (item,count,[bank|loc])</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=del&inp=$(inp)&val=$(val)&new=$(new)\">delete (item,count|all,[bank])</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=gtp&inp=$(inp)&val=$(val)\">get (field,index)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=unset&inp=$(inp)&val=$(val)\">unset (field,index)</a>
	<br/><input name=\"new\" emptyok=\"true\"/>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=stn&inp=$(inp)&val=$(val)&new=$(new)\">set (field,index,val)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=skillinfo\">[skill info]</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=locmail&inp=$(inp)\">mail_loc(email)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=reploc&inp=$(inp)\">load_bank(npc)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=resp&inp=$(inp)&val=$(val)&new=$(new)\">timer (id,min,max)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=guard&inp=$(inp)\">guard (1|0)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=setgclan&inp=$(inp)&val=$(val)\">setgclan (clan,glava)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=ban&inp=$(inp)\">CLEAR(skills=1)</a>
	<br/>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=stop&inp=$(inp)\">stop 5 min(msg)</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=play\">play (msg: " . intval(isset($game["msg"])) . ")</a>
	<br/><a href=\"$PHP_SELF?sid=$sid&adm=blank\">blank world</a>
	\n</p>\n</div>\n<div id=\"tele\" title=\"Телепорт\">\n<p>\n<a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=_begin\">Начало</a><br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=x1092x474\">Банк</a><br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=x1284x393\">Кладбище</a><br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=x902x254\">Замок</a><br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=x1486x247\">Пещера</a><br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=x2169x273\">Ансалон</a><br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=x1778x493\">Крепость</a><br/><a href=\"$PHP_SELF?sid=$sid&adm=tln&inp=x2247x786\">Подземелье</a>",
                $loc,
                0
            );
            break;
        case 'rsn':
            $char[1] = $char[2];
            $char[3] = $char[4];
            $char[8] = 0;
            $char[9] = 0;
            $loc_i[$loc][$login]["char"] = implode("|", $char);
            $char = explode("|", $loc_i[$loc][$login]["char"]);
            $loc_i[$loc][$login]["magic"] = preg_replace('/:(\d+)/', ":0", $loc_i[$loc][$login]["magic"]);
            $loc_i[$loc][$login]["priem"] = preg_replace('/:(\d+)/', ":0", $loc_i[$loc][$login]["priem"]);
            break;
        case 'tln':
            if ($inp) {
                manageNPC($login, $loc, strtolower($inp));
                $loc_c = explode("|", $loc_tt[$loc]["d"]);
            }
            break;
        case 'mnn':
            if ($inp) {
                manageItems($loc, "", $login, "i.money", $inp);
            }
            break;
        case 'gtp':
            $field = explode("|", $loc_i[$loc][$login][strtolower($inp)]);
            msg($field[strtolower($val)]);
            break;
        case 'unset':
            if ($inp && $val) {
                $field = explode("|", $loc_i[$loc][$login][strtolower($inp)]);
                unset($field[strtolower($val)]);
                $loc_i[$loc][$login][strtolower($inp)] = implode("|", $field);
                if ($val == "items") {
                    calcparam($loc, $login);
                }
            } else {
                if ($to) {
                    if (isset($loc_i[$loc][$to])) {
                        unset($loc_i[$loc][$to]);
                        msg("Объект удален");
                    } else {
                        msg("Нет объекта");
                    }
                }
            }
            break;
        case 'stn':
            $field = explode("|", $loc_i[$loc][$login][strtolower($inp)]);
            $field[strtolower($val)] = $new;
            $loc_i[$loc][$login][strtolower($inp)] = implode("|", $field);
            calcparam($loc, $login);
            break;
        case 'adni':
            if ($inp && $val) {
                if ($new == "loc") {
                    $to = "loc";
                } else {
                    $to = $login;
                }
                if ($new != "bank") {
                    $new = "items";
                }
                manageItems($loc, "", $to, $inp, $val . "items", $new);
            }
            break;
        case 'del':
            if ($inp && $val) {
                if ($new != "bank") {
                    $new = "items";
                }
                if ($val == "all") {
                    $loc_i[$loc][$login][$new] = "";
                } else {
                    manageItems($loc, $login, "", strtolower($inp), $val, "items", $new);
                }
                calcparam($loc, $login);
            }
            break;
        case 'gdn':
            if ($loc_i[$loc][$login]["god"]) {
                unset($loc_i[$loc][$login]["god"]);
            } else {
                $loc_i[$loc][$login]["god"] = 1;
            }
            break;
        case 'blank':
            $f_all = 1;
            include "f_online.inc";
            include "f_blank.inc";
            msg("Мир обновлен, все игроки переведены в оффлайн.");
            break;
        case 'locsave':
            if ($desc) {
                $file = fopen("loc_f/" . $loc, "w");
                fputs($file, str_replace('\"', '"', $desc));
                fclose($file);
            }
            if ($exits) {
                $dloc = str_replace("\r\n", "|", $exits);
                if (substr($dloc, strlen($dloc) - 1) == "|") {
                    $dloc = substr($dloc, 0, strlen($dloc) - 1);
                }
                $loc_tt[$loc]["d"] = $dloc;
                $loc_tt[$loc]["save"] = 1;
                $loc_c = explode("|", $loc_tt[$loc]["d"]);
            }
            break;
        case 'view':
            echo $inp . ":<br/>";
            switch ($inp) {
                case "loc":
                    $msg = serialize($loc_i[$loc]);
                    break;
                case "t":
                    $msg = serialize($loc_tt[$loc]["t"]);
                    break;
                case "login":
                    $msg = serialize($loc_i[$loc][$login]);
                    break;
                default:
                    $msg = $loc_i[$loc][$login][strtolower($inp)];
                    break;
            }
            msg("$msg<br/><a href=\"$PHP_SELF?sid=$sid\">В игру</a>");
            break;
        case 'ban':
            $loc_i[$loc][$login]["items"] = "";
            $loc_i[$loc][$login]["bank"] = "";
            if ($inp == 1) {
                $loc_i[$loc][$login]["skills"] = "1|1|1|0|2|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0";
                $loc_i[$loc][$login]["magic"] = "";
            }
            calcparam($loc, $login);
            msg("Персонаж обнулен");
            break;
        case "locmail":
            addjournal($loc, $login, "Не удалось отправить письмо на " . $inp);
            break;
        case 'getibm':
            msg("items: " . $loc_i[$loc][$login]["items"] . "<br/>bank: " . $loc_i[$loc][$login]["bank"] .
                "<br/>magic: " . $loc_i[$loc][$login]["magic"] . "<br/>skills: " . $loc_i[$loc][$login]["skills"]);
            break;
        case 'resp':
            manageTimers($loc, 0, $val, $inp . "|$val:$new");
            addjournal($loc, $login, "Таймер установлен");
            break;
        case 'skillinfo':
            msg("skills = 0:str|1:dex|2:int|3:level|4:points|5:meditation|6:steal|7:animaltaming|8:hand|9:coldweapon|10:ranged|11:parring|12:uklon|13:magic|14:magic_resist|15:magic_uklon|16:regeneration|17:hiding|18:look|19:steallook|20:animallore|21:spirit|22:healing|23:alchemy|24:mine|25:smith|26:lumb|27:bow|28:stone|29:fish|30:food");
            break;
        case 'stop':
            $game["stop"] = time() + 180;
            if ($inp) {
                $game["msg"] = 'Игра обновляется. зайдите позже';
            }
            msg("Остановка через 3 минут");
            break;
        case 'play':
            unset($game["journal"]);
            unset($game["info"]);
            unset($game["stop"]);
            unset($game["msg"]);
            msg("Игра запущена");
            break;
        case 'guard':
            $d = explode("|", $loc_tt[$loc]["d"]);
            if ($inp) {
                $d[1] = 1;
            } else {
                $d[1] = 0;
            }
            $loc_tt[$loc]["d"] = implode("|", $d);
            $loc_tt[$loc]["save"] = 1;
            break;
        case 'filter':
            if ($inp) {
                $stmp = "<p>Добавить предмет:";
                $dh = opendir("items/");
                while (($fname = readdir($dh)) !== false) {
                    if ($fname != "." && $fname != ".." && substr($fname, 0, strlen($inp)) == $inp) {
                        $item = findItemByBaseId($fname);
                        $stmp .= "\n<br/><a href=\"$PHP_SELF?sid=$sid&adm=adni&inp=$fname&val=1\">" . $item[0] . "</a>";
                        $count++;
                    }
                }
                closedir($dh);
                msg($stmp, "Предметы", 0);
            }
            break;
        case 'setgclan':
            if ($inp && $val) {
                if (file_exists("data/clans/" . $inp)) {
                    $tmp = unserialize((file_get_contents("data/clans/" . $inp)));
                } else {
                    msg("нет клана");
                }
                if (substr($val, 0, 2) != "u.") {
                    $val = "u." . $val;
                }
                $tmp["g"] = [$val => ""];
                $file = fopen("data/clans/" . $inp, "w");
                if ($file !== false) {
                    fputs($file, serialize($tmp));
                    fclose($file);
                }
                msg("Глава клана установлен");
            }
            break;
        case 'reploc':
            if ($inp) {
                if (substr($inp, 0, 2) !== "n.") {
                    msg("npc должен начинаться на n. и быть в локации в вами");
                }
                if (!isset($loc_i[$loc][$inp])) {
                    msg("рядом с вами нет npc с id " . $inp . " (регистр учитывается)");
                }
                $npc = loadNpcById($inp);
                if (!is_array($npc)) {
                    msg("ошибка загрузки npc/" . $inp);
                }
                $loc_i[$loc][$inp]["bank"] = $npc["bank"];
                msg("Банк у $inp загружен из файла npc/$inp");
            }
            break;
    }

    //if($inp==1) $inp=$loc; if(!file_exists("loc_t/".$inp)) msg("Нет локации $inp"); copy("loc_t/".$inp,"loc_i/".$inp); msg("Локация восстановлена");}
}
