
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


ALTER TABLE `_delivraptor_colis`
  ADD PRIMARY KEY (`numBordereau`),
  ADD UNIQUE KEY `noCommande` (`noCommande`);
COMMIT;
