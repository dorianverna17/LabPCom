#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int file_read = open("fisier.txt", O_RDONLY);
	int result;
	char buf[100];
	char ch[1];
	if (file_read < 0) {
		perror("Eroare la citire");
		exit(0);
	}
	// ma pozitionez la un char distanta de sfarsitul fisierului
	int i = 1;
	int pos = lseek(file_read, -(i * sizeof(char)), SEEK_END);
	result = read(file_read, ch, sizeof(char));
	if (result < 0) {
		perror("Eroare la citire");
		exit(0);
	}
	int nr = 0;
	int first_pos;
	while (pos != SEEK_SET) {
		// in first_pos retin adresa de la care incep sa verific caracter
		// cu caracter elementele de pe fiecare linie
		if (i == 1) {
			first_pos = pos + 1;
		} else {
			first_pos = pos;
		}
		// in acest while mut variabila pos inapoi in fisier pana cand
		// ajung la un '\n'
		while (ch[0] != '\n') {
			i++;
			pos = lseek(file_read, -(i * sizeof(char)), SEEK_END);
			result = read(file_read, ch, sizeof(ch));
			if (result < 0) {
				perror("Eroare la citire");
				exit(0);
			}
			if (pos == SEEK_SET) {
				pos = lseek(file_read, -(i * sizeof(char)), SEEK_END);
				break;
			}
		}
		// daca pos ajunge la inceputul fisierului trebuie sa tratez putin
		// mai special acest caz
		if (pos != SEEK_SET)
			result = read(file_read, buf, first_pos - pos + 1);
		else
			result = read(file_read, buf, first_pos - pos + 2);
		if (result < 0) {
			perror("Eroare la citire");
			exit(0);
		}
		// scriu rezultatul la stdout
		result = write(STDOUT_FILENO, buf, result);
		if (result < 0) {
			perror("Eroare la scriere");
			exit(0);
		}
		// daca pos nu e pe inceputul fisierului, atunci fac pregatirile
		// pentru urmatoarea iteratie a while-ului - mut pos mai in spate un caracter
		if (pos != SEEK_SET) {
			i++;
			pos = lseek(file_read, -(i * sizeof(char)), SEEK_END);
			result = read(file_read, ch, sizeof(ch));
			if (result < 0) {
				perror("Eroare la scriere");
				exit(0);
			}
		}
	}
	result = close(file_read);
	if (result < 0) {
		perror("Eroare la scriere");
		exit(0);
	}
	return 0;
}