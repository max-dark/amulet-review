<?php
/**
 * @copyright Copyright (C) 2017. Max Dark maxim.dark@gmail.com
 * @license   MIT; see LICENSE.txt
 */

namespace MaxDark\Amulet\OldCode;


class WordFilter
{
    /**
     * @var WordFilter
     */
    private static $instance = null;

    /**
     * @var string[]
     */
    private $gw;

    /**
     * @var string[]
     */
    private $bw;

    /**
     * @var string[]
     */
    private $eng2ru;

    /**
     * @var string[]
     */
    private $change2;

    /**
     * Возвращает первое плохое слово или пустую строку
     *
     * @param string $s
     * @return string
     */
    public static function getAbuse($s)
    {
        return self::getInstance()->getBadWord($s);
    }

    /**
     * @return WordFilter
     */
    public static function getInstance()
    {
        if (is_null(self::$instance))
        {
            self::$instance = new self();
        }
        return self::$instance;
    }


    /**
     * Возвращает первое плохое слово или пустую строку
     *
     * @param string $s
     * @return string
     */
    public function getBadWord($s)
    {
        $gw = $this->gw;
        $bw = $this->bw;

        $s = strtr($s, $this->change2);
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
        $s = strtr($s, $this->eng2ru);
        // Удаление правильных слов
        for ($i = 0; $i < count($gw); $i++) {
            while (false !== ($pos = mb_strpos($s, $gw[$i]))) {
                $s     = mb_substr($s, 0, $pos) . mb_substr($s, $pos + mb_strlen($gw[$i]));
                $s_out = mb_substr($s_out, 0, $pos) . mb_substr($s_out, $pos + strlen($gw[$i]));
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

    private function __construct()
    {
        $this->eng2ru = array_combine(
            UTFTool::chars_of('a@b6cvgde*z3ijklmno0prs$tufhx+c4wy&9'),
            UTFTool::chars_of('ааббцвгдежззийклмноопрсстуфхххцчшуяя')
        );
        $this->change2 = [
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
        $this->gw = [
            "страху",
            "влюблять",
            "скорбл",
            "авлят",
            "гнезд",
            "нездр",
            "нездо",
            "облада",
            "владе",
            "плох",
            "епс",
            "миди",
            "яблок",
            "бладе",
        ];
        $this->bw = [
            "><у",
            ")(у",
            " хуй",
            " хуи",
            " хуё",
            " хуе",
            " хий",
            " хии",
            " хиё",
            " хие",
            " хуя",
            " хия",

            " хуу",
            " хиу",

            "охуи",
            "охии",
            "охие",
            "охуу",
            "охиу",
            "охую",

            "ахуи",
            "ахии",
            "ахие",
            "ахуу",
            "ахиу",

            "пизд",
            "писд",
            "ризд",
            "рисд",
            "пузд",
            "пусд",
            "рузд",
            "русд",
            "низд",
            "нисд",
            "нузд",
            "нусд",

            "пызд",
            "пысд",
            "рызд",
            "рысд",
            "нызд",
            "нысд",

            "пицд",
            "рицд",
            "ницд",
            "нуцд",
            "пуцд",

            "пезд",
            "песд",
            "резд",
            "ресд",
            "пецд",
            "рецд",
            "нецд",
            "незд",
            "несд",

            "бля",
            "бляд",
            "блад",
            "блят",
            "блать",
            "блйад",
            "блйат",
            "блиад",
            "блиат",
            "вляд",
            "влать",
            "влйад",
            "влйат",
            "влиад",
            "влиат",

            " ёб",
            " ёп",
            " еб",
            " йоб",
            " иоб",

            "ъеб",

            "аёб",
            "аёп",
            "аеб",
            "айоб",
            "аиоб",

            "оёб",
            "оеб",

            "уёб",
            "уёп",
            "уеб",
            "уйоб",
            "уиоб",

            "иеб",
            "ыеб",

            "муди",
            "муда",
            "миди",
            "мида",

            "член",
            "члеп",
            "пидор",
            "пидар",
            "педик",
            "нидор",
            "нидар",
            "недик",
            "нидар",
            "пидор",
            "пидар",

            " жоп",
            " жон",
            " зхоп",
            " зхон",

            "драчи",
            "драцхи",
            "дпачи",
            "дпацхи",
        ];
    }
}