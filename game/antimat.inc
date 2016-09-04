<?php

// Массив хороших слов - только кириллица
$gw[] = "страху";
$gw[] = "влюблять";
$gw[] = "скорбл";
$gw[] = "авлят";
$gw[] = "гнезд";
$gw[] = "нездр";
$gw[] = "нездо";
$gw[] = "облада";
$gw[] = "владе";
$gw[] = "плох";
$gw[] = "епс";
$gw[] = "миди";
$gw[] = "яблок";
$gw[] = "бладе";

// Массив плохих слов - только кириллица

$bw[] = "><у";
$bw[] = ")(у";
$bw[] = " хуй";
$bw[] = " хуи";
$bw[] = " хуё";
$bw[] = " хуе";
$bw[] = " хий";
$bw[] = " хии";
$bw[] = " хиё";
$bw[] = " хие";
$bw[] = " хуя";
$bw[] = " хия";

$bw[] = " хуу";
$bw[] = " хиу";

$bw[] = "охуи";
$bw[] = "охии";
$bw[] = "охие";
$bw[] = "охуу";
$bw[] = "охиу";
$bw[] = "охую";

$bw[] = "ахуи";
$bw[] = "ахии";
$bw[] = "ахие";
$bw[] = "ахуу";
$bw[] = "ахиу";

$bw[] = "пизд";
$bw[] = "писд";
$bw[] = "ризд";
$bw[] = "рисд";
$bw[] = "пузд";
$bw[] = "пусд";
$bw[] = "рузд";
$bw[] = "русд";
$bw[] = "низд";
$bw[] = "нисд";
$bw[] = "нузд";
$bw[] = "нусд";

$bw[] = "пызд";
$bw[] = "пысд";
$bw[] = "рызд";
$bw[] = "рысд";
$bw[] = "нызд";
$bw[] = "нысд";

$bw[] = "пицд";
$bw[] = "рицд";
$bw[] = "ницд";
$bw[] = "нуцд";
$bw[] = "пуцд";

$bw[] = "пезд";
$bw[] = "песд";
$bw[] = "резд";
$bw[] = "ресд";
$bw[] = "пецд";
$bw[] = "рецд";
$bw[] = "нецд";
$bw[] = "незд";
$bw[] = "несд";

$bw[] = "бля";
$bw[] = "бляд";
$bw[] = "блад";
$bw[] = "блят";
$bw[] = "блать";
$bw[] = "блйад";
$bw[] = "блйат";
$bw[] = "блиад";
$bw[] = "блиат";
$bw[] = "вляд";
// $bw[]="влят";
$bw[] = "влать";
$bw[] = "влйад";
$bw[] = "влйат";
$bw[] = "влиад";
$bw[] = "влиат";

$bw[] = " ёб";
$bw[] = " ёп";
$bw[] = " еб";
$bw[] = " йоб";
$bw[] = " иоб";

$bw[] = "ъеб";

$bw[] = "аёб";
$bw[] = "аёп";
$bw[] = "аеб";
$bw[] = "айоб";
$bw[] = "аиоб";

$bw[] = "оёб";
$bw[] = "оеб";

$bw[] = "уёб";
$bw[] = "уёп";
$bw[] = "уеб";
$bw[] = "уйоб";
$bw[] = "уиоб";

$bw[] = "иеб";
$bw[] = "ыеб";

$bw[] = "муди";
$bw[] = "муда";
$bw[] = "миди";
$bw[] = "мида";

$bw[] = "член";
$bw[] = "члеп";
$bw[] = "пидор";
$bw[] = "пидар";
$bw[] = "педик";
$bw[] = "нидор";
$bw[] = "нидар";
$bw[] = "недик";
$bw[] = "нидар";
$bw[] = "пидор";
$bw[] = "пидар";

$bw[] = " жоп";
$bw[] = " жон";
$bw[] = " зхоп";
$bw[] = " зхон";

$bw[] = "драчи";
$bw[] = "драцхи";
$bw[] = "дпачи";
$bw[] = "дпацхи";

$change2 = [
    'Ä' => 'a',
    'Å' => 'a',
    'Æ' => 'a',
    'à' => 'a',
    'ä' => 'a',
    'å' => 'a',
    'Ö' => 'o',
    'ö' => 'o',
    'ø' => 'o',
    'ò' => 'o',
    'è' => 'e',
    'é' => 'e',
    'É' => 'e',
    'Ü' => 'u',
    'ü' => 'u',
    'ù' => 'u',
    'Ø' => 'i',
    'Ñ' => 'i',
    'Π' => 'p',
    'π' => 'p',
    'Δ' => 'd',
    'Λ' => 'l',
    'ß' => 'b'
];

/**
 * split utf-8 string to array of chars
 *
 * @param string $str
 *
 * @return array
 */
function chars_of($str)
{
    return preg_split('/(?<!^)(?!$)/u', $str);
}

/**
 * utf-8 analog for strtr function
 *
 * @param string $str
 * @param string $from
 * @param string $to
 *
 * @return string
 */
function mb_strtr($str, $from, $to)
{
    return strtr($str, array_combine(chars_of($from), chars_of($to)));
}

$en2ru = [];

function GetBadWord($s) // Возвращает первое плохое слово или пустую строку

{
    global $gw, $bw, $change2, $en2ru;

    if (empty($en2ru)) {
        $en2ru = array_combine(chars_of('a@b6cvgde*z3ijklmno0prs$tufhx+c4wy&9'),
            chars_of('ааббцвгдежззийклмноопрсстуфхххцчшуяя'));
    }
    $s = strtr($s, $change2);
    $s = str_replace("&amp;", "я", $s);
    $s = str_replace("&apos;", "ь", $s);
    $s = str_replace("&lt;", "<", $s);
    $s = str_replace("&gt;", ">", $s);

    $s = str_replace("_", "", $s);
    $s = str_replace("-", "", $s);
    $s = str_replace("+", "", $s);
    $s = str_replace("*", "", $s);
    $s = str_replace("^", "", $s);
    $s = str_replace("~", "", $s);
    $s = str_replace("'", "", $s);
    $s = str_replace("`", "", $s);
    $s = str_replace(":", "", $s);
    $s = str_replace("=", "", $s);

    // Пробелы убираем интеллектуально
    $bspos = 0;
    $s     = " $s ";
    for ($i = 1; $i < strlen($s); $i++) {
        $ch = $s[$i];
        if (($ch == " ") or ($ch == ",") or ($ch == ".")) {
            if (($i - $bspos) > 3) {
                $s[$bspos] = " ";
            } else {
                $s[$i] = "-";
            }
            $bspos = $i;
        }
    }
    $s     = str_replace("-", "", trim($s));
    $s_out = $s;

    // Преобразуем в нижний регистр
    $s = mb_strtolower($s);
    // Преобразуем возможные замены в кириллицу
    $s = strtr($s, $en2ru);
    // Удаление правильных слов
    for ($i = 0; $i < count($gw); $i++) {
        $pos = mb_strpos($s, $gw[$i]);
        while ($pos !== false) {
            $s     = mb_substr($s, 0, $pos) . mb_substr($s, $pos + mb_strlen($gw[$i]));
            $s_out = mb_substr($s_out, 0, $pos) . mb_substr($s_out, $pos + strlen($gw[$i]));
            $pos   = mb_strpos($s, $gw[$i]);
        }
    }

    $s     = " $s";
    $s_out = " $s_out";

    for ($i = 0; $i < count($bw); $i++) {
        $pos = mb_strpos($s, $bw[$i]);
        if ($pos !== false) {
            $pos = $pos - 5;
            if ($pos < 0) {
                $pos = 0;
            }
            $s = mb_substr($s_out, $pos, mb_strlen($bw[$i]) + 10);

            return $s;
        }
    }

    return "";
}