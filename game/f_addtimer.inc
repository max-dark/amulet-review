<?php
/**Добавление/обновление таймера
 * @param string $loct локация
 * @param int $curr текущий таймер
 * @param int $time период
 * @param string $text текст для установки или old для использования старого
 * @param int $delete флаг удаления текущего
 */
function addtimer($loct, $curr, $time, $text = "old", $delete = 1)
{

//$curr - тек. таймер в локации $loct
    global $loc_i, $loc_t, $loc, $login;
    loadloc($loct);
    $new = time() + $time;
    while (isset($loc_t[$loct][$new]))
        $new++;
    if ($text == "old")
        $text = $loc_t[$loct][$curr];
    $loc_t[$loct][$new] = $text;
    if ($delete && $curr)
        unset($loc_t[$loct][$curr]);
}