#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

#define SPEED 10
#define DELAY 10

char parity(my_payload m) {
	char sum = 0;
	for (int i = 0; i < strlen(m.payload); i++)
		sum ^= m.payload[i];
	return sum;
}

int main(int argc, char *argv[])
{
	msg t;
	int res;
	my_payload message;
	
	printf("[SENDER] Starting.\n");	
	init(HOST, PORT);

	/* printf("[SENDER]: BDP=%d\n", atoi(argv[1])); */
	int frames = (SPEED * DELAY * 1000) / (sizeof(t) * 8);
	printf("%d\n", frames);

	// auxiliary in which I count the frames sent until now
	int count = 0;

	while (count < frames) {
		/* cleanup msg */
		memset(&t, 0, sizeof(msg));

		// initializez un mesaj cu payload-ul
		strcpy(t.payload, "Acesta este un mesaj barosanesc");
		// Here I compose message my_payload
		strcpy(message.payload, t.payload);
		// now I compute the parity
		message.parity = parity(message);
		// now that I have the parity I can remodify t.payload
		// putting parity as its last char
		t.len = strlen(t.payload);
		t.payload[t.len] = message.parity;
		t.payload[t.len + 1] = '\0';
		
		/* send msg */
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		} else {
			count++;
		}
	}

	while (count < COUNT) {
		/* wait for ACK */
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		} else {
			/* cleanup msg */
			memset(&t, 0, sizeof(msg));
			
			// initializez un mesaj cu payload-ul
			strcpy(t.payload, "Acesta este un mesaj barosanesc");
			// Here I compose message my_payload
			strcpy(message.payload, t.payload);
			// now I compute the parity
			message.parity = parity(message);
			// now that I have the parity I can remodify t.payload
			// putting parity as its last char
			t.len = strlen(t.payload);
			t.payload[t.len] = message.parity;
			t.payload[t.len + 1] = '\0';
		
			/* send msg */
			res = send_message(&t);
			if (res < 0) {
				perror("[SENDER] Send error. Exiting.\n");
				return -1;
			} else {
				count++;
			}		
		}
	}

	printf("[SENDER] Job done, all sent.\n");
		
	return 0;
}
