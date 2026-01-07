#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void recv_all(int fd, void *buf, size_t len) {
    size_t received = 0;
    while (received < len) {
        ssize_t r = recv(fd, (char *)buf + received, len - received, 0);
        if (r <= 0) die("recv");
        received += r;
    }
}

void send_prograptor(int fd, uint16_t type, const char *json) {
    uint16_t version = htons(1);
    uint16_t t = htons(type);
    uint32_t len = htonl(strlen(json));

    send(fd, &version, 2, 0);
    send(fd, &t, 2, 0);
    send(fd, &len, 4, 0);
    send(fd, json, strlen(json), 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { 
        die("socket");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        die("bind");
    }

    if (listen(server_fd, 5) < 0) {
        die("listen");
    }

    printf("prograptor prêt sur le port %d\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
    if (client_fd < 0) { 
        die("accept"); 
    }

    // --- Lecture header ---
    uint16_t version, type;
    uint32_t len;

    recv_all(client_fd, &version, 2);
    recv_all(client_fd, &type, 2);
    recv_all(client_fd, &len, 4);

    version = ntohs(version);
    type = ntohs(type);
    len = ntohl(len);

    char *json = malloc(len + 1);
    recv_all(client_fd, json, len);
    json[len] = '\0';

    printf("Reçu prograptor v%d type=%d\nJSON=%s\n", version, type, json);

    // --- Réponse ---
    if (type == 0x0001) {
        send_prograptor(
            client_fd,
            0x0001,
            "{ \"type\": \"hello\", \"from\": \"server\", \"status\": \"ok\" }"
        );
    } else {
        send_prograptor(
            client_fd,
            0x00FF,
            "{ \"error\": \"unknown type\" }"
        );
    }

    free(json);
    close(client_fd);
    close(server_fd);
    return 0;
}
