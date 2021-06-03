/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd, sockfd, rd;

	if (argc!=2)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];


	/*Deschidere socket*/
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	
	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
	memset(&my_sockaddr, 0, sizeof(my_sockaddr)); 
	memset(&from_station, 0, sizeof(from_station)); 
	
	// Filling server information 
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	my_sockaddr.sin_port = htons(50000); 
	
	/* Legare proprietati de socket */
	if ( bind(sockfd, (const struct sockaddr *)&my_sockaddr, 
			sizeof(my_sockaddr)) < 0 ) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}

	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
   		perror("setsocketopt");
   		exit(1);
 	}

	
	
	/* Deschidere fisier pentru scriere */
	// DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	
	socklen_t len = sizeof(from_station);

	while ((rd = recvfrom(sockfd, (char*) buf, BUFLEN, MSG_WAITALL,
		(struct sockaddr *) &from_station, &len))) {
			printf("%s\n", buf);
			if (buf[0] == 'N') {
				printf("caz 0\n\n");
				fd=open(buf,O_WRONLY|O_CREAT|O_TRUNC,0644);
				memset(buf, 0, BUFLEN);
			}
			else if (buf[0] == 'M') {
				printf("caz 1\n\n");
				write(fd, buf + 1, rd - 1);
				memset(buf, 0, BUFLEN);
			}
			else if (buf[0] == 'E' && buf[1] == 'N' && buf[2] == 'D') {
				printf("caz 2\n\n");
				memset(buf, 0, BUFLEN);
				close(fd);
			}
			else {
				printf("caz 3\n\n");
			}
	}
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/


	/*Inchidere socket*/	
	close(sockfd);
	
	/*Inchidere fisier*/
	close(fd);

	return 0;
}
