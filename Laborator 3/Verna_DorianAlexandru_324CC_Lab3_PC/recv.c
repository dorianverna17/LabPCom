#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

char parity(my_payload m) {
	char sum = 0;
	for (int i = 0; i < strlen(m.payload); i++)
		sum ^= m.payload[i];
	return sum;
}

int main(void)
{
	msg r;
	int i, res;
	my_payload message;
	char computed_parity;
	
	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);
	
	for (i = 0; i < COUNT; i++) {
		/* wait for message */
		res = recv_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Receive error. Exiting.\n");
			return -1;
		} else {
			// Confirming having received the message
			printf("%s -> ", r.payload);
			message.parity = r.payload[strlen(r.payload) - 1];
			strncpy(message.payload, r.payload, strlen(r.payload) - 1);
			computed_parity = parity(message);
			if ((computed_parity ^ message.parity) != 0)
				printf("Message received is corrupted\n");
			else
				printf("Message received is OK\n");
		}
		
		/* send dummy ACK */
		res = send_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}
	}

	printf("[RECEIVER] Finished receiving..\n");
	return 0;
}
