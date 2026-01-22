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
-- Structure de la table `_delivraptor_colis`
--

CREATE TABLE `_delivraptor_colis` (
  `numBordereau` bigint(20) NOT NULL,
  `noCommande` int(11) NOT NULL,
  `destination` varchar(255) NOT NULL,
  `localisation` varchar(255) DEFAULT NULL,
  `etape` int(11) NOT NULL CHECK (`etape` between 1 and 9),
  `date_etape` datetime NOT NULL,
  `contact` tinyint(1) DEFAULT 0,
  `livraison_type` enum('MAINS_PROPRES','ABSENT','REFUSE') DEFAULT NULL,
  `refus_raison` varchar(255) DEFAULT NULL,
  `photo_path` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `_delivraptor_colis`
--

INSERT INTO `_delivraptor_colis` (`numBordereau`, `noCommande`, `destination`, `localisation`, `etape`, `date_etape`, `contact`, `livraison_type`, `refus_raison`, `photo_path`) VALUES
(1840421921, 182284, 'test, 22000 SB', 'Chez destinataire', 9, '2026-01-20 16:50:02', 0, 'MAINS_PROPRES', NULL, NULL),
(2040070074, 183995, '4 rue de la rue, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 13:47:01', 0, 'MAINS_PROPRES', NULL, NULL),
(2637637769, 670396, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 13:20:53', 0, 'REFUSE', 'Refusé par le destinataire', NULL),
(2839043729, 120747, '1 rue Pas chez moi, 56300 Pontivy', 'Chez destinataire', 9, '2026-01-20 21:30:01', 0, 'ABSENT', NULL, '../../images/imgBoiteAuxLettres.jpg'),
(3533039181, 835283, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 12:38:01', 0, 'REFUSE', 'Colis endommagé', NULL),
(3712722359, 410226, 'azert, 35500 Vitré', 'Chez destinataire', 9, '2026-01-20 23:04:01', 0, 'MAINS_PROPRES', NULL, NULL),
(3747221028, 467037, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 09:24:02', 0, 'REFUSE', 'Adresse incorrecte', NULL),
(3799851784, 407225, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 12:43:01', 0, 'ABSENT', NULL, '../../images/imgBoiteAuxLettres.jpg'),
(3814315252, 815004, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 12:57:07', 0, 'REFUSE', 'Colis endommagé', NULL),
(4073954663, 383409, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 16:11:01', 0, 'MAINS_PROPRES', NULL, NULL),
(4532021424, 971388, '11 rue de la touche, 35500 Saint-M\'hervé', 'Chez destinataire', 9, '2026-01-20 13:41:01', 0, 'REFUSE', 'Colis endommagé', NULL),
(4944412917, 782186, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 08:59:01', 0, 'REFUSE', 'Refusé par le destinataire', NULL),
(5362027140, 679400, '4 rue de la rue, 48630 Saint-JEan', 'Chez destinataire', 9, '2026-01-20 14:33:01', 0, 'REFUSE', 'Adresse incorrecte', NULL),
(5364065909, 252765, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 12:45:06', 0, 'MAINS_PROPRES', NULL, NULL),
(5563302303, 570357, '4 rue de la rue, 48630 Saint-JEan', 'Chez destinataire', 9, '2026-01-20 19:30:01', 0, 'MAINS_PROPRES', NULL, NULL),
(5573291360, 430561, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 14:26:02', 0, 'REFUSE', 'Destinataire absent', NULL),
(5811485597, 94922, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 13:08:01', 0, 'MAINS_PROPRES', NULL, NULL),
(5992895055, 355728, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 08:47:01', 0, 'MAINS_PROPRES', NULL, NULL),
(6234604967, 365186, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 09:27:01', 0, 'REFUSE', 'Adresse incorrecte', NULL),
(6375815420, 491804, '1 Rue Chez moi, 56300 Pontivy', 'Chez destinataire', 9, '2026-01-20 21:28:02', 0, 'MAINS_PROPRES', NULL, NULL),
(6469986877, 754563, '9 rue des lilas, 35500 Le Mans', 'Chez destinataire', 9, '2026-01-20 21:32:01', 0, 'REFUSE', 'Destinataire absent', NULL),
(6537451174, 847945, '27 Chemin du Pavillon, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 14:51:01', 0, 'ABSENT', NULL, '../../images/imgBoiteAuxLettres.jpg'),
(6599705224, 675376, '11 rue de la touché, 35500 Saint-M\'hervé', 'Chez destinataire', 9, '2026-01-20 13:44:01', 0, 'REFUSE', 'Colis endommagé', NULL),
(7159238835, 22132, 'test, 21322 hjks', 'Chez destinataire', 9, '2026-01-20 13:48:01', 0, 'MAINS_PROPRES', NULL, NULL),
(7444932494, 844961, '11 rue de la touché, 35500 Saint-M\'hervé', 'Chez destinataire', 9, '2026-01-20 13:42:01', 0, 'MAINS_PROPRES', NULL, NULL),
(7484883887, 620908, '4 rue de la rue, 48630 Saint-JEan', 'Chez destinataire', 9, '2026-01-20 13:54:01', 0, 'MAINS_PROPRES', NULL, NULL),
(7959229557, 172410, '27 Chemin du Pavillon, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 14:07:01', 0, 'ABSENT', NULL, '../../images/imgBoiteAuxLettres.jpg'),
(7964716223, 156995, '3 rue Edouard Branly, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 08:38:01', 0, 'MAINS_PROPRES', NULL, NULL),
(9131266605, 819654, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 09:08:01', 0, 'ABSENT', NULL, '../../images/imgBoiteAuxLettres.jpg'),
(9203336088, 472083, '11 rue de la touche, 35500 Saint-M\'hervé', 'Chez destinataire', 9, '2026-01-20 13:20:53', 0, 'MAINS_PROPRES', NULL, NULL),
(9237820167, 76153, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 12:56:55', 0, 'MAINS_PROPRES', NULL, NULL),
(9253520822, 443180, '122, 35500 Saint-M\'hervé', 'Chez destinataire', 9, '2026-01-20 13:46:01', 0, 'MAINS_PROPRES', NULL, NULL),
(9309368015, 371958, '11 rue de la touche, 35500 Saint-M\'hervé', 'Chez destinataire', 9, '2026-01-20 12:56:40', 0, 'ABSENT', NULL, './images/imgBoiteAuxLettres.jpg'),
(9412625604, 897816, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-21 09:11:01', 0, 'MAINS_PROPRES', NULL, NULL),
(9497828882, 658950, '1 Route Du Krec H, 22300 Lannion', 'Chez destinataire', 9, '2026-01-20 14:31:01', 0, 'REFUSE', 'Refusé par le destinataire', NULL);

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `_delivraptor_colis`
--
ALTER TABLE `_delivraptor_colis`
  ADD PRIMARY KEY (`numBordereau`),
  ADD UNIQUE KEY `noCommande` (`noCommande`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
