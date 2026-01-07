INSERT INTO _delivraptor_colis (
    noCommande,
    destination,
    localisation,
    etape,
    date_etape,
    contact
) VALUES (
    123456,
    '12 rue de la Paix, 35000 Rennes',
    'Entrepôt Alizon - Rennes',
    2,
    NOW(),
    FALSE
);

INSERT INTO _delivraptor_colis_historique (
    numBordereau,
    etape,
    date_etape,
    localisation
) VALUES (
    1,
    2,
    NOW(),
    'Entrepôt Alizon - Rennes'
);


INSERT INTO _delivraptor_file_prise_en_charge (
    numBordereau,
    date_entree
) VALUES (
    1,
    NOW()
);
