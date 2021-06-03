/*
 * Protocoale de comunicatii
 * Laborator 7 - TCP
 * Echo Server
 * client.c
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "helpers.h"

void run_client(int sockfd) {
    char buf[BUFLEN];
    memset(buf, 0, BUFLEN);

    int byte_count;

    while (read(STDIN_FILENO, buf, BUFLEN - 1) > 0 && !isspace(buf[0])) {
        byte_count = strlen(buf) + 1;

        int bytes_send;
        int bytes_remaining = byte_count;
        int bytes_received;

        // TODO 4: Trimiteti mesajul catre server prin sockfd

        while (byte_count > 0) {
            bytes_send = send(sockfd, buf, byte_count, 0);
            byte_count -= bytes_send;
        }
        
        memset(buf, 0, BUFLEN);
       
        // TODO 5: Receptionati un mesaj venit de la server
        bytes_received = recv(sockfd, buf, BUFLEN, 0);

        fprintf(stdout, "Received: %s", buf);

        memset(buf, 0, BUFLEN);
    }
}

void run_client2(int sockfd) {
    char buf[BUFLEN];
    memset(buf, 0, BUFLEN);

    int byte_count;

    do {
        byte_count = strlen(buf) + 1;

        int bytes_send;
        int bytes_remaining = byte_count;
        int bytes_received;
       
        // TODO 5: Receptionati un mesaj venit de la server
        bytes_received = recv(sockfd, buf, BUFLEN, 0);

        fprintf(stdout, "Received: %s", buf);

        memset(buf, 0, BUFLEN);

        byte_count = read(STDIN_FILENO, buf, BUFLEN);
        // if (byte_count <= 0 || isspace(buf[0])) {
        //     break;
        // }

        // TODO 4: Trimiteti mesajul catre server prin sockfd
        bytes_send = send(sockfd, buf, byte_count, 0);
        
        memset(buf, 0, BUFLEN);
    } while (1);
}

int main(int argc, char* argv[])
{
    int sockfd = -1;
    struct sockaddr_in serv_addr;
    socklen_t socket_len = sizeof(struct sockaddr_in);
    memset(&serv_addr, 0, socket_len);

    if (argc != 4) {
        printf("\n Usage: %s <ip> <port> <type>\n", argv[0]);
        return 1;
    }

    // TODO 1: Creati un socket TCP pentru conectarea la server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
        perror("setsocketopt");
        exit(1);
    }
    // TODO 2: Completati in serv_addr adresa serverului, familia de adrese si portul pentru conectare
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    inet_aton(argv[1], &addr.sin_addr);


    // TODO 3: Creati conexiunea catre server
    int c = connect(sockfd, (struct sockaddr*) &addr, sizeof(addr));

    if (argv[3][0] == '1') {
        run_client(sockfd);
    }
    else {
        run_client2(sockfd);
    }
    

    // TODO 6: Inchideti conexiunea si socketul creat
    close(sockfd);

    return 0;
}
