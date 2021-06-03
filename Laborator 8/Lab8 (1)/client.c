#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN], buffer2[BUFLEN];

	if (argc < 3) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
        perror("setsocketopt");
        exit(1);
    }

	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	ret = inet_aton(argv[1], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	fd_set read_fds;
	FD_SET(STDIN_FILENO, &read_fds);
	FD_SET(sockfd, &read_fds);

	int max_fd = STDIN_FILENO > sockfd ? STDIN_FILENO + 1 : sockfd + 1;

	while (1) {
  		

		fd_set tmp_read_fds = read_fds;
    	int rc = select(max_fd, &tmp_read_fds, NULL, NULL, NULL);

		DIE(rc < 0, "select");

		if (FD_ISSET(STDIN_FILENO, &tmp_read_fds)) {
			// citim din acel file descriptor
			// se citeste de la tastatura
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

			// se trimite mesaj la server
			n = send(sockfd, buffer, strlen(buffer), 0);
			DIE(n < 0, "send");
    	}

		if (FD_ISSET(sockfd, &tmp_read_fds)) {
			// citim de pe socket
			memset(buffer2, 0, BUFLEN);

			n = recv(sockfd, buffer2, sizeof(buffer2), 0);
			DIE(n < 0, "recv");

			printf("%s\n", buffer2);
		}

		if (strncmp(buffer, "exit", 4) == 0) {
			break;
		}
	}

	close(sockfd);

	return 0;
}
