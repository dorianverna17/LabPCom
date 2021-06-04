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
	if (argc!=3)
		usage(argv[0]);
	
	int fd, sockfd, rd;
	struct sockaddr_in to_station;

	/*Deschidere socket*/
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
	memset(&to_station, 0, sizeof(to_station)); 

	to_station.sin_family = AF_INET; 
	to_station.sin_port = htons(atoi(argv[2])); 
	to_station.sin_addr.s_addr = INADDR_ANY; 

	// mesajul il primesc de la tastatura si
	// il trimit la server
	char buffer[200];

	while (1) {
		sendto(sockfd, (const char *) buffer, strlen(buffer), 0,
			(const struct sockaddr*) &to_station, sizeof(to_station));
		printf("am trimis %s\n", buffer);
	}

	/*Inchidere socket*/
	close(sockfd);
	
	/*Inchidere fisier*/
	close(fd);

	return 0;
}