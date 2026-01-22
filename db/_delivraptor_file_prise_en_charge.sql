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
-- Structure de la table `_delivraptor_file_prise_en_charge`
--

CREATE TABLE `_delivraptor_file_prise_en_charge` (
  `numBordereau` bigint(20) NOT NULL,
  `date_entree` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `_delivraptor_file_prise_en_charge`
--
ALTER TABLE `_delivraptor_file_prise_en_charge`
  ADD PRIMARY KEY (`numBordereau`);

--
-- Contraintes pour les tables déchargées
--

--
-- Contraintes pour la table `_delivraptor_file_prise_en_charge`
--
ALTER TABLE `_delivraptor_file_prise_en_charge`
  ADD CONSTRAINT `_delivraptor_file_prise_en_charge_ibfk_1` FOREIGN KEY (`numBordereau`) REFERENCES `_delivraptor_colis` (`numBordereau`) ON DELETE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
