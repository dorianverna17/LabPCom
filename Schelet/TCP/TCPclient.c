#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*
*	Utilizare: ./client ip_server port_server
*/
int main(int argc,char**argv)
{
	// dezactivez buffering-ul
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	// pornesc socketul
	int sockfd, n, ret, flag = 0;
	struct sockaddr_in serv_addr;
	char buffer[200];

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
	// incerc conectarea la server
	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	strcpy(buffer, "DA");

	n = send(sockfd, buffer, strlen(buffer), 0);
	DIE(n < 0, "send");

	char m[5];
	n = recv(sockfd, m, 2, 0);
	printf("%s\n", m);

	/*Inchidere socket*/
	close(sockfd);

	return 0;
}