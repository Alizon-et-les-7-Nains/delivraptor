// Fichier de test pour la base de données Delivraptor
// Compilation : cc test_db.c $(mysql_config --cflags --libs) -o test_db
// Exécution : ./test_db

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

MYSQL *conn;

#define MAX_CAPACITY 100

void config_BD() {
    // Initialisation et connexion à la base
    conn = mysql_init(NULL);
    if (!conn) { 
        fprintf(stderr, "mysql_init failed\n"); 
        exit(EXIT_FAILURE); 
    }

    if (!mysql_real_connect(conn, "localhost", "pperche", "grognasseEtCompagnie", "delivraptor", 0, NULL, 0)) {
        fprintf(stderr, "Connexion échouée : %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
    printf("✓ Connexion à la base de données réussie\n\n");
}

void test_insert_colis() {
    printf("=== TEST 1 : Insertion de colis ===\n");
    
    int numBordereau = 987654321;
    int noCommande = 654321;
    
    char query[512];
    snprintf(query, sizeof(query), 
        "INSERT INTO _delivraptor_colis (numBordereau, noCommande, destination, localisation, etape, date_etape, contact) "
        "VALUES (%d, %d, 'Quartier de la Fontaine, 75003 Paris', 'Entrepôt Paris Centre', 1, NOW(), FALSE)",
        numBordereau, noCommande);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "✗ Erreur insertion colis: %s\n", mysql_error(conn));
    } else {
        printf("✓ Colis inséré avec succès (numBordereau: %d, noCommande: %d)\n\n", numBordereau, noCommande);
    }
}

void test_capacity_check() {
    printf("=== TEST 2 : Vérification de capacité ===\n");
    
    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM _delivraptor_colis");
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "✗ Erreur requête capacité: %s\n", mysql_error(conn));
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "✗ Erreur mysql_store_result: %s\n", mysql_error(conn));
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int current_count = atoi(row[0]);
    mysql_free_result(result);
    
    printf("Nombre de colis actuels: %d / %d\n", current_count, MAX_CAPACITY);
    
    if (current_count >= MAX_CAPACITY) {
        printf("✗ File PLEINE - Création refusée\n\n");
    } else {
        printf("✓ File disponible - %d places restantes\n\n", MAX_CAPACITY - current_count);
    }
}

void test_get_bordereau() {
    printf("=== TEST 3 : Récupération de bordereau ===\n");
    
    int noCommande = 654321;
    char query[256];
    
    snprintf(query, sizeof(query), 
        "SELECT numBordereau, etape, destination, localisation FROM _delivraptor_colis WHERE noCommande = %d",
        noCommande);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "✗ Erreur requête bordereau: %s\n", mysql_error(conn));
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "✗ Erreur mysql_store_result: %s\n", mysql_error(conn));
        return;
    }
    
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(result))) {
        printf("✓ Bordereau trouvé :\n");
        printf("  - Numéro: %s\n", row[0]);
        printf("  - Étape: %s\n", row[1]);
        printf("  - Destination: %s\n", row[2]);
        printf("  - Localisation: %s\n\n", row[3]);
    } else {
        printf("✗ Aucun bordereau trouvé pour noCommande = %d\n\n", noCommande);
    }
    
    mysql_free_result(result);
}

void test_update_etape() {
    printf("=== TEST 4 : Mise à jour d'étape ===\n");
    
    int numBordereau = 987654321;
    int new_etape = 3;
    
    char query[256];
    snprintf(query, sizeof(query), 
        "UPDATE _delivraptor_colis SET etape = %d, date_etape = NOW() WHERE numBordereau = %d",
        new_etape, numBordereau);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "✗ Erreur mise à jour: %s\n", mysql_error(conn));
    } else {
        printf("✓ Étape mise à jour (numBordereau: %d, nouvelle étape: %d)\n\n", numBordereau, new_etape);
    }
}

void test_historique() {
    printf("=== TEST 5 : Insertion dans l'historique ===\n");
    
    int numBordereau = 987654321;
    int etape = 3;
    
    char query[512];
    snprintf(query, sizeof(query), 
        "INSERT INTO _delivraptor_colis_historique (numBordereau, etape, date_etape, localisation) "
        "VALUES (%d, %d, NOW(), 'Entrepôt Paris Centre')",
        numBordereau, etape);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "✗ Erreur insertion historique: %s\n", mysql_error(conn));
    } else {
        printf("✓ Historique inséré (numBordereau: %d, étape: %d)\n\n", numBordereau, etape);
    }
}

void test_get_historique() {
    printf("=== TEST 6 : Récupération de l'historique ===\n");
    
    int numBordereau = 987654321;
    char query[256];
    
    snprintf(query, sizeof(query), 
        "SELECT etape, date_etape, localisation FROM _delivraptor_colis_historique WHERE numBordereau = %d ORDER BY date_etape",
        numBordereau);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "✗ Erreur requête historique: %s\n", mysql_error(conn));
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "✗ Erreur mysql_store_result: %s\n", mysql_error(conn));
        return;
    }
    
    printf("✓ Historique du colis %d :\n", numBordereau);
    MYSQL_ROW row;
    int count = 0;
    while ((row = mysql_fetch_row(result))) {
        count++;
        printf("  %d. Étape %s - %s - %s\n", count, row[0], row[1], row[2]);
    }
    printf("\n");
    
    mysql_free_result(result);
}

void test_file_prise_en_charge() {
    printf("=== TEST 7 : Insertion dans file prise en charge ===\n");
    
    int numBordereau = 987654321;
    char query[256];
    
    snprintf(query, sizeof(query), 
        "INSERT INTO _delivraptor_file_prise_en_charge (numBordereau, date_entree) VALUES (%d, NOW())",
        numBordereau);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "✗ Erreur insertion file: %s (peut être déjà existante)\n", mysql_error(conn));
    } else {
        printf("✓ Colis ajouté à la file de prise en charge\n\n");
    }
}

void test_duplicate_commande() {
    printf("=== TEST 8 : Vérification contrainte unicité noCommande ===\n");
    
    int numBordereau = 111111111;
    int noCommande = 654321; // Même noCommande que le premier test
    
    char query[512];
    snprintf(query, sizeof(query), 
        "INSERT INTO _delivraptor_colis (numBordereau, noCommande, destination, localisation, etape, date_etape, contact) "
        "VALUES (%d, %d, 'Test', 'Test', 1, NOW(), FALSE)",
        numBordereau, noCommande);
    
    if (mysql_query(conn, query)) {
        printf("✓ Insertion refusée (contrainte d'unicité respectée) : %s\n\n", mysql_error(conn));
    } else {
        printf("✗ Insertion réussie (ATTENTION : contrainte non respectée)\n\n");
    }
}

int main() {
    srand(time(NULL));
    config_BD();
    
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║          TESTS DE LA BASE DE DONNÉES DELIVRAPTOR           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    test_insert_colis();
    test_capacity_check();
    test_get_bordereau();
    test_update_etape();
    test_historique();
    test_get_historique();
    test_file_prise_en_charge();
    test_duplicate_commande();
    
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                   TESTS TERMINÉS                           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    mysql_close(conn);
    return 0;
}
