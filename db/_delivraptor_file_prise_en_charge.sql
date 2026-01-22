
CREATE TABLE `_delivraptor_file_prise_en_charge` (
  `numBordereau` bigint(20) NOT NULL,
  `date_entree` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

ALTER TABLE `_delivraptor_file_prise_en_charge`
  ADD PRIMARY KEY (`numBordereau`);


ALTER TABLE `_delivraptor_file_prise_en_charge`
  ADD CONSTRAINT `_delivraptor_file_prise_en_charge_ibfk_1` FOREIGN KEY (`numBordereau`) REFERENCES `_delivraptor_colis` (`numBordereau`) ON DELETE CASCADE;
COMMIT;