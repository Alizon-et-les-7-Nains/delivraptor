
CREATE TABLE `_delivraptor_queue` (
  `id` int(11) NOT NULL,
  `numBordereau` bigint(20) DEFAULT NULL,
  `noCommande` int(11) NOT NULL,
  `destination` varchar(255) NOT NULL,
  `username` varchar(50) DEFAULT NULL,
  `date_creation` timestamp NULL DEFAULT current_timestamp(),
  `traite` tinyint(1) DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

ALTER TABLE `_delivraptor_queue`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `noCommande` (`noCommande`),
  ADD KEY `numBordereau` (`numBordereau`);


ALTER TABLE `_delivraptor_queue`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

ALTER TABLE `_delivraptor_queue`
  ADD CONSTRAINT `_delivraptor_queue_ibfk_1` FOREIGN KEY (`numBordereau`) REFERENCES `_delivraptor_colis` (`numBordereau`) ON DELETE SET NULL;
COMMIT;