#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "helpers.h"

int main(int argc, char *argv[]) {
	int sockfd, n;
	struct sockaddr_in serv_addr;

	int enable = 1, ret;

	memset((char *) &serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// family este familia protocoalelor folosite
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    DIE(sockfd < 0, "Error creating socket");

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
        perror("setsocketopt");
        exit(1);
    }

	DIE(sockfd < 0, "socket");

	ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");

	ret = listen(sockfd, 2);
	DIE(ret < 0, "listen");

	char buffer[200];

	struct sockaddr_in client_address;
	socklen_t addr_size = sizeof(struct sockaddr_in);

	int tcp = accept(sockfd, (struct sockaddr *)
		&client_address, &addr_size);

	n = recv(tcp, buffer, sizeof(buffer), 0);
	DIE(n < 0, "recv");
	printf("Am primit mesajul: %s\n", buffer);
	char message[5];
	strcpy(message, "OK");
	n = send(tcp, message, strlen(message), 0);
	DIE(n < 0, "send");

	/*Inchidere socket*/
	close(sockfd);

	return 0;
}
