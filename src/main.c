// Compilation : cc main.c $(mysql_config --cflags --libs) -o delivraptor_server

#include <mysql/mysql.h>      // Bibliothèque MySQL pour accès à la base de données
#include <stdio.h>            // Entrées/sorties standard (printf, fprintf, fopen, etc.)
#include <stdlib.h>           // Fonctions standard (malloc, exit, atoi, etc.)
#include <string.h>           // Manipulation de chaînes (strlen, strcmp, strcpy, etc.)
#include <time.h>             // Gestion du temps (rand seed, timestamps, localtime)
#include <unistd.h>           // Fonctions système Unix (close, read, write, fork)
#include <arpa/inet.h>        // Conversion d'adresses IP (inet_ntoa, inet_ntop, htons, etc.)
#include <sys/socket.h>       // Programmation des sockets (socket, bind, listen, accept, etc.)
#include <ctype.h>            // Vérification/conversion de caractères (isxdigit, etc.)
#include <getopt.h>           // Parsing des options en ligne de commande (getopt_long)
#include <libgen.h>           // Manipulation de chemins (basename pour extraire nom du fichier)
#include <sys/wait.h>         // Gestion des processus fils (waitpid pour éviter les zombies)
#include <signal.h>           // Gestion des signaux (SIGCHLD pour détecter fin des processus fils)
#include <errno.h>            // Gestion des codes d'erreur (errno)


extern int errno;
MYSQL *conn = NULL;
char *global_log_file = NULL;
struct ServerConfig *global_config = NULL;

// Constantes définissant les limites de taille des buffers
#define BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 256
#define MAX_LOG_MESSAGE 512
#define PORT_SERVER_DEFAULT 8080

// Structure pour stocker l'état d'une session client
// Chaque client connecté au serveur a sa propre session associée
// Cette structure est transmise au processus fils qui gère le client
struct ClientSession {
    int client_socket;
    int authentified;
    char username[50];
    char client_ip[INET_ADDRSTRLEN];
    int client_port;
    pid_t pid;
};

// Structure pour la configuration du serveur
struct ServerConfig {
    int port;
    int capacity;
    char *auth_file;
    char *log_file;
};

// DÉCLARATIONS FORWARD
int require_auth(struct ClientSession *session);
void create(struct ClientSession *session, int commande_id, char *destination,
            struct ServerConfig config, MYSQL *conn);
void traiter_queue(MYSQL *conn, int capacity);
void liberer_file_colis(MYSQL *conn, long long bordereau);
void status(struct ClientSession *session, char *bordereau, struct ServerConfig config, MYSQL *conn);
void status_queue(struct ClientSession *session, struct ServerConfig config, MYSQL *conn);
void help(struct ClientSession *session);
void auth(struct ClientSession *session, char *username, char *password_md5, 
          char *auth_file, struct ServerConfig config, MYSQL *conn);
int check_auth(const char *auth_file, const char *username, const char *password_md5);
int is_valid_md5(const char *str);
int get_capacite_actuelle(MYSQL *conn);
long long num_bordereau_unique();

// FONCTIONS DE LOGGING

/**
 * get_timestamp() - Retourne un timestamp formaté pour les logs
 */
char* get_timestamp() {
    static char timestamp[20];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d",
             t->tm_year + 1900, 
             t->tm_mon + 1,
             t->tm_mday,
             t->tm_hour,
             t->tm_min,
             t->tm_sec);
    
    return timestamp;
}

/**
 * write_log() - Écrit un message dans le fichier de log et sur la console
 */
void write_log(const char *log_file, const char *ip, int port, 
               const char *username, const char *action, const char *message) {
    FILE *log = fopen(log_file, "a");
    if (!log) {
        perror("Erreur d'ouverture du fichier de log");
        return;
    }
    
    char prefix[100] = "";
    
    if (ip && port > 0) {
        if (username && strlen(username) > 0) {
            snprintf(prefix, sizeof(prefix), "[%s:%d] [%s]", ip, port, username);
        } else {
            snprintf(prefix, sizeof(prefix), "[%s:%d]", ip, port);
        }
    }
    
    fprintf(log, "[%s] %s %s: %s\n", 
            get_timestamp(),
            prefix,
            action,
            message);
    
    fclose(log);
    
    printf("[%s] %s %s: %s\n", 
           get_timestamp(), 
           prefix,
           action,
           message);
}

/**
 * log_server_start() - Enregistre le démarrage du serveur dans les logs
 */
void log_server_start(struct ServerConfig config, const char *server_ip) {
    char message[256];
    
    snprintf(message, sizeof(message), 
             "Serveur démarré - Port: %d, Capacité: %d, Auth: %s, Log: %s",
             config.port,
             config.capacity,
             config.auth_file,
             config.log_file);
    
    write_log(config.log_file, server_ip, config.port, 
              "SERVER", "START", message);
}

/**
 * log_server_stop() - Enregistre l'arrêt du serveur dans les logs
 */
void log_server_stop(struct ServerConfig config, const char *server_ip) {
    write_log(config.log_file, server_ip, config.port, 
              "SERVER", "STOP", "Serveur arrêté proprement");
}

// FONCTIONS BASE DE DONNÉES

/**
 * config_BD() - Initialise une nouvelle connexion à la base de données MySQL
 */
MYSQL* config_BD() {
    MYSQL *local_conn = mysql_init(NULL);
    if (!local_conn) {
        fprintf(stderr, "mysql_init failed\n");
        exit(EXIT_FAILURE);
    }

    
    if (!mysql_real_connect(local_conn, "mariadb", "sae", "grognasseEtCompagnie", "saedb", 3306, NULL, 0)) {
        fprintf(stderr, "Connexion échouée : %s\n", mysql_error(local_conn));
        mysql_close(local_conn);
        exit(EXIT_FAILURE);
    }
    
    printf("Connexion à la base de données MySQL établie\n");
    return local_conn;
}

/**
 * num_bordereau_unique() - Génère un numéro de bordereau aléatoire à 10 chiffres
 */
long long num_bordereau_unique() {
    long long num = 0;
    
    for (int i = 0; i < 10; i++) {
        num = num * 10 + (rand() % 10);
    }
    return num;
}

/**
 * get_capacite_actuelle() - Récupère le nombre de colis actuellement dans la file
 */
int get_capacite_actuelle(MYSQL *conn) {
    char query[256];
    
    snprintf(query, sizeof(query), 
             "SELECT COUNT(*) FROM _delivraptor_file_prise_en_charge");
    
    if (mysql_query(conn, query)) {
        return -1;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        return -1;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int current_load = row ? atoi(row[0]) : 0;
    
    mysql_free_result(result);
    
    return current_load;
}

/**
 * require_auth() - Vérifie que le client est authentifié
 */
int require_auth(struct ClientSession *session) {
    if(!session->authentified) {
        char response[] = "ERROR NOT_AUTHENTICATED\n";
        send(session->client_socket, response, strlen(response), 0);
        return 0;
    }
    return 1;
}

/**
 * create() - Traite la commande de création de bordereau CREATE
 * Version avec file d'attente
 */
void create(struct ClientSession *session, int commande_id, char *destination,
            struct ServerConfig config, MYSQL *conn) {
    if(!require_auth(session)) {
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "CREATE", "Tentative sans authentification");
        return;
    }

    char query[512];
    char response[BUFFER_SIZE];
    
    // Échapper les chaînes
    char escaped_destination[256];
    
    mysql_real_escape_string(conn, escaped_destination, destination, strlen(destination));
    
    // 1. Vérifier si un bordereau existe déjà pour cette commande
    snprintf(query, sizeof(query), 
             "SELECT numBordereau FROM _delivraptor_colis WHERE noCommande = %d", 
             commande_id);
    
    if (mysql_query(conn, query)) {
        snprintf(response, sizeof(response), "ERROR DB_QUERY\n");
        send(session->client_socket, response, strlen(response), 0);
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (result && mysql_num_rows(result) > 0) {
        MYSQL_ROW row = mysql_fetch_row(result);
        snprintf(response, sizeof(response), "BORDEREAU %s\n", row[0]);
        send(session->client_socket, response, strlen(response), 0);
        mysql_free_result(result);
        
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Bordereau existant: %s pour commande %d", row[0], commande_id);
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "CREATE", log_msg);
        return;
    }
    mysql_free_result(result);
    
    // 2. Vérifier la capacité actuelle
    int current_load = get_capacite_actuelle(conn);
    if (current_load < 0) {
        snprintf(response, sizeof(response), "ERROR DB_QUERY_CAPACITY\n");
        send(session->client_socket, response, strlen(response), 0);
        return;
    }
    
    // 3. Si capacité disponible, créer immédiatement
    if (current_load < config.capacity) {
        long long new_bordereau = num_bordereau_unique();
        
        // Insérer le colis
        snprintf(query, sizeof(query),
                 "INSERT INTO _delivraptor_colis(numBordereau, noCommande, destination, localisation, etape, date_etape) "
                 "VALUES (%lld, %d, '%s', 'Entrepôt Alizon', 1, NOW())",
                 new_bordereau, commande_id, escaped_destination);
        
        if (mysql_query(conn, query)) {
            snprintf(response, sizeof(response), "ERROR DB_INSERT\n");
            send(session->client_socket, response, strlen(response), 0);
            return;
        }
        
        // Insérer dans la file de prise en charge
        snprintf(query, sizeof(query),
                 "INSERT INTO _delivraptor_file_prise_en_charge (numBordereau, date_entree) VALUES (%lld, NOW())",
                 new_bordereau);
        
        if (mysql_query(conn, query)) {
            snprintf(response, sizeof(response), "ERROR DB_INSERT_FILE\n");
            send(session->client_socket, response, strlen(response), 0);
            
            // Rollback
            snprintf(query, sizeof(query), "DELETE FROM _delivraptor_colis WHERE numBordereau = %lld", new_bordereau);
            mysql_query(conn, query);
            return;
        }
        
        snprintf(response, sizeof(response), "%lld\n", new_bordereau);
        send(session->client_socket, response, strlen(response), 0);
        
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Nouveau bordereau %lld créé pour commande %d", new_bordereau, commande_id);
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "CREATE", log_msg);
    } 
    // 4. Si capacité pleine, mettre en file d'attente
    else {
        // Insérer dans la queue d'attente
        snprintf(query, sizeof(query),
                 "INSERT INTO _delivraptor_queue (noCommande, destination, username) "
                 "VALUES (%d, '%s', '%s')",
                 commande_id, escaped_destination, session->username);
        
        if (mysql_query(conn, query)) {
            snprintf(response, sizeof(response), "ERROR DB_QUEUE_INSERT\n");
            send(session->client_socket, response, strlen(response), 0);
            return;
        }
        
        snprintf(response, sizeof(response), "QUEUED %d\n", commande_id);
        send(session->client_socket, response, strlen(response), 0);
        
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), 
                 "Commande %d mise en file d'attente (capacité: %d/%d)", 
                 commande_id, current_load, config.capacity);
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "CREATE", log_msg);
    }
}

/**
 * traiter_queue() - Traite les commandes en attente quand de la capacité se libère
 */
void traiter_queue(MYSQL *conn, int capacity) {
    int current_load = get_capacite_actuelle(conn);
    if (current_load < 0 || current_load >= capacity) {
        return;
    }
    
    int places_libres = capacity - current_load;
    
    char query[512];
    
    snprintf(query, sizeof(query),
             "SELECT id, noCommande, destination, username "
             "FROM _delivraptor_queue "
             "WHERE traite = 0 "
             "ORDER BY date_creation ASC "
             "LIMIT %d",
             places_libres);
    
    if (mysql_query(conn, query)) {
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        return;
    }
    
    int traites = 0;
    MYSQL_ROW row;
    
    while ((row = mysql_fetch_row(result))) {
        int queue_id = atoi(row[0]);
        int commande_id = atoi(row[1]);
        char *destination = row[2];
        char *username = row[3];
        
        long long new_bordereau = num_bordereau_unique();
        
        char escaped_destination[256];
        mysql_real_escape_string(conn, escaped_destination, destination, strlen(destination));
        
        snprintf(query, sizeof(query),
                 "INSERT INTO _delivraptor_colis(numBordereau, noCommande, destination, localisation, etape, date_etape) "
                 "VALUES (%lld, %d, '%s', 'Entrepôt Alizon', 1, NOW())",
                 new_bordereau, commande_id, escaped_destination);
        
        if (mysql_query(conn, query)) {
            continue;
        }
        
        snprintf(query, sizeof(query),
                 "INSERT INTO _delivraptor_file_prise_en_charge (numBordereau, date_entree) VALUES (%lld, NOW())",
                 new_bordereau);
        
        if (mysql_query(conn, query)) {
            snprintf(query, sizeof(query), "DELETE FROM _delivraptor_colis WHERE numBordereau = %lld", new_bordereau);
            mysql_query(conn, query);
            continue;
        }
        
        snprintf(query, sizeof(query),
                 "UPDATE _delivraptor_queue SET traite = 1 WHERE id = %d",
                 queue_id);
        mysql_query(conn, query);
        
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), 
                 "Commande %d traitée depuis queue -> bordereau %lld", 
                 commande_id, new_bordereau);
        write_log(global_log_file, "0.0.0.0", 0, username, "QUEUE", log_msg);
        
        traites++;
    }
    
    mysql_free_result(result);
    
    if (traites > 0) {
        printf("[Queue] %d commandes traitées depuis la file d'attente\n", traites);
    }
}

/**
 * liberer_file_colis() - Retire un colis de la file de prise en charge
 */
void liberer_file_colis(MYSQL *conn, long long bordereau) {
    char query[256];
    
    snprintf(query, sizeof(query),
             "DELETE FROM _delivraptor_file_prise_en_charge WHERE numBordereau = %lld",
             bordereau);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur libération file: %s\n", mysql_error(conn));
    } else {
        printf("Colis %lld libéré de la file (passage étape 4->5)\n", bordereau);
        
        if (global_config) {
            traiter_queue(conn, global_config->capacity);
        }
    }
}

// FONCTIONS AUTHENTIFICATION

/**
 * is_valid_md5() - Vérifie si une chaîne a un format MD5 valide
 */
int is_valid_md5(const char *str) {
    if (strlen(str) != 32) {
        return 0;
    }
    
    for (int i = 0; i < 32; i++) {
        if (!isxdigit(str[i]))
            return 0;
    }
    return 1;
}

/**
 * check_auth() - Vérifie les identifiants d'un utilisateur contre un fichier
 */
int check_auth(const char *auth_file, const char *username, const char *password_md5) {
    FILE *file = fopen(auth_file, "r");
    if(!file) {
        printf("Impossible d'ouvrir le fichier d'authentification\n");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int authentified = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (line[0] == '#' || strlen(line) == 0) {
            continue;
        }

        char *colon = strchr(line, ':');
        if (!colon) {
            continue;
        }

        *colon = '\0';
        char *file_user = line;
        char *file_password_md5 = colon + 1;

        if(strcmp(file_user, username) == 0 && strcmp(file_password_md5, password_md5) == 0) {
            authentified = 1;
            break;
        }
    }

    fclose(file);
    return authentified;
}

/**
 * auth() - Traite la commande d'authentification AUTH
 */
void auth(struct ClientSession *session, char *username, char *password_md5, 
          char *auth_file, struct ServerConfig config, MYSQL *conn) {
    char reponse[BUFFER_SIZE];

    if(session->authentified) {
        snprintf(reponse, sizeof(reponse), "ERROR ALREADY_AUTHENTICATED\n");
        send(session->client_socket, reponse, strlen(reponse), 0);
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "AUTH", "Tentative d'authentification alors que déjà authentifié");
        return;
    }

    if(!username || !password_md5) {
        snprintf(reponse, sizeof(reponse), "ERROR MISSING_CREDENTIALS\n");
        send(session->client_socket, reponse, strlen(reponse), 0);
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  NULL, "AUTH", "Identifiants manquants dans la requête");
        return;
    }

    if (!is_valid_md5(password_md5)) {
        snprintf(reponse, sizeof(reponse), "ERROR INVALID_MD5_FORMAT\n");
        send(session->client_socket, reponse, strlen(reponse), 0);
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  NULL, "AUTH", "Format MD5 invalide");
        return;
    }

    if(check_auth(auth_file, username, password_md5)) {
        session->authentified = 1;
        strncpy(session->username, username, sizeof(session->username)-1);
        session->username[sizeof(session->username)-1] = '\0';
        snprintf(reponse, sizeof(reponse), "AUTH_SUCCESS\n");
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "AUTH", "Authentification réussie");
    } else {
        snprintf(reponse, sizeof(reponse), "ERROR AUTH_FAILED\n");
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  NULL, "AUTH", "Échec d'authentification - identifiants invalides");
    }

    send(session->client_socket, reponse, strlen(reponse), 0);
}


/**
 * status() - Traite la commande de consultation de statut STATUS
 */
void status(struct ClientSession *session, char *bordereau, struct ServerConfig config, MYSQL *conn) {
    if(!require_auth(session)) {
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "STATUS", "Tentative sans authentification");
        return;
    }

    char query[256];
    char response[BUFFER_SIZE];

    snprintf(query, sizeof(query),
             "SELECT etape, localisation, date_etape, livraison_type, refus_raison, photo_path "
             "FROM _delivraptor_colis WHERE numBordereau = %s", bordereau);
    
    if (mysql_query(conn, query)) {
        snprintf(response, sizeof(response), "ERROR DB_QUERY\n");
        send(session->client_socket, response, strlen(response), 0);
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "STATUS", bordereau);
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        snprintf(response, sizeof(response), "ERROR NOT_FOUND\n");
        send(session->client_socket, response, strlen(response), 0);
        if (result) mysql_free_result(result);
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "STATUS", bordereau);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    
    snprintf(response, sizeof(response), 
             "ETAPE %s|%s|%s|%s|%s\n",
             row[0] ? row[0] : "",
             row[1] ? row[1] : "",
             row[2] ? row[2] : "",
             row[3] ? row[3] : "",
             row[4] ? row[4] : "");
    
    send(session->client_socket, response, strlen(response), 0);
    
    write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "STATUS", bordereau);
    // Vérifier si besoin de libérer la file (étape 4 -> 5)
    if (row[0] && atoi(row[0]) == 5) {
        long long num_bordereau = atoll(bordereau);
        liberer_file_colis(conn, num_bordereau);
        
        write_log(config.log_file, session->client_ip, session->client_port,
                  session->username, "STATUS", bordereau);
    }
    
    // Gestion de l'image pour l'étape 9
    if (row[0] && atoi(row[0]) == 9 && row[5] && strlen(row[5]) > 0) {
        FILE *img_file = fopen(row[5], "rb");
        if (img_file) {
            send(session->client_socket, "---IMAGE---\n", 12, 0);
            
            char buffer[1024];
            size_t bytes_read;
            
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), img_file)) > 0) {
                send(session->client_socket, buffer, bytes_read, 0);
            }
            fclose(img_file);
            
            write_log(config.log_file, session->client_ip, session->client_port,
                      session->username, "STATUS", bordereau);
        }
    }
    
    mysql_free_result(result);
}


/**
 * status_queue() - Affiche l'état de la file d'attente
 */
void status_queue(struct ClientSession *session, struct ServerConfig config, MYSQL *conn) {
    if(!require_auth(session)) {
        return;
    }
    
    char query[256];
    char response[BUFFER_SIZE * 4]; // Plus grand buffer pour liste
    
    snprintf(query, sizeof(query),
             "SELECT COUNT(*) as total, "
             "SUM(CASE WHEN traite = 0 THEN 1 ELSE 0 END) as en_attente "
             "FROM _delivraptor_queue");
    
    if (mysql_query(conn, query)) {
        snprintf(response, sizeof(response), "ERROR DB_QUERY_QUEUE\n");
        send(session->client_socket, response, strlen(response), 0);
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        snprintf(response, sizeof(response), "ERROR DB_RESULT_QUEUE\n");
        send(session->client_socket, response, strlen(response), 0);
        return;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int total = row[0] ? atoi(row[0]) : 0;
    int en_attente = row[1] ? atoi(row[1]) : 0;
    int traites = total - en_attente;
    
    mysql_free_result(result);
    
    // Récupérer les détails des commandes en attente
    snprintf(query, sizeof(query),
             "SELECT noCommande, destination, date_creation "
             "FROM _delivraptor_queue "
             "WHERE traite = 0 "
             "ORDER BY date_creation ASC");
    
    if (mysql_query(conn, query)) {
        snprintf(response, sizeof(response), "ERROR DB_QUERY_DETAILS\n");
        send(session->client_socket, response, strlen(response), 0);
        return;
    }
    
    result = mysql_store_result(conn);
    if (!result) {
        snprintf(response, sizeof(response), "ERROR DB_RESULT_DETAILS\n");
        send(session->client_socket, response, strlen(response), 0);
        return;
    }
    
    // Construire la réponse
    snprintf(response, sizeof(response),
             "QUEUE_STATUS\n"
             "Capacité maximale: %d\n"
             "Colis actifs: %d\n"
             "Commandes totales: %d\n"
             "  - Traitées: %d\n"
             "  - En attente: %d\n\n"
             "File d'attente (FIFO):\n",
             config.capacity,
             get_capacite_actuelle(conn),
             total, traites, en_attente);
    
    int pos = strlen(response);
    int compteur = 1;
    
    while ((row = mysql_fetch_row(result))) {
        char ligne[256];
        snprintf(ligne, sizeof(ligne), "%d. %s -> %s (%s)\n",
                 compteur++, row[0], row[1], row[2]);
        
        // Vérifier si on dépasse la taille du buffer
        if (pos + strlen(ligne) < sizeof(response) - 1) {
            strcat(response + pos, ligne);
            pos += strlen(ligne);
        }
    }
    
    mysql_free_result(result);
    
    if (en_attente == 0) {
        strcat(response, "(file d'attente vide)\n");
    }
    
    send(session->client_socket, response, strlen(response), 0);
}

/**
 * help() - Traite la commande HELP du protocole
 */
void help(struct ClientSession *session) {
    char *help_text = 
        "╔══════════════════════════════════════════════════════════════╗\n"
        "║               PROTOCOLE DÉLIVRAPTOR - COMMANDES              ║\n"
        "╚══════════════════════════════════════════════════════════════╝\n\n"
        "AUTHENTIFICATION (requise pour les autres commandes):\n"
        "  AUTH <username> <md5_password>\n"
        "  Exemple: AUTH alizon e10adc3949ba59abbe56e057f20f883e\n\n"
        "GESTION DES COLIS:\n"
        "  CREATE <commande_id>      Crée un nouveau bordereau de livraison\n"
        "  STATUS <bordereau>        Consulte l'état d'un colis\n\n"
        "INFORMATIONS:\n"
        "  HELP                      Affiche cette aide\n"
        "  QUIT                      Déconnexion propre\n"
        "  EXIT                      Alias de QUIT\n\n"
        "FORMAT DES RÉPONSES:\n"
        "  • CREATE   → BORDEREAU <num>\n"
        "  • STATUS   → ETAPE <num>|<localisation>|<date>|<type>|<raison>\n"
        "  • AUTH     → AUTH_SUCCESS ou ERROR AUTH_FAILED\n"
        "  • ERREURS  → ERROR <type> (CAPACITE, NOT_FOUND, DB_QUERY, etc.)\n\n"
        "NOTES:\n"
        "  • Toutes les commandes sont en MAJUSCULES\n"
        "  • Le hash MD5 doit être 32 caractères hexadécimaux\n"
        "  • L'authentification est valide pour toute la session\n"
        "  • Une commande = une ligne terminée par \\n\n"
        "╔══════════════════════════════════════════════════════════════╗\n"
        "║                 Service Délivraptor - v1.0                   ║\n"
        "╚══════════════════════════════════════════════════════════════╝\n";
    
    send(session->client_socket, help_text, strlen(help_text), 0);
}

/**
 * print_help() - Affiche l'aide en ligne du serveur
 */
void print_help(const char *prog_name) {
    const char *base_name = basename((char *)prog_name);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              SERVEUR DÉLIVRAPTOR - R3.05 SAÉ 3               ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
    
    printf("DESCRIPTION:\n");
    printf("  Simulateur de transport et de livraison de colis pour Alizon.\n");
    printf("  Gère la prise en charge, le suivi et la livraison des colis.\n\n");
    
    printf("UTILISATION:\n");
    printf("  %s [OPTIONS]\n\n", prog_name);
    
    printf("OPTIONS:\n");
    printf("  -p, --port PORT        Port TCP d'écoute (1-65535)\n");
    printf("                          Défaut: 8080\n");
    printf("                          Exemple: -p 9090 ou --port 9090\n\n");
    
    printf("  -c, --capacity CAP     Capacité maximale de la file de livraison\n");
    printf("                          Nombre de colis simultanés en étape 1-4\n");
    printf("                          Défaut: 5\n");
    printf("                          Exemple: -c 10 ou --capacity 10\n\n");
    
    printf("  -a, --auth FILE        Fichier d'authentification des clients\n");
    printf("                          Format: username:md5hash (un par ligne)\n");
    printf("                          Défaut: /etc/delivraptor/auth.txt\n");
    printf("                          Exemple: -a /home/user/auth.txt\n\n");
    
    printf("  -l, --log FILE         Fichier de logs du serveur\n");
    printf("                          Défaut: /var/log/delivraptor.log\n");
    printf("                          Exemple: -l /tmp/debug.log\n\n");
    
    printf("  -h, --help             Affiche cette aide et quitte\n\n");
    
    printf("EXEMPLES D'UTILISATION:\n");
    printf("  %s -p 8080 -c 5 -a auth.txt\n", base_name);
    printf("    Lance le serveur sur le port 8080 avec capacité 5\n\n");
    
    printf("  %s --port 9090 --capacity 10 --auth /etc/delivraptor/auth.txt\n", base_name);
    printf("    Lance le serveur sur le port 9090 avec capacité 10\n\n");
    
    printf("  %s --help\n", base_name);
    printf("    Affiche ce message d'aide\n\n");
    
    printf("NOTES:\n");
    printf("  • Le serveur nécessite une connexion MySQL/MariaDB fonctionnelle\n");
    printf("  • Le fichier d'authentification doit être au format username:md5hash\n");
    printf("  • Pour générer un hash MD5 : echo -n 'motdepasse' | md5sum\n");
    printf("  • Les logs incluent date, IP client et actions effectuées\n");
    printf("  • Utilisez Ctrl+C pour arrêter le serveur\n");
    
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              IUT Lannion - Département Informatique          ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}


/**
 * gerer_client() - Fonction exécutée par chaque processus fils pour gérer un client
 */
void gerer_client(struct ClientSession session, struct ServerConfig config) {
    MYSQL *conn_client = config_BD();
    if (!conn_client) {
        close(session.client_socket);
        exit(EXIT_FAILURE);
    }
    
    write_log(config.log_file, session.client_ip, session.client_port,
              NULL, "CONNECT", "Nouveau processus fils pour gestion client");
    
    int connection_active = 1;
    
    while (connection_active) {
        char buffer[BUFFER_SIZE];
        
        ssize_t bytes_read = read(session.client_socket, buffer, sizeof(buffer)-1);
        
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                write_log(config.log_file, session.client_ip, session.client_port,
                          session.username, "DISCONNECT", "Client déconnecté proprement");
            } else {
                write_log(config.log_file, session.client_ip, session.client_port,
                          session.username, "DISCONNECT", "Erreur de lecture socket");
            }
            break;
        }
        
        buffer[bytes_read] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0;
        
        char *token = strtok(buffer, " \n");
        
        if (token) {
            if (strcmp(token, "AUTH") == 0) {
                char *username = strtok(NULL, " \n");
                char *password_md5 = strtok(NULL, " \n");
                auth(&session, username, password_md5, config.auth_file, config, conn_client);
            }
            
            else if (strcmp(token, "CREATE") == 0) {
                char *commande_id_str = strtok(NULL, " \n");
                char *destination = strtok(NULL, "\n");
                
                if (commande_id_str && destination) {
                    while (*destination == ' ') {
                        destination++;
                    }
                    
                    int commande_id = atoi(commande_id_str);
                    
                    if (commande_id <= 0) {
                        char response[] = "ERROR INVALID_COMMANDE_ID\n";
                        send(session.client_socket, response, strlen(response), 0);
                        write_log(config.log_file, session.client_ip, session.client_port,
                                session.username, "CREATE", "ID de commande invalide");
                    } else {
                        create(&session, commande_id, destination, config, conn_client);
                    }
                } else {
                    char response[] = "ERROR MISSING_PARAMETERS\n";
                    send(session.client_socket, response, strlen(response), 0);
                    write_log(config.log_file, session.client_ip, session.client_port,
                            session.username, "CREATE", "Paramètres manquants (commande_id ou destination)");
                }
            }
            
            else if (strcmp(token, "QUEUE_STATUS") == 0) {
                status_queue(&session, config, conn_client);
            }
            
            else if (strcmp(token, "STATUS") == 0) {
                char *bordereau = strtok(NULL, " \n");
                if (bordereau) {
                    status(&session, bordereau, config, conn_client);
                } else {
                    char response[] = "ERROR MISSING_BORDEREAU\n";
                    send(session.client_socket, response, strlen(response), 0);
                    write_log(config.log_file, session.client_ip, session.client_port,
                              session.username, "STATUS", "Bordereau manquant");
                }
            }
            
            else if (strcmp(token, "HELP") == 0) {
                help(&session);
                write_log(config.log_file, session.client_ip, session.client_port,
                          session.username, "HELP", "Commande HELP exécutée");
            }
            
            else if (strcmp(token, "QUIT") == 0 || strcmp(token, "EXIT") == 0) {
                char response[] = "BYE\n";
                send(session.client_socket, response, strlen(response), 0);
                connection_active = 0;
                write_log(config.log_file, session.client_ip, session.client_port,
                          session.username, "QUIT", "Déconnexion demandée par client");
            }
            
            else {
                char response[] = "ERROR UNKNOWN_COMMAND\n";
                send(session.client_socket, response, strlen(response), 0);
                write_log(config.log_file, session.client_ip, session.client_port,
                          session.username, "ERROR", token);
            }
        }
    }
    
    mysql_close(conn_client);
    close(session.client_socket);
    
    write_log(config.log_file, session.client_ip, session.client_port,
              session.username, "EXIT", "Processus fils terminé");

    exit(EXIT_SUCCESS);
}