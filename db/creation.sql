CREATE TABLE delivraptor_auth (
    username VARCHAR(50) PRIMARY KEY,
    u_password CHAR(32) NOT NULL
);


CREATE TABLE delivraptor_colis (
    numBordereau INT AUTO_INCREMENT PRIMARY KEY,
    noCommande INT NOT NULL UNIQUE,
    destination VARCHAR(255) NOT NULL,
    localisation VARCHAR(255),
    etape INT NOT NULL CHECK (etape BETWEEN 1 AND 9),
    date_etape DATETIME NOT NULL,

    -- Étape 9
    livraison_type ENUM('MAINS_PROPRES', 'ABSENT', 'REFUSE'),
    refus_raison VARCHAR(255),
    photo_path VARCHAR(255)
);


CREATE TABLE delivraptor_colis_historique (
    id INT AUTO_INCREMENT PRIMARY KEY,
    numBordereau INT NOT NULL,
    etape INT NOT NULL CHECK (etape BETWEEN 1 AND 9),
    date_etape DATETIME NOT NULL,
    localisation VARCHAR(255),
    FOREIGN KEY (numBordereau) REFERENCES colis(numBordereau)
);


CREATE TABLE delivraptor_file_prise_en_charge (
    numBordereau INT PRIMARY KEY,
    date_entree DATETIME NOT NULL,
    FOREIGN KEY (numBordereau) REFERENCES colis(numBordereau)
);
