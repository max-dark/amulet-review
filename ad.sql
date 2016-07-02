-- phpMyAdmin SQL Dump
-- version 3.3.5
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Ноя 10 2010 г., 13:51
-- Версия сервера: 5.1.34
-- Версия PHP: 5.3.3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT = @@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS = @@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION = @@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `wavilon_ad`
--

-- --------------------------------------------------------

--
-- Структура таблицы `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `ind`      INT(11)     NOT NULL        AUTO_INCREMENT,
  `nick`     TEXT,
  `pass`     VARCHAR(15)                 DEFAULT NULL,
  `email`    TEXT,
  `lastrefr` VARCHAR(14) NOT NULL        DEFAULT '',
  `regtime`  VARCHAR(14) NOT NULL        DEFAULT '',
  `messlim`  SMALLINT(6)                 DEFAULT NULL,
  `names`    TEXT,
  `vals`     TEXT,
  PRIMARY KEY (`ind`),
  KEY `nick` (`nick`(10))
)
  ENGINE = MyISAM
  DEFAULT CHARSET = utf8
  AUTO_INCREMENT = 16;

--
-- Дамп данных таблицы `users`
--

INSERT INTO `users` (`ind`, `nick`, `pass`, `email`, `lastrefr`, `regtime`, `messlim`, `names`, `vals`) VALUES
  (13, 'rastomanchik', '123456', 'sanek7.7@mail.ru', '', '1289395326', NULL, ':gamedata',
   ':a!~!13!~!{s!~!4!~!"user";s!~!34!~!"123456||m|33||1289395326|0|0|0|0|0";s!~!4!~!"char";s!~!98!~!"Rastomanchik|20|20|20|20|1289410040|1289410039||0|0|1289405190|1289410270||||1289410030|1289410030";s!~!6!~!"skills";s!~!63!~!"1|3|1|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0";s!~!7!~!"journal";s!~!0!~!"";s!~!5!~!"items";s!~!13!~!"i.w.k.begin!~!1";s!~!5!~!"magic";s!~!0!~!"";s!~!3!~!"war";s!~!45!~!"50|0|2|3|0|0|3|0|0|0|0|-15|кулаками|5|0|0|0|0";s!~!3!~!"srv";i!~!1289410040;s!~!1!~!"o";s!~!27!~!"30|15000|1|0|0|1|0|22|0|1|0";s!~!5!~!"equip";s!~!0!~!"";s!~!5!~!"priem";s!~!0!~!"";s!~!3!~!"loc";s!~!9!~!"x1069x520";s!~!4!~!"time";i!~!1289410764;}'),
  (14, 'rastoman', '123456', 'sanek7.7@mail.ru', '', '1289395943', NULL, ':gamedata',
   ':a!~!12!~!{s!~!4!~!"user";s!~!34!~!"123456||m|33||1289395943|0|0|0|0|0";s!~!4!~!"char";s!~!85!~!"Rastoman|20|20|20|20|1289416856|1289416855||0|0|0|1289416859||||1289416846|1289416846";s!~!6!~!"skills";s!~!63!~!"1|1|1|0|2|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0";s!~!7!~!"journal";s!~!0!~!"";s!~!5!~!"items";s!~!13!~!"i.w.k.begin!~!2";s!~!5!~!"magic";s!~!0!~!"";s!~!3!~!"war";s!~!45!~!"30|0|2|4|0|0|1|0|0|0|0|-15|кулаками|5|0|0|0|0";s!~!3!~!"srv";i!~!1289416856;s!~!1!~!"o";s!~!27!~!"30|15000|1|0|0|1|0|22|0|1|0";s!~!5!~!"equip";s!~!0!~!"";s!~!3!~!"loc";s!~!9!~!"x1032x471";s!~!4!~!"time";i!~!1289416859;}'),
  (15, '6ahdut', '9l5557779l', 'Tarakan1993@bk.ru', '', '1289398373', NULL, ':gamedata',
   ':a!~!14!~!{s!~!4!~!"user";s!~!38!~!"9l5557779l||m|17||1289398373|0|0|0|0|0";s!~!4!~!"char";s!~!93!~!"6ahdut|20|20|20|20|1289415653|1289411297|n.c.wolf.dl|0|0||1289418177||||1289411288|1289411288";s!~!6!~!"skills";s!~!63!~!"1|1|1|0|2|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0";s!~!7!~!"journal";s!~!0!~!"";s!~!5!~!"items";s!~!0!~!"";s!~!5!~!"magic";s!~!0!~!"";s!~!3!~!"war";s!~!45!~!"30|0|2|4|0|0|1|0|0|0|0|-15|кулаками|5|0|0|0|0";s!~!3!~!"srv";i!~!1289411298;s!~!5!~!"equip";s!~!0!~!"";s!~!1!~!"o";s!~!31!~!"30|15000|1|0|0|0|0|301021|3|0|0";s!~!5!~!"priem";s!~!0!~!"";s!~!4!~!"bank";s!~!0!~!"";s!~!3!~!"loc";s!~!9!~!"x1092x474";s!~!4!~!"time";i!~!1289418177;}');
