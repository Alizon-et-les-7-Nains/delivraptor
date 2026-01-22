

CREATE TABLE `_delivraptor_colis_historique` (
  `id` int(11) NOT NULL,
  `numBordereau` bigint(20) NOT NULL,
  `etape` int(11) NOT NULL CHECK (`etape` between 1 and 9),
  `date_etape` datetime NOT NULL,
  `localisation` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

ALTER TABLE `_delivraptor_colis_historique`
  ADD PRIMARY KEY (`id`),
  ADD KEY `numBordereau` (`numBordereau`);

ALTER TABLE `_delivraptor_colis_historique`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=281;

ALTER TABLE `_delivraptor_colis_historique`
  ADD CONSTRAINT `_delivraptor_colis_historique_ibfk_1` FOREIGN KEY (`numBordereau`) REFERENCES `_delivraptor_colis` (`numBordereau`) ON DELETE CASCADE;
COMMIT;
