// Fichier du code source principal de l'application Delivraptor

//pour compiler le programme avec de la BDD il faut faire :
//cc testBddEnC.c $(mysql_config --cflags --libs) -o testBddEnC

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MYSQL *conn;

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

int main() {
    // Configuration de la base de données
    config_BD();

    // ################################################
    // #                  Etape 1                     #
    // ################################################

    // Exemple avec un numéro de commande spécifique (envoyé par le client)
    int noCommande = 123; 

    // Tableau pour stocker la requête SQL 255 caractères + 1 pour le caractère null
    char query[256];

    //  Requête pour vérifier l'existence d'un bordereau
    // snprintf ecrit la requête SQL dans le buffer query elle sert à ne pas depasser la taille du buffer
    snprintf(query, sizeof(query), "SELECT numBordereau FROM _delivraptor_colis WHERE noCommande = %d", noCommande);

    // Récupération et affichage du résultat
    query = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Erreur mysql_store_result: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    // int num_fields = mysql_num_fields(result);
    // MYSQL_ROW row;
    // while ((row = mysql_fetch_row(result))) {
    //     for (int i = 0; i < num_fields; i++)
    //         printf("%s\t", row[i]);
    //     printf("\n");
    // }

    mysql_close(conn);

    return 0;
}
