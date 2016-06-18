<?php
$letters = [];
function alt($s)
{
    global $ttans, $letters;
    if ($ttans) {
        if(empty($letters)) {
            // 'lazy' load
            $letters[0] = [
                "jo" => "ё", "je" => "э", "ju" => "ю", "ja" => "я",
                "zh" => "ж", "ch" => "ч", "sh" => "ш", "JO" => "Ё",
                "JE" => "Э", "JU" => "Ю", "JA" => "Я",
                "ZH" => "Ж", "CH" => "Ч", "SH" => "Ш"
            ];
            $letters[1] = array_combine(
                chars_of("wxq'ertyuiopasdfghjklzcvbnmWQXERTYUIOPASDFGHJKLZCVBNM"),
                chars_of('щъььертыуиопасдфгхйклзцвбнмЩЬЪЕРТЫУИОПАСДФГХЙКЛЗЦВБНМ')
            );
        }
        $s = strtr($s, $letters[0]);
        $s = preg_replace('/(j|J)_(a|A|u|U|o|O|e|E)/u', "\\1\\2", $s);
        $s = preg_replace('/(z|Z|c|C|s|S)_(h|H)/u', "\\1\\2", $s);
        $s = strtr($s, $letters[1]);
    }
    $ttans = !$ttans;
    return $s;
}

function trans($s)
{
    global $ttans;
    $ttans = true;
    return preg_replace_callback(
        '/(.*)##??/Uu',
        function (array $m){
            return alt($m[1]);
        },
        "$s#"
    );
}
