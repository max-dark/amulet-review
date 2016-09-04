<?php

interface Menu
{
    const mode_full = 0; // полное
    const mode_short = 1; // сокращенное
    const mode_page = 2; // на отдельной странице
    const mode_links = 3; // в виде ссылок (если не видно обычное меню)
}