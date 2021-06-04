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
	// dezactivarea buffering-ului
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	// auxiliaries
	int ret, port;

	// preluarea datelor serverului
	struct sockaddr_in serv_addr_tcp, serv_addr_udp;

	// preluarea portului
	port = atoi(argv[1]);
	DIE(port == 0, "atoi");

	// socketurile pentru TCP si UDP
	int sockfdTCP, sockfdUDP;

	// crearea socket-urilor si realizarea bind-urilor
	// crearea socketului pentru UDP
	int enable = 1;
	memset((char*) &serv_addr_udp, 0, sizeof(struct sockaddr_in));
	serv_addr_udp.sin_family = AF_INET;
	serv_addr_udp.sin_port = htons(port);
	serv_addr_udp.sin_addr.s_addr = INADDR_ANY;

	// family este familia protocoalelor folosite
    sockfdUDP = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(sockfdUDP < 0, "Error creating socket");

	if (setsockopt(sockfdUDP, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
        perror("setsocketopt");
        exit(1);
    }

    DIE(sockfdUDP < 0, "socket");

	ret = bind(sockfdUDP, (struct sockaddr *) &serv_addr_udp, sizeof(struct sockaddr));
	DIE(ret < 0, "bind");

	// buffer-ul cu care citesc de la tastatura
	char buffer[200], received[200];

	while (1) {
		printf("Mesajul este: ");
		int len = sizeof(struct sockaddr_in);
		int rd = recvfrom(sockfdUDP, (char*) received, 200, MSG_WAITALL,
			(struct sockaddr *) &serv_addr_udp, &len);
		printf("%s\n", received);
	}
}
