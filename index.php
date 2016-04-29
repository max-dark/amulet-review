<?php

header ("Expires: Thu, 01 Jan 1970 00:00:01 GMT");
header ("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header ("Cache-Control: no-cache, no-store, must-revalidate, max-age=0");
header ("Pragma: no-cache");
header("Content-type:text/vnd.wap.wml;charset=utf-8");

echo "<?xml version=\"1.0\" ?>\n";?>
<!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML 1.1//EN" "http://www.wapforum.org/DTD/wml_1.1.xml"> 
<?php setlocale (LC_CTYPE, 'ru_RU.CP1251'); 
function win2unicode ( $s ) { if ( (ord($s)>=192) & (ord($s)<=255) ) $hexvalue=dechex(ord($s)+848); if ($s=="Ё") $hexvalue="401"; if ($s=="ё") $hexvalue="451"; return("&#x0".$hexvalue.";");} 
function translate($s) {return(preg_replace("/[А-яЁё]/e","win2unicode('\\0')",$s));} 
ob_start("translate");
?>
<wml>
<card title="Амулет Дракона" ontimer="1/g.php">
<timer value="1"/>
<p>
Если вас автоматически не перенаправляет на другую страницу, нажмите эту 
<a href="1/g.php">ссылку</a>.
</p>
</card>
</wml>
<?
ob_end_flush();die("");