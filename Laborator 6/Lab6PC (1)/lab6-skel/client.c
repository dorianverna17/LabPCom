/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

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
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd, sockfd, rd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];
	char aux[BUFLEN];


	/*Deschidere socket*/
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	
	/* Deschidere fisier pentru citire */
	DIE((fd=open(argv[3],O_RDONLY))==-1,"open file");
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
	memset(&to_station, 0, sizeof(to_station)); 

	to_station.sin_family = AF_INET; 
	to_station.sin_port = htons(50000); 
	to_station.sin_addr.s_addr = INADDR_ANY; 
	
	sprintf(aux, "N%s", argv[3]);
	sendto(sockfd, (const char *)aux, (strlen(argv[3]) + 1), 0,
		(const struct sockaddr*) &to_station, sizeof(to_station));

	while ((rd = read(fd, buf, BUFLEN - 1))) {
		sprintf(aux, "M%s", buf);
		sendto(sockfd, (const char *)aux, rd, 0,
		(const struct sockaddr*) &to_station, sizeof(to_station));
	}

	sprintf(aux, "END");
	sendto(sockfd, (const char *)aux, 3, 0,
		(const struct sockaddr*) &to_station, sizeof(to_station));
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din fisier
	*		trimite pe socket
	*/	


	/*Inchidere socket*/
	close(sockfd);
	
	/*Inchidere fisier*/
	close(fd);

	return 0;
}
