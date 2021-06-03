#ifndef LIB
#define LIB

#define MAX_LEN 1400

typedef struct {
  int len;
  char payload[MAX_LEN];
} msg;

// type == 0 -> strt - start transmission
// type == 1 -> pachet de date random trimise de sender (concatenez "send" la mesaj)
// type == 2 -> mesaj de deschidere a unui fisier trimis de sender (adaug "open" la mesaj)
// type == 3 -> mesaj de inchidere a unui fisier trimis de sender (adaug "clos" la mesaj)
// type == 4 -> stop transmission

typedef struct __attribute__((__packed__)) {
	int type;
	char payload[MAX_LEN - 4];
} message;

void init(char* remote,int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);

#endif

