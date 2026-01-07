#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024


int main(void)
{
    srand(time(NULL));
    int sock;
    int socketClient;
    int ret;
    int size;
    char buffer[BUF_SIZE];

    struct sockaddr_in addr;
    struct sockaddr_in conn_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("socket() créé : fd = %d\n", sock);

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sock);
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("bind() sur 127.0.0.1:8080\n");


    ret = listen(sock, 1);
    if (ret == -1) {
        perror("listen");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("listen() : en attente d'une connexion...\n");

    size = sizeof(conn_addr);
    socketClient = accept(sock, (struct sockaddr *)&conn_addr, (socklen_t *)&size);
    if (socketClient == -1) {
        perror("accept");
        close(sock);
        exit(EXIT_FAILURE);
    }


    printf("En attente du message du client...\n");
    write(socketClient, "Entrez votre numero de commande : \n", strlen("Entrez votre numero de commande : \n"));

    printf("Réponse envoyée au client\n");
        ret = read(socketClient, buffer, BUF_SIZE - 1);
        if (ret == -1) {
            perror("read");
            close(socketClient);
            close(sock);
            exit(EXIT_FAILURE);
        }
    buffer[ret] = '\0';
    printf("Numero de commande reçu : %s", buffer);

    const char *message = "Message bien reçu par le serveur !\n";
    write(socketClient, message, strlen(message));

    write(socketClient, "En attente du numero de bordereau : \n", strlen("En attente du numero de bordereau : \n"));
    
    char numBandereau[12];
    for (int i = 0; i < 10; i++) {
        int n = rand() % 10;
        sprintf(&numBandereau[i], "%d", n);
    }
    numBandereau[10] = '\n';
    numBandereau[11] = '\0';

    write(socketClient, numBandereau , strlen(numBandereau));

    close(socketClient);
    close(sock);

    printf("Connexion fermée, serveur arrêté.\n");

    return 0;
}