//pour compiler le programme avec de la BDD il faut faire :
//cc testBddEnC.c $(mysql_config --cflags --libs) -o testBddEnC

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Initialisation et connexion à la base
    MYSQL *conn = mysql_init(NULL);
    if (!conn) { fprintf(stderr, "mysql_init failed\n"); exit(EXIT_FAILURE); }

    if (!mysql_real_connect(conn, "localhost", "pperche", "grognasseEtCompagnie", "delivraptor", 0, NULL, 0)) {
        fprintf(stderr, "Connexion échouée : %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    // Requête
    const char *sql = "UPDATE _delivraptor_colis SET etape = 5 WHERE numBordereau = 1";
    if (mysql_query(conn, sql)) {
        fprintf(stderr, "Erreur mysql_query: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    // Récupération et affichage du résultat
    // MYSQL_RES *result = mysql_store_result(conn);
    // if (!result) {
    //     fprintf(stderr, "Erreur mysql_store_result: %s\n", mysql_error(conn));
    //     mysql_close(conn);
    //     exit(EXIT_FAILURE);
    // }

    // int num_fields = mysql_num_fields(result);
    // MYSQL_ROW row;
    // while ((row = mysql_fetch_row(result))) {
    //     for (int i = 0; i < num_fields; i++)
    //         printf("%s\t", row[i] ? row[i] : "NULL");
    //     printf("\n");
    // }

    // Libération des ressources et fermeture
    //mysql_free_result(result);
    mysql_close(conn);

    return 0;
}
