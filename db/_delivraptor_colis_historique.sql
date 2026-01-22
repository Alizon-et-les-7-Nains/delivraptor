-- phpMyAdmin SQL Dump
-- version 5.2.2
-- https://www.phpmyadmin.net/
--
-- Hôte : mariadb:3306
-- Généré le : jeu. 22 jan. 2026 à 12:31
-- Version du serveur : 11.0.6-MariaDB-ubu2204
-- Version de PHP : 8.2.29

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `saedb`
--

-- --------------------------------------------------------

--
-- Structure de la table `_delivraptor_colis_historique`
--


CREATE TABLE `_delivraptor_colis_historique` (
  `id` int(11) NOT NULL,
  `numBordereau` bigint(20) NOT NULL,
  `etape` int(11) NOT NULL CHECK (`etape` between 1 and 9),
  `date_etape` datetime NOT NULL,
  `localisation` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `_delivraptor_colis_historique`
--

INSERT INTO `_delivraptor_colis_historique` (`id`, `numBordereau`, `etape`, `date_etape`, `localisation`) VALUES
(1, 5364065909, 2, '2026-01-20 12:41:01', 'En transit vers plateforme transporteur'),
(2, 5364065909, 3, '2026-01-20 12:42:01', 'Arrivé chez transporteur'),
(3, 5364065909, 4, '2026-01-20 12:43:01', 'Départ vers plateforme régionale'),
(4, 5364065909, 5, '2026-01-20 12:44:01', 'Arrivé plateforme régionale'),
(5, 5364065909, 6, '2026-01-20 12:45:01', 'Départ vers centre local'),
(6, 5364065909, 7, '2026-01-20 12:45:03', 'Arrivé centre local'),
(7, 5364065909, 8, '2026-01-20 12:45:04', 'Départ pour livraison finale'),
(8, 5364065909, 9, '2026-01-20 12:45:06', 'Chez destinataire'),
(9, 9309368015, 2, '2026-01-20 12:50:01', 'En transit vers plateforme transporteur'),
(10, 9309368015, 3, '2026-01-20 12:51:01', 'Arrivé chez transporteur'),
(11, 9237820167, 2, '2026-01-20 12:51:01', 'En transit vers plateforme transporteur'),
(12, 9237820167, 3, '2026-01-20 12:52:01', 'Arrivé chez transporteur'),
(13, 9309368015, 4, '2026-01-20 12:52:01', 'Départ vers plateforme régionale'),
(14, 9237820167, 4, '2026-01-20 12:53:01', 'Départ vers plateforme régionale'),
(15, 9309368015, 5, '2026-01-20 12:53:01', 'Arrivé plateforme régionale'),
(16, 9237820167, 5, '2026-01-20 12:54:01', 'Arrivé plateforme régionale'),
(17, 9309368015, 6, '2026-01-20 12:54:01', 'Départ vers centre local'),
(18, 9237820167, 6, '2026-01-20 12:55:01', 'Départ vers centre local'),
(19, 9309368015, 7, '2026-01-20 12:55:01', 'Arrivé centre local'),
(20, 9237820167, 7, '2026-01-20 12:56:02', 'Arrivé centre local'),
(21, 9309368015, 8, '2026-01-20 12:56:02', 'Départ pour livraison finale'),
(22, 3814315252, 2, '2026-01-20 12:56:40', 'En transit vers plateforme transporteur'),
(23, 9237820167, 8, '2026-01-20 12:56:40', 'Départ pour livraison finale'),
(24, 9309368015, 9, '2026-01-20 12:56:40', 'Chez destinataire'),
(25, 3814315252, 3, '2026-01-20 12:56:55', 'Arrivé chez transporteur'),
(26, 9237820167, 9, '2026-01-20 12:56:55', 'Chez destinataire'),
(27, 3814315252, 4, '2026-01-20 12:57:01', 'Départ vers plateforme régionale'),
(28, 3814315252, 5, '2026-01-20 12:57:05', 'Arrivé plateforme régionale'),
(29, 3814315252, 6, '2026-01-20 12:57:06', 'Départ vers centre local'),
(30, 3814315252, 7, '2026-01-20 12:57:06', 'Arrivé centre local'),
(31, 3814315252, 8, '2026-01-20 12:57:07', 'Départ pour livraison finale'),
(32, 3814315252, 9, '2026-01-20 12:57:07', 'Chez destinataire'),
(33, 9203336088, 2, '2026-01-20 13:20:01', 'En transit vers plateforme transporteur'),
(34, 2637637769, 2, '2026-01-20 13:20:01', 'En transit vers plateforme transporteur'),
(35, 2637637769, 3, '2026-01-20 13:20:43', 'Arrivé chez transporteur'),
(36, 9203336088, 3, '2026-01-20 13:20:43', 'Arrivé chez transporteur'),
(37, 2637637769, 4, '2026-01-20 13:20:43', 'Départ vers plateforme régionale'),
(38, 9203336088, 4, '2026-01-20 13:20:43', 'Départ vers plateforme régionale'),
(39, 2637637769, 5, '2026-01-20 13:20:44', 'Arrivé plateforme régionale'),
(40, 9203336088, 5, '2026-01-20 13:20:44', 'Arrivé plateforme régionale'),
(41, 2637637769, 6, '2026-01-20 13:20:45', 'Départ vers centre local'),
(42, 9203336088, 6, '2026-01-20 13:20:45', 'Départ vers centre local'),
(43, 2637637769, 7, '2026-01-20 13:20:45', 'Arrivé centre local'),
(44, 9203336088, 7, '2026-01-20 13:20:45', 'Arrivé centre local'),
(45, 2637637769, 8, '2026-01-20 13:20:52', 'Départ pour livraison finale'),
(46, 9203336088, 8, '2026-01-20 13:20:52', 'Départ pour livraison finale'),
(47, 2637637769, 9, '2026-01-20 13:20:53', 'Chez destinataire'),
(48, 9203336088, 9, '2026-01-20 13:20:53', 'Chez destinataire'),
(49, 4532021424, 2, '2026-01-20 13:34:01', 'En transit vers plateforme transporteur'),
(50, 4532021424, 3, '2026-01-20 13:35:01', 'Arrivé chez transporteur'),
(51, 7444932494, 2, '2026-01-20 13:35:01', 'En transit vers plateforme transporteur'),
(52, 4532021424, 4, '2026-01-20 13:36:01', 'Départ vers plateforme régionale'),
(53, 7444932494, 3, '2026-01-20 13:36:01', 'Arrivé chez transporteur'),
(54, 4532021424, 5, '2026-01-20 13:37:01', 'Arrivé plateforme régionale'),
(55, 7444932494, 4, '2026-01-20 13:37:01', 'Départ vers plateforme régionale'),
(56, 6599705224, 2, '2026-01-20 13:37:01', 'En transit vers plateforme transporteur'),
(57, 6599705224, 3, '2026-01-20 13:38:02', 'Arrivé chez transporteur'),
(58, 7444932494, 5, '2026-01-20 13:38:02', 'Arrivé plateforme régionale'),
(59, 4532021424, 6, '2026-01-20 13:38:02', 'Départ vers centre local'),
(60, 6599705224, 4, '2026-01-20 13:39:01', 'Départ vers plateforme régionale'),
(61, 9253520822, 2, '2026-01-20 13:39:01', 'En transit vers plateforme transporteur'),
(62, 7444932494, 6, '2026-01-20 13:39:01', 'Départ vers centre local'),
(63, 4532021424, 7, '2026-01-20 13:39:01', 'Arrivé centre local'),
(64, 6599705224, 5, '2026-01-20 13:40:01', 'Arrivé plateforme régionale'),
(65, 9253520822, 3, '2026-01-20 13:40:01', 'Arrivé chez transporteur'),
(66, 2040070074, 2, '2026-01-20 13:40:01', 'En transit vers plateforme transporteur'),
(67, 7444932494, 7, '2026-01-20 13:40:01', 'Arrivé centre local'),
(68, 4532021424, 8, '2026-01-20 13:40:01', 'Départ pour livraison finale'),
(69, 2040070074, 3, '2026-01-20 13:41:01', 'Arrivé chez transporteur'),
(70, 7159238835, 2, '2026-01-20 13:41:01', 'En transit vers plateforme transporteur'),
(71, 9253520822, 4, '2026-01-20 13:41:01', 'Départ vers plateforme régionale'),
(72, 6599705224, 6, '2026-01-20 13:41:01', 'Départ vers centre local'),
(73, 7444932494, 8, '2026-01-20 13:41:01', 'Départ pour livraison finale'),
(74, 4532021424, 9, '2026-01-20 13:41:01', 'Chez destinataire'),
(75, 2040070074, 4, '2026-01-20 13:42:01', 'Départ vers plateforme régionale'),
(76, 7159238835, 3, '2026-01-20 13:42:01', 'Arrivé chez transporteur'),
(77, 9253520822, 5, '2026-01-20 13:42:01', 'Arrivé plateforme régionale'),
(78, 6599705224, 7, '2026-01-20 13:42:01', 'Arrivé centre local'),
(79, 7444932494, 9, '2026-01-20 13:42:01', 'Chez destinataire'),
(80, 2040070074, 5, '2026-01-20 13:43:01', 'Arrivé plateforme régionale'),
(81, 7159238835, 4, '2026-01-20 13:43:01', 'Départ vers plateforme régionale'),
(82, 6599705224, 8, '2026-01-20 13:43:01', 'Départ pour livraison finale'),
(83, 9253520822, 6, '2026-01-20 13:43:01', 'Départ vers centre local'),
(84, 7159238835, 5, '2026-01-20 13:44:01', 'Arrivé plateforme régionale'),
(85, 6599705224, 9, '2026-01-20 13:44:01', 'Chez destinataire'),
(86, 9253520822, 7, '2026-01-20 13:44:01', 'Arrivé centre local'),
(87, 2040070074, 6, '2026-01-20 13:44:01', 'Départ vers centre local'),
(88, 7159238835, 6, '2026-01-20 13:45:02', 'Départ vers centre local'),
(89, 9253520822, 8, '2026-01-20 13:45:02', 'Départ pour livraison finale'),
(90, 2040070074, 7, '2026-01-20 13:45:02', 'Arrivé centre local'),
(91, 7159238835, 7, '2026-01-20 13:46:01', 'Arrivé centre local'),
(92, 9253520822, 9, '2026-01-20 13:46:01', 'Chez destinataire'),
(93, 2040070074, 8, '2026-01-20 13:46:01', 'Départ pour livraison finale'),
(94, 7484883887, 2, '2026-01-20 13:47:01', 'En transit vers plateforme transporteur'),
(95, 7159238835, 8, '2026-01-20 13:47:01', 'Départ pour livraison finale'),
(96, 2040070074, 9, '2026-01-20 13:47:01', 'Chez destinataire'),
(97, 7484883887, 3, '2026-01-20 13:48:01', 'Arrivé chez transporteur'),
(98, 7159238835, 9, '2026-01-20 13:48:01', 'Chez destinataire'),
(99, 7484883887, 4, '2026-01-20 13:49:01', 'Départ vers plateforme régionale'),
(100, 7484883887, 5, '2026-01-20 13:50:01', 'Arrivé plateforme régionale'),
(101, 7484883887, 6, '2026-01-20 13:51:01', 'Départ vers centre local'),
(102, 7484883887, 7, '2026-01-20 13:52:01', 'Arrivé centre local'),
(103, 7484883887, 8, '2026-01-20 13:53:01', 'Départ pour livraison finale'),
(104, 7484883887, 9, '2026-01-20 13:54:01', 'Chez destinataire'),
(105, 5573291360, 2, '2026-01-20 14:19:01', 'En transit vers plateforme transporteur'),
(106, 5573291360, 3, '2026-01-20 14:20:01', 'Arrivé chez transporteur'),
(107, 5573291360, 4, '2026-01-20 14:21:01', 'Départ vers plateforme régionale'),
(108, 5573291360, 5, '2026-01-20 14:22:01', 'Arrivé plateforme régionale'),
(109, 5573291360, 6, '2026-01-20 14:23:01', 'Départ vers centre local'),
(110, 9497828882, 2, '2026-01-20 14:24:01', 'En transit vers plateforme transporteur'),
(111, 5573291360, 7, '2026-01-20 14:24:01', 'Arrivé centre local'),
(112, 9497828882, 3, '2026-01-20 14:25:01', 'Arrivé chez transporteur'),
(113, 5573291360, 8, '2026-01-20 14:25:01', 'Départ pour livraison finale'),
(114, 9497828882, 4, '2026-01-20 14:26:02', 'Départ vers plateforme régionale'),
(115, 5362027140, 2, '2026-01-20 14:26:02', 'En transit vers plateforme transporteur'),
(116, 5573291360, 9, '2026-01-20 14:26:02', 'Chez destinataire'),
(117, 5362027140, 3, '2026-01-20 14:27:01', 'Arrivé chez transporteur'),
(118, 9497828882, 5, '2026-01-20 14:27:01', 'Arrivé plateforme régionale'),
(119, 5362027140, 4, '2026-01-20 14:28:01', 'Départ vers plateforme régionale'),
(120, 9497828882, 6, '2026-01-20 14:28:01', 'Départ vers centre local'),
(121, 5362027140, 5, '2026-01-20 14:29:01', 'Arrivé plateforme régionale'),
(122, 9497828882, 7, '2026-01-20 14:29:01', 'Arrivé centre local'),
(123, 9497828882, 8, '2026-01-20 14:30:01', 'Départ pour livraison finale'),
(124, 5362027140, 6, '2026-01-20 14:30:01', 'Départ vers centre local'),
(125, 9497828882, 9, '2026-01-20 14:31:01', 'Chez destinataire'),
(126, 5362027140, 7, '2026-01-20 14:31:01', 'Arrivé centre local'),
(127, 5362027140, 8, '2026-01-20 14:32:01', 'Départ pour livraison finale'),
(128, 5362027140, 9, '2026-01-20 14:33:01', 'Chez destinataire'),
(129, 4073954663, 2, '2026-01-20 16:04:01', 'En transit vers plateforme transporteur'),
(130, 4073954663, 3, '2026-01-20 16:05:02', 'Arrivé chez transporteur'),
(131, 4073954663, 4, '2026-01-20 16:06:01', 'Départ vers plateforme régionale'),
(132, 4073954663, 5, '2026-01-20 16:07:01', 'Arrivé plateforme régionale'),
(133, 4073954663, 6, '2026-01-20 16:08:01', 'Départ vers centre local'),
(134, 4073954663, 7, '2026-01-20 16:09:01', 'Arrivé centre local'),
(135, 4073954663, 8, '2026-01-20 16:10:01', 'Départ pour livraison finale'),
(136, 4073954663, 9, '2026-01-20 16:11:01', 'Chez destinataire'),
(137, 1840421921, 2, '2026-01-20 16:43:02', 'En transit vers plateforme transporteur'),
(138, 1840421921, 3, '2026-01-20 16:44:01', 'Arrivé chez transporteur'),
(139, 1840421921, 4, '2026-01-20 16:45:01', 'Départ vers plateforme régionale'),
(140, 1840421921, 5, '2026-01-20 16:46:01', 'Arrivé plateforme régionale'),
(141, 1840421921, 6, '2026-01-20 16:47:01', 'Départ vers centre local'),
(142, 1840421921, 7, '2026-01-20 16:48:01', 'Arrivé centre local'),
(143, 1840421921, 8, '2026-01-20 16:49:01', 'Départ pour livraison finale'),
(144, 1840421921, 9, '2026-01-20 16:50:02', 'Chez destinataire'),
(145, 5563302303, 2, '2026-01-20 19:23:01', 'En transit vers plateforme transporteur'),
(146, 5563302303, 3, '2026-01-20 19:24:01', 'Arrivé chez transporteur'),
(147, 5563302303, 4, '2026-01-20 19:25:01', 'Départ vers plateforme régionale'),
(148, 5563302303, 5, '2026-01-20 19:26:01', 'Arrivé plateforme régionale'),
(149, 5563302303, 6, '2026-01-20 19:27:02', 'Départ vers centre local'),
(150, 5563302303, 7, '2026-01-20 19:28:01', 'Arrivé centre local'),
(151, 5563302303, 8, '2026-01-20 19:29:01', 'Départ pour livraison finale'),
(152, 5563302303, 9, '2026-01-20 19:30:01', 'Chez destinataire'),
(153, 6375815420, 2, '2026-01-20 21:21:01', 'En transit vers plateforme transporteur'),
(154, 6375815420, 3, '2026-01-20 21:22:01', 'Arrivé chez transporteur'),
(155, 6375815420, 4, '2026-01-20 21:23:01', 'Départ vers plateforme régionale'),
(156, 2839043729, 2, '2026-01-20 21:23:01', 'En transit vers plateforme transporteur'),
(157, 2839043729, 3, '2026-01-20 21:24:01', 'Arrivé chez transporteur'),
(158, 6375815420, 5, '2026-01-20 21:24:01', 'Arrivé plateforme régionale'),
(159, 2839043729, 4, '2026-01-20 21:25:01', 'Départ vers plateforme régionale'),
(160, 6469986877, 2, '2026-01-20 21:25:01', 'En transit vers plateforme transporteur'),
(161, 6375815420, 6, '2026-01-20 21:25:01', 'Départ vers centre local'),
(162, 2839043729, 5, '2026-01-20 21:26:01', 'Arrivé plateforme régionale'),
(163, 6469986877, 3, '2026-01-20 21:26:01', 'Arrivé chez transporteur'),
(164, 6375815420, 7, '2026-01-20 21:26:01', 'Arrivé centre local'),
(165, 6469986877, 4, '2026-01-20 21:27:01', 'Départ vers plateforme régionale'),
(166, 2839043729, 6, '2026-01-20 21:27:01', 'Départ vers centre local'),
(167, 6375815420, 8, '2026-01-20 21:27:01', 'Départ pour livraison finale'),
(168, 6469986877, 5, '2026-01-20 21:28:02', 'Arrivé plateforme régionale'),
(169, 2839043729, 7, '2026-01-20 21:28:02', 'Arrivé centre local'),
(170, 6375815420, 9, '2026-01-20 21:28:02', 'Chez destinataire'),
(171, 6469986877, 6, '2026-01-20 21:29:01', 'Départ vers centre local'),
(172, 2839043729, 8, '2026-01-20 21:29:01', 'Départ pour livraison finale'),
(173, 6469986877, 7, '2026-01-20 21:30:01', 'Arrivé centre local'),
(174, 2839043729, 9, '2026-01-20 21:30:01', 'Chez destinataire'),
(175, 6469986877, 8, '2026-01-20 21:31:01', 'Départ pour livraison finale'),
(176, 6469986877, 9, '2026-01-20 21:32:01', 'Chez destinataire'),
(177, 3712722359, 2, '2026-01-20 22:57:01', 'En transit vers plateforme transporteur'),
(178, 3712722359, 3, '2026-01-20 22:58:01', 'Arrivé chez transporteur'),
(179, 3712722359, 4, '2026-01-20 22:59:01', 'Départ vers plateforme régionale'),
(180, 3712722359, 5, '2026-01-20 23:00:01', 'Arrivé plateforme régionale'),
(181, 3712722359, 6, '2026-01-20 23:01:01', 'Départ vers centre local'),
(182, 3712722359, 7, '2026-01-20 23:02:01', 'Arrivé centre local'),
(183, 3712722359, 8, '2026-01-20 23:03:01', 'Départ pour livraison finale'),
(184, 3712722359, 9, '2026-01-20 23:04:01', 'Chez destinataire'),
(185, 7964716223, 2, '2026-01-21 08:31:01', 'En transit vers plateforme transporteur'),
(186, 7964716223, 3, '2026-01-21 08:32:01', 'Arrivé chez transporteur'),
(187, 7964716223, 4, '2026-01-21 08:33:01', 'Départ vers plateforme régionale'),
(188, 7964716223, 5, '2026-01-21 08:34:02', 'Arrivé plateforme régionale'),
(189, 7964716223, 6, '2026-01-21 08:35:01', 'Départ vers centre local'),
(190, 7964716223, 7, '2026-01-21 08:36:01', 'Arrivé centre local'),
(191, 7964716223, 8, '2026-01-21 08:37:01', 'Départ pour livraison finale'),
(192, 7964716223, 9, '2026-01-21 08:38:01', 'Chez destinataire'),
(193, 5992895055, 2, '2026-01-21 08:40:01', 'En transit vers plateforme transporteur'),
(194, 5992895055, 3, '2026-01-21 08:41:01', 'Arrivé chez transporteur'),
(195, 5992895055, 4, '2026-01-21 08:42:01', 'Départ vers plateforme régionale'),
(196, 5992895055, 5, '2026-01-21 08:43:01', 'Arrivé plateforme régionale'),
(197, 5992895055, 6, '2026-01-21 08:44:02', 'Départ vers centre local'),
(198, 5992895055, 7, '2026-01-21 08:45:01', 'Arrivé centre local'),
(199, 5992895055, 8, '2026-01-21 08:46:01', 'Départ pour livraison finale'),
(200, 5992895055, 9, '2026-01-21 08:47:01', 'Chez destinataire'),
(201, 4944412917, 2, '2026-01-21 08:52:01', 'En transit vers plateforme transporteur'),
(202, 4944412917, 3, '2026-01-21 08:53:01', 'Arrivé chez transporteur'),
(203, 4944412917, 4, '2026-01-21 08:54:02', 'Départ vers plateforme régionale'),
(204, 4944412917, 5, '2026-01-21 08:55:01', 'Arrivé plateforme régionale'),
(205, 4944412917, 6, '2026-01-21 08:56:01', 'Départ vers centre local'),
(206, 4944412917, 7, '2026-01-21 08:57:01', 'Arrivé centre local'),
(207, 4944412917, 8, '2026-01-21 08:58:01', 'Départ pour livraison finale'),
(208, 4944412917, 9, '2026-01-21 08:59:01', 'Chez destinataire'),
(209, 9131266605, 2, '2026-01-21 09:01:01', 'En transit vers plateforme transporteur'),
(210, 9131266605, 3, '2026-01-21 09:02:01', 'Arrivé chez transporteur'),
(211, 9131266605, 4, '2026-01-21 09:03:01', 'Départ vers plateforme régionale'),
(212, 9131266605, 5, '2026-01-21 09:04:02', 'Arrivé plateforme régionale'),
(213, 9412625604, 2, '2026-01-21 09:04:02', 'En transit vers plateforme transporteur'),
(214, 9412625604, 3, '2026-01-21 09:05:01', 'Arrivé chez transporteur'),
(215, 9131266605, 6, '2026-01-21 09:05:01', 'Départ vers centre local'),
(216, 9412625604, 4, '2026-01-21 09:06:01', 'Départ vers plateforme régionale'),
(217, 9131266605, 7, '2026-01-21 09:06:01', 'Arrivé centre local'),
(218, 9412625604, 5, '2026-01-21 09:07:01', 'Arrivé plateforme régionale'),
(219, 9131266605, 8, '2026-01-21 09:07:01', 'Départ pour livraison finale'),
(220, 9131266605, 9, '2026-01-21 09:08:01', 'Chez destinataire'),
(221, 9412625604, 6, '2026-01-21 09:08:01', 'Départ vers centre local'),
(222, 9412625604, 7, '2026-01-21 09:09:01', 'Arrivé centre local'),
(223, 9412625604, 8, '2026-01-21 09:10:01', 'Départ pour livraison finale'),
(224, 9412625604, 9, '2026-01-21 09:11:01', 'Chez destinataire'),
(225, 3747221028, 2, '2026-01-21 09:17:01', 'En transit vers plateforme transporteur'),
(226, 3747221028, 3, '2026-01-21 09:18:01', 'Arrivé chez transporteur'),
(227, 3747221028, 4, '2026-01-21 09:19:01', 'Départ vers plateforme régionale'),
(228, 3747221028, 5, '2026-01-21 09:20:01', 'Arrivé plateforme régionale'),
(229, 6234604967, 2, '2026-01-21 09:20:01', 'En transit vers plateforme transporteur'),
(230, 6234604967, 3, '2026-01-21 09:21:01', 'Arrivé chez transporteur'),
(231, 3747221028, 6, '2026-01-21 09:21:01', 'Départ vers centre local'),
(232, 6234604967, 4, '2026-01-21 09:22:01', 'Départ vers plateforme régionale'),
(233, 3747221028, 7, '2026-01-21 09:22:01', 'Arrivé centre local'),
(234, 6234604967, 5, '2026-01-21 09:23:01', 'Arrivé plateforme régionale'),
(235, 3747221028, 8, '2026-01-21 09:23:01', 'Départ pour livraison finale'),
(236, 3747221028, 9, '2026-01-21 09:24:02', 'Chez destinataire'),
(237, 6234604967, 6, '2026-01-21 09:24:02', 'Départ vers centre local'),
(238, 6234604967, 7, '2026-01-21 09:25:01', 'Arrivé centre local'),
(239, 6234604967, 8, '2026-01-21 09:26:01', 'Départ pour livraison finale'),
(240, 6234604967, 9, '2026-01-21 09:27:01', 'Chez destinataire'),
(241, 3533039181, 2, '2026-01-21 12:31:01', 'En transit vers plateforme transporteur'),
(242, 3533039181, 3, '2026-01-21 12:32:01', 'Arrivé chez transporteur'),
(243, 3533039181, 4, '2026-01-21 12:33:01', 'Départ vers plateforme régionale'),
(244, 3533039181, 5, '2026-01-21 12:34:01', 'Arrivé plateforme régionale'),
(245, 3533039181, 6, '2026-01-21 12:35:02', 'Départ vers centre local'),
(246, 3799851784, 2, '2026-01-21 12:36:01', 'En transit vers plateforme transporteur'),
(247, 3533039181, 7, '2026-01-21 12:36:01', 'Arrivé centre local'),
(248, 3799851784, 3, '2026-01-21 12:37:01', 'Arrivé chez transporteur'),
(249, 3533039181, 8, '2026-01-21 12:37:01', 'Départ pour livraison finale'),
(250, 3799851784, 4, '2026-01-21 12:38:01', 'Départ vers plateforme régionale'),
(251, 3533039181, 9, '2026-01-21 12:38:01', 'Chez destinataire'),
(252, 3799851784, 5, '2026-01-21 12:39:01', 'Arrivé plateforme régionale'),
(253, 3799851784, 6, '2026-01-21 12:40:01', 'Départ vers centre local'),
(254, 3799851784, 7, '2026-01-21 12:41:01', 'Arrivé centre local'),
(255, 3799851784, 8, '2026-01-21 12:42:01', 'Départ pour livraison finale'),
(256, 3799851784, 9, '2026-01-21 12:43:01', 'Chez destinataire'),
(257, 5811485597, 2, '2026-01-21 13:01:01', 'En transit vers plateforme transporteur'),
(258, 5811485597, 3, '2026-01-21 13:02:01', 'Arrivé chez transporteur'),
(259, 5811485597, 4, '2026-01-21 13:03:01', 'Départ vers plateforme régionale'),
(260, 5811485597, 5, '2026-01-21 13:04:01', 'Arrivé plateforme régionale'),
(261, 5811485597, 6, '2026-01-21 13:05:01', 'Départ vers centre local'),
(262, 5811485597, 7, '2026-01-21 13:06:01', 'Arrivé centre local'),
(263, 5811485597, 8, '2026-01-21 13:07:02', 'Départ pour livraison finale'),
(264, 5811485597, 9, '2026-01-21 13:08:01', 'Chez destinataire'),
(265, 7959229557, 2, '2026-01-21 14:00:01', 'En transit vers plateforme transporteur'),
(266, 7959229557, 3, '2026-01-21 14:01:01', 'Arrivé chez transporteur'),
(267, 7959229557, 4, '2026-01-21 14:02:01', 'Départ vers plateforme régionale'),
(268, 7959229557, 5, '2026-01-21 14:03:02', 'Arrivé plateforme régionale'),
(269, 7959229557, 6, '2026-01-21 14:04:01', 'Départ vers centre local'),
(270, 7959229557, 7, '2026-01-21 14:05:01', 'Arrivé centre local'),
(271, 7959229557, 8, '2026-01-21 14:06:01', 'Départ pour livraison finale'),
(272, 7959229557, 9, '2026-01-21 14:07:01', 'Chez destinataire'),
(273, 6537451174, 2, '2026-01-21 14:44:01', 'En transit vers plateforme transporteur'),
(274, 6537451174, 3, '2026-01-21 14:45:01', 'Arrivé chez transporteur'),
(275, 6537451174, 4, '2026-01-21 14:46:01', 'Départ vers plateforme régionale'),
(276, 6537451174, 5, '2026-01-21 14:47:01', 'Arrivé plateforme régionale'),
(277, 6537451174, 6, '2026-01-21 14:48:01', 'Départ vers centre local'),
(278, 6537451174, 7, '2026-01-21 14:49:02', 'Arrivé centre local'),
(279, 6537451174, 8, '2026-01-21 14:50:01', 'Départ pour livraison finale'),
(280, 6537451174, 9, '2026-01-21 14:51:01', 'Chez destinataire');

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `_delivraptor_colis_historique`
--
ALTER TABLE `_delivraptor_colis_historique`
  ADD PRIMARY KEY (`id`),
  ADD KEY `numBordereau` (`numBordereau`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `_delivraptor_colis_historique`
--
ALTER TABLE `_delivraptor_colis_historique`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=281;

--
-- Contraintes pour les tables déchargées
--

--
-- Contraintes pour la table `_delivraptor_colis_historique`
--
ALTER TABLE `_delivraptor_colis_historique`
  ADD CONSTRAINT `_delivraptor_colis_historique_ibfk_1` FOREIGN KEY (`numBordereau`) REFERENCES `_delivraptor_colis` (`numBordereau`) ON DELETE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
