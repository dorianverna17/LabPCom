#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  message m;

  // Sending begin transmission signal to receiver
  printf("[send] Sending begin signal to receiver...\n");

  m.type = 0;
  char* text = malloc(4);
  strcpy(text, "strt");
  
  sprintf(t.payload, "%s", text);
  t.len = strlen(t.payload) + 1;
  send_message(&t);

  // Check response:
  if (recv_message(&t) < 0) {
    perror("Receive error ...");
    return -1;
  } else {
    printf("[send] Got reply with payload: %s\n", t.payload);
  }
  free(text);

  // Sending file name to receiver
  printf("[send] Sending filename to receiver...\n");
  strcpy(m.payload, argv[1]);
  m.type = 2;

  text = malloc(strlen(m.payload) + 4);
  strcpy(text, "open");
  strcat(text, argv[1]);
  
  sprintf(t.payload, "%s", text);
  t.len = strlen(t.payload) + 1;
  send_message(&t);

  // Check response:
  if (recv_message(&t) < 0) {
    perror("Receive error ...");
    return -1;
  } else {
    printf("[send] Got reply with payload: %s\n", t.payload);
  }
  free(text);

  // acum trimit fiecare linie din fisier

  int file_read = open(argv[1], O_RDONLY);
  int result;
  char buf[MAX_LEN - 4];
  if (file_read < 0) {
    perror("Eroare la citire");
    exit(0);
  }
  lseek(file_read, 0, SEEK_SET);
  while ((result = read(file_read, buf, sizeof(buf)))) {
    if (result < 0) {
      perror("Eroare la citire");
      exit(0);
    }
    printf("[send] Sending message to receiver...\n");
    m.type = 1;
    strcpy(m.payload, buf);
    text = malloc(strlen(m.payload) + 4);
    strcpy(text, "send");
    strcat(text, m.payload);  
    
    sprintf(t.payload, "%s", text);
    t.len = result;
    send_message(&t);    

    if (result < 0) {
      perror("Eroare la scriere");
      exit(0);
    }
    if (recv_message(&t) < 0) {
      perror("Receive error ...");
      return -1;
    } else {
      printf("[send] Got reply with payload: %s\n", t.payload);
    }
  }
  // acum trimit close message pentru inchiderea fisierului
  printf("[send] Sending close file message to receiver...\n");
  result = close(file_read);
  if (result < 0) {
    perror("Eroare la scriere");
    exit(0);
  }

  m.type = 3;
  strcpy(m.payload, "clos");  

  sprintf(t.payload, "%s", m.payload);
  t.len = strlen(t.payload) + 1;
  send_message(&t);

  if (recv_message(&t) < 0) {
    perror("Receive error ...");
    return -1;
  } else {
    printf("[send] Got reply with payload: %s\n", t.payload);
  }

  printf("[send] Sending end transmission message to receiver...\n");
  // end transmission
  m.type = 0;
  text = malloc(4);
  strcpy(text, "stop");
  
  sprintf(t.payload, "%s", text);
  t.len = strlen(t.payload) + 1;
  send_message(&t);

  if (recv_message(&t) < 0) {
    perror("Receive error ...");
    return -1;
  } else {
    printf("[send] Got reply with payload: %s\n", t.payload);
  }
  
  return 0;
}
