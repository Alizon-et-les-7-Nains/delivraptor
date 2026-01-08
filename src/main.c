// Fichier du code source principal de l'application Delivraptor
// Ajouter les fonctions du protocole de communication ici

//pour compiler le programme avec de la BDD il faut faire :
//cc testBddEnC.c $(mysql_config --cflags --libs) -o testBddEnC

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MYSQL *conn;

#define MAX_CAPACITY 100

void config_BD() {
    // Initialisation et connexion à la base
    conn = mysql_init(NULL);
    if (!conn) { fprintf(stderr, "mysql_init failed\n"); exit(EXIT_FAILURE); }

    if (!mysql_real_connect(conn, "localhost", "pperche", "grognasseEtCompagnie", "delivraptor", 0, NULL, 0)) {
        fprintf(stderr, "Connexion échouée : %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
}

long long num_bordereau_unique() {
    char numBandereau[11];
    for (int i = 0; i < 10; i++) {
        int n = rand() % 10;
        sprintf(&numBandereau[i], "%d", n);
    }
    numBandereau[10] = '\0';

    return atoll(numBandereau);
}

int main() {
    srand(time(NULL));
    // Configuration de la base de données
    config_BD();

    // ################################################
    // #                  Etape 1                     #
    // ################################################

    // Exemple avec un numéro de commande spécifique (envoyé par le client)
    int noCommande = 1234056; 

    // Tableau pour stocker la requête SQL 255 caractères + 1 pour le caractère null
    char query[256];

    //  Requête pour vérifier l'existence d'un bordereau
    // snprintf ecrit la requête SQL dans le buffer query elle sert à ne pas depasser la taille du buffer
    snprintf(query, sizeof(query), "SELECT numBordereau FROM _delivraptor_colis WHERE noCommande = %d", noCommande);


    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur mysql_query: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Erreur mysql_store_result: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }    

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
       for (int i = 0; i < num_fields; i++) {
            printf("%s\t", row[i]);
            printf("\n");
        }
    }

    // Verification si aucun bordereau n'a été trouvé
    if (mysql_num_rows(result) == 0) {
        printf("Aucun bordereau trouvé pour le numéro de commande %d\n", noCommande);
        
        // Vérification de la capacité de la file de livraison
        char check_capacity[256];
        snprintf(check_capacity, sizeof(check_capacity), "SELECT COUNT(*) FROM _delivraptor_colis");
        
        if (mysql_query(conn, check_capacity)) {
            fprintf(stderr, "Erreur mysql_query capacité: %s\n", mysql_error(conn));
            mysql_close(conn);
            exit(EXIT_FAILURE);
        }
        
        MYSQL_RES *capacity_result = mysql_store_result(conn);
        if (!capacity_result) {
            fprintf(stderr, "Erreur mysql_store_result capacité: %s\n", mysql_error(conn));
            mysql_close(conn);
            exit(EXIT_FAILURE);
        }
        
        MYSQL_ROW capacity_row = mysql_fetch_row(capacity_result);
        int current_count = atoi(capacity_row[0]);
        mysql_free_result(capacity_result);
        
        // Vérification si la file est pleine
        if (current_count >= MAX_CAPACITY) {
            printf("ERREUR : La file de livraison est pleine (capacité : %d colis). Création refusée.\n", MAX_CAPACITY);
            mysql_free_result(result);
            mysql_close(conn);
            return EXIT_FAILURE;
        }
        
        long long num_bordereau_unique = num_bordereau_unique();

        // BD mise à jour pour insérer le nouveau bordereau 
        // etat = etape 1 | horodatage
        char insert_bd_colis[256];

        snprintf(insert_bd_colis, sizeof(insert_bd_colis), "INSERT INTO _delivraptor_colis(numBordereau, noCommande, destination, localisation, etape, date_etape) VALUES (%lld, %d, %s, %s, 1, NOW(), %s)", num_bordereau_unique, noCommande, "'Destination Exemple'", "'Localisation Exemple'", "'Contact Exemple'");
        
        if (mysql_query(conn, insert_bd_colis)) {
            fprintf(stderr, "Erreur mysql_query insertion colis: %s\n", mysql_error(conn));
            mysql_close(conn);
            exit(EXIT_FAILURE);
        }
        
        // Affichage du numéro de bordereau généré
        printf("Numéro de bordereau généré : %lld\n", num_bordereau_unique);
    } else {
        printf("Bordereau trouvé pour le numéro de commande %d\n", noCommande);
        // Envoyer num bordereau au client avec mon super protocole
    }

    mysql_close(conn);

    return 0;
}
