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
-- Structure de la table `_delivraptor_queue`
--

CREATE TABLE `_delivraptor_queue` (
  `id` int(11) NOT NULL,
  `numBordereau` bigint(20) DEFAULT NULL,
  `noCommande` int(11) NOT NULL,
  `destination` varchar(255) NOT NULL,
  `username` varchar(50) DEFAULT NULL,
  `date_creation` timestamp NULL DEFAULT current_timestamp(),
  `traite` tinyint(1) DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `_delivraptor_queue`
--

INSERT INTO `_delivraptor_queue` (`id`, `numBordereau`, `noCommande`, `destination`, `username`, `date_creation`, `traite`) VALUES
(5, 7159238835, 22132, 'test, 21322 hjks', 'admin', '2026-01-20 13:39:16', 1);

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `_delivraptor_queue`
--
ALTER TABLE `_delivraptor_queue`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `noCommande` (`noCommande`),
  ADD KEY `numBordereau` (`numBordereau`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `_delivraptor_queue`
--
ALTER TABLE `_delivraptor_queue`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- Contraintes pour les tables déchargées
--

--
-- Contraintes pour la table `_delivraptor_queue`
--
ALTER TABLE `_delivraptor_queue`
  ADD CONSTRAINT `_delivraptor_queue_ibfk_1` FOREIGN KEY (`numBordereau`) REFERENCES `_delivraptor_colis` (`numBordereau`) ON DELETE SET NULL;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
