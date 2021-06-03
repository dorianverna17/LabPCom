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
		result = write(STDOUT_FILENO, buf, result);
		if (result < 0) {
			perror("Eroare la scriere");
			exit(0);
		}
	}
	result = close(file_read);
	if (result < 0) {
		perror("Eroare la scriere");
		exit(0);
	}
	return 0;
}