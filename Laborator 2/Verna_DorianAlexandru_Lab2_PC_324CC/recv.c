#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc,char** argv){
	msg r;
	init(HOST,PORT);
	int loop = 0, file_read;
	char str[5], filename[100], message[MAX_LEN];

	if (recv_message(&r) < 0) {
		perror("Receive message");
		return -1;
	} else {
		strncpy(str, r.payload, 4);
  		str[4] = '\0';
  		// start transmission
		if (strcmp(str, "strt") == 0)  		
			loop = 1;
		// Send ACK:
		sprintf(r.payload,"%s", "ACK");
		r.len = strlen(r.payload) + 1;
		send_message(&r);
		printf("[recv] ACK sent\n");
	}

	while (loop == 1) {
		if (recv_message(&r) < 0) {
			perror("Receive message");
			return -1;
		}

		printf("[recv] Got msg with payload: <%s>, sending ACK...\n", r.payload);

  		strncpy(str, r.payload, 4);
  		str[4] = '\0';
  		printf("%s\n", str);

		if (strcmp(str, "stop") == 0) {
			// end transmission
			loop = 0;
		} else if (strcmp(str, "open") == 0) {
			// open / creating file for writting
			strcpy(filename, r.payload + 4);
			printf("%s\n", filename);
			strcat(filename, "_copied");
			file_read = open(filename, O_WRONLY | O_CREAT, 0644);
			if (file_read < 0) {
				perror("Eroare la creare fisier");
				exit(0);
			}
			lseek(file_read, 0, SEEK_SET);
		} else if (strcmp(str, "send") == 0) {
			strcpy(message, r.payload + 4);
        	int copy = write(file_read, message, strlen(message));
        	if (copy < 0)
            	perror("Eroare la scriere");
		} else if (strcmp(str, "clos") == 0) {
			int result = close(file_read);
			if (result < 0) {
				perror("Eroare la scriere");
				exit(0);
			}
		}
		// Send ACK:
		sprintf(r.payload,"%s", "ACK");
		r.len = strlen(r.payload) + 1;
		send_message(&r);
		printf("[recv] ACK sent\n");
	}
  	return 0;
}
