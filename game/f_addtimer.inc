<?php
/**
 * Добавление/обновление таймера
 */
/**
 * Добавление/обновление таймера
 *
 * @param string       $location_id локация
 * @param int          $curr        тек. таймер в локации $location_id
 * @param int          $time        период
 * @param string|array $value       описание таймера в виде текста либо массива для установки или old для использования
 *                                  старого
 * @param int          $delete      флаг удаления текущего
 */
function addtimer($location_id, $curr, $time, $value = "old", $delete = 1)
{
    global $loc_t;
    loadloc($location_id);
    $new_time = time() + $time;
    while (isset($loc_t[$location_id][$new_time])) {
        $new_time++;
    }
    if ($value == "old") {
        $value = $loc_t[$location_id][$curr];
    }
    $loc_t[$location_id][$new_time] = $value;
    if ($delete && $curr) {
        unset($loc_t[$location_id][$curr]);
    }
}