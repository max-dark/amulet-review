<?php
/**
 * @global login
 * @global loc
 * @global loc_i
 * @global loc_t
 * @global loc_tt
 * @global item
 * @global game
 * @global sid
 * @global PHP_SELF
 * @global char
 * @global $look
 */
$tlook = $look;
if (strpos($look, "_") !== false) {
    $look = substr($look, 0, strpos($look, "_"));
}
if (substr($look, 0, 6) != 'i.s.d.') {
    if (substr($look, 0, 5) == "i.rr.") {
        $item = explode("|", "руна|50");
    } else {
        if (file_exists("items/" . $look)) {
            $item = explode("|", (file_get_contents("items/" . $look)));
        } else {
            die("err: no items/" . $look);
        }
    }
}
function tsret($s)
{
    if ($s > 0) {
        return $s;
    } else {
        return 0;
    }
}

$stmp = "<p>";

if (substr($look, 0, 6) == 'i.s.d.') {
    $stmp .= "Труп, можно разделать ножом.";
} else {
    if ($look == 'i.flag') {
        $stmp .= "Флаг Лидерства";
    } else {
        if (substr($look, 0, 4) == 'i.i.') {
            $stmp .= $item[3] . "<br/>***<br/>Самоцвет, при инкрустировании добавляет магические свойства предметам.";
        } else {
            if (substr($look, 0, 10) == 'i.s.portal') {
                $stmp .= "Мерцающий овал магического портала, если вы дотронетесь до него (пункт меню Взять), то телепортируетесь в том место, в которое ведет этот портал.";
            } else {
                if (substr($look, 0, 5) == 'i.bc.') {
                    $stmp .= "Из цветов можно создавать букеты, для этого выберите цветок в рюкзаке и нажмите ссылку Использовать, потом выберите из списка либо букет, куда добавить, либо другой цветок, с кот. объединить для создания таким образом нового букета.";
                } else {
                    switch ($look) {
                        case 'i.magicbook':
                            $stmp .= "Книга магии, позволяет использовать заклинания которым вы научились, если хранится в рюкзаке.";
                        break;
                        case 'i.q.pervor':
                            $stmp .= "Перчатки вора позволяют незаметно подглядеть в любой рюкзак (только подглядеть! шанс своровать увиденное зависит от вашей ловкости и навыка Воровство, а также от навыка Осторожность у цели), но после использования исчезают. Сделано в гильдии воров в Ансалоне.";
                        break;
                        case 'i.q.pjpt':
                            $stmp .= "Перо жар-птицы защищает того, кто его носит. В случае смерти все предметы, которые были у персонажа, остаются у призрака (а не на трупе), при этом само перо исчезает.";
                        break;
                        case 'i.q.ssword':
                            $stmp .= "Наносит урон цели 0..90, после чего разлетается на осколки. Требует минимум силы 5 и ловкости 4.";
                        break;
                        case 'i.q.pdeath':
                            $stmp .= "Наносит урон магией всем в округе 0..90, после чего исчезает. Не имеет требований к параметрам персонажа.";
                        break;
                        case 'i.q.keykrep1':
                            $stmp .= "Ключ на столько начищен что в нем отчетливо видны ваши очертания, на нем имеется надпись на неезвестном вам языке.";
                        break;
                        case 'i.b.fire':
                            $stmp .= "Наносит урон всем в округе 1..24, не имеет требований к параметрам персонажа.";
                        break;
                        case 'i.b.jad':
                            $stmp .= "Наносит урон цели 1..28, не имеет требований к параметрам персонажа.";
                        break;
                        case 'i.b.holy':
                            $stmp .= "Наносит урон всем преступникам в округе 6..24, не имеет требований к параметрам персонажа.";
                        break;
                        case 'i.guildstone':
                            $stmp .= "Камень гильдии позволяет управлять собственным кланом.";
                        break;
                        case 'i.s.tree':
                            $stmp .= "С дерева можно рубить ветки с помощью любого топора, если у вас развит навык Лесоруб.";
                        break;
                        case 'i.s.rudnik':
                            $stmp .= "Здесь можно накопать руды, если у вас есть кирка и развит навык Рудокоп.";
                        break;
                        case 'i.s.res':
                            $stmp .= "Если человек-призрак дотронется до этого камня, то тут же воскреснет. Также восресить мертвого может лекарь или сильный маг заклинанием Воскрешение.";
                        break;
                        case 'i.s.fontan':
                            $stmp .= "В фонтане можно наполнить водой пустую бутылку";
                        break;
                        case 'i.set.koptilka':
                            $stmp .= "Переносная коптилка для рыбы, для копчения требует ветки и рыбу";
                        break;
                        case 'i.money':
                            $stmp .= "Золотые монеты, на них можно что-нибудь купить в магазинах.";
                        break;
                        default:
                            if (substr($look, 0, 4) == 'i.s.' && substr($look, 0, 8) != 'i.s.note' &&
                                substr($look, 0, 8) != 'i.s.book'
                            ) {
                                $stmp .= $item[1];
                            } else {
                                if (substr($look, 0, 5) == 'i.rr.') {
                                    $stmp .= "Руна для телепортации";
                                    if ($look == "i.rr.empty") {
                                        $stmp .= ", не помечена ни в одно место (это можно сделать заклинанием Пометить).";
                                    } else {
                                        loadloc(substr($look, 5));
                                        $tt = explode("|", $loc_tt[substr($look, 5)]["d"]);
                                        if (strpos($tt[0], "*") !== false) {
                                            $tt[0] = substr($tt[0], 0, strpos($tt[0], "*"));
                                        }
                                        $stmp .= " в " . $tt[0] . " (с помощью заклинания Возвращение)";
                                    }
                                } else {
                                    if (substr($tlook, 0, 5) == 'i.ms_') {
                                        $stmp .= "После использования этого свитка рядом появится <b>";
                                        if (strpos(substr($tlook, 5), "_") !== false) {
                                            include "f_translit.inc";
                                            $stmp .= trans(substr($tlook, strrpos($tlook, "_") + 1));
                                        } else {

                                            $npc = "npc/" . substr($tlook, 5);
                                            $npc = @include $npc;
                                            $tc  = explode("|", $npc["char"]);
                                            $stmp .= $tc[0];
                                        }
                                        $stmp .= "</b>, который будет защищать и следовать за призвавшим его. Не зависит от навыка Магия.";
                                    } else {
                                        if (substr($look, 0, 4) == 'i.a.') {
                                            $stmp .= "Броня: " . $item[2];
                                            if ($item[3]) {
                                                $str = explode(":", $item[3]);
                                                if ($str[0]) {
                                                    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=armor.str\">Требует силы</a>: " .
                                                             $str[0];
                                                }
                                                if ($str[1]) {
                                                    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=armor.dex\">Требует ловкости</a>: " .
                                                             $str[1];
                                                }
                                                if ($str[2]) {
                                                    $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=armor.int\">Требует интеллекта</a>: " .
                                                             $str[2];
                                                }
                                            }
                                            $stmp .= "<br/>Чтобы одеть или снять, выберите в меню \"Использовать\".";
                                        } else {
                                            if (substr($look, 0, 6) == 'i.note' || substr($look, 0, 8) == 'i.s.note') {
                                                $stmp .= $item[2];
                                            } else {
                                                if (substr($look, 0, 6) == 'i.book' ||
                                                    substr($look, 0, 8) == 'i.s.book'
                                                ) {
                                                    $book = explode("[page]", $item[2]);
                                                    if ( ! $page) {
                                                        $page = 0;
                                                    }
                                                    if ($page > count($book) - 1) {
                                                        $page = count($book) - 1;
                                                    }
                                                    $stmp .= $book[$page];
                                                    if (count($book) > 1) {
                                                        $stmp .= "<br/>Стр. " . ($page + 1) . " из " . count($book) .
                                                                 "<br/><a href=\"$PHP_SELF?sid=$sid&look=$look\">[Начало]</a>";
                                                    }
                                                    if ($page < count($book) - 1) {
                                                        $stmp .= "<a href=\"$PHP_SELF?sid=$sid&look=$look&page=" .
                                                                 ($page + 1) . "\">[След. стр.]</a>";
                                                    }
                                                } else {
                                                    if (substr($look, 0, 4) == 'i.m.') {
                                                        $stmp .= "Cвиток с заклинанием, исчезает после прочтения.<br/><a href=\"$PHP_SELF?sid=$sid&look=m." .
                                                                 substr($look, 4) . "\">Описание</a>";
                                                    } else {
                                                        if (substr($look, 0, 4) == 'i.r.') {
                                                            $stmp .= "Руна с заклинанием, после использования не исчезает.<br/><a href=\"$PHP_SELF?sid=$sid&look=m." .
                                                                     substr($look, 4) . "\">Описание</a>";
                                                        } else {
                                                            if (substr($look, 0, 4) == 'i.f.') {
                                                                $stmp .= "Пища восстанавливает здоровье и иногда ману<br/>Здоровье +" .
                                                                         intval($item[2]) . "<br/>Мана +" .
                                                                         intval($item[3]);
                                                            } else {
                                                                if (substr($look, 0, 4) == 'i.b.') {
                                                                    $stmp .= "Бутылка с зельем, которую можно бросить под ноги цели, пузырек разобьется и элексир попадет на кожу.";
                                                                } else {
                                                                    if (substr($look, 0, 4) == 'i.w.') {
                                                                        if (substr($look, 0, 6) == 'i.w.r.') {
                                                                            $stmp .= "Стрелковое/метательное ";
                                                                        } else {
                                                                            $stmp .= "Холодное ";
                                                                        }
                                                                        $stmp .= "оружие";
                                                                        if (substr($look, 0, 6) == 'i.w.k.') {
                                                                            $stmp .= ", подходит для разделки трупов.";
                                                                        }
                                                                        if (substr($look, 0, 6) == 'i.w.t.') {
                                                                            $stmp .= ", подходит для рубки деревьев.";
                                                                        }
                                                                        $stmp .= "<br/>Урон: " . $item[2] . "-" .
                                                                                 $item[3];
                                                                        if ($item[4]) {
                                                                            $str = explode(":", $item[4]);
                                                                            if ($str[0]) {
                                                                                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=weapon.str\">Требует силы</a>: " .
                                                                                         $str[0];
                                                                            }
                                                                            if ($str[1]) {
                                                                                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=weapon.dex\">Требует ловкости</a>: " .
                                                                                         $str[1];
                                                                            }
                                                                            if ($str[2]) {
                                                                                $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=weapon.int\">Требует интеллекта</a>: " .
                                                                                         $str[2];
                                                                            }
                                                                        }
                                                                        $stmp .= "<br/><a href=\"$PHP_SELF?sid=$sid&look=weapon.speed\">Скорость</a>: " .
                                                                                 intval($item[5]);
                                                                        if ($item[7]) {
                                                                            if (file_exists("items/" . $item[7])) {
                                                                                $tmp = explode("|",
                                                                                    (file_get_contents("items/" .
                                                                                                       $item[7])));
                                                                            } else {
                                                                                die("err: no items/" . $item[7]);
                                                                            }
                                                                            $stmp .= "<br/>Использует: " . $tmp[0];
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        break;
                    }

                }
            }
        }
    }
}
if (strpos($tlook, "_") !== false && substr($look, 0, 4) != 'i.s.' && substr($look, 0, 4) != 'i.ms') {
    $tpos      = strpos($tlook, "_");
    $createdby = substr($tlook, $tpos + 1, strrpos($tlook, "_") - $tpos - 1);
    $stmp .= "<br/>Сделано мастером: <b>" . $createdby . "</b>";
}
if (strpos($tlook, "..") !== false) {
    $stmp .= "<br/>Инкрустированы самоцветы:";
    $xF = preg_match_all("/\.\.(\w+)/", $tlook, $regF);
    for ($i = 0; $i < $xF; $i++) {
        $ti = explode("|", (file_get_contents("items/i.i." . $regF[1][$i])));
        $stmp .= "<br/><b>" . $ti[0] . "</b>:<br/>" . $ti[3];
    }
}
if (substr($tlook, 0, 5) == "i.bc_") {
    $stmp .= "<br/>Букет состоит из цветов:";
    $xF = preg_match_all("/(\w)(\d+)/", substr($tlook, strrpos($tlook, "_") + 1), $regF);
    for ($i = 0; $i < $xF; $i++) {
        $ti = explode("|", (file_get_contents("items/i.bc." . $regF[1][$i])));
        $stmp .= "<br/>" . $ti[0];
        if ($regF[2][$i] > 1) {
            $stmp .= " (" . $regF[2][$i] . ")";
        }
    }
}

if ($stmp == "<p>") {
    $stmp .= "Описание отсутствует";
}
msg($stmp, $item[0]);