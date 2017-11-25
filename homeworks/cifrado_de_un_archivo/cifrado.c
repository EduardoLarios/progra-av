#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define SIZE 128

int main(int argc, char* argv[]) {
	int fd_in, fd_out, bytes;
	char buffer[SIZE];
	
	int fd_three;
	unsigned long int lol;
    
    if (argc != 4) {
		fprintf(stderr, "Forma de uso: %s numero origen destino\n", argv[0]);
		return -1;
	}
	
	if (atoi(argv[1]) <= 0) {
		fprintf(stderr, "Error: el primer parámetro debe ser un numero entero positivo.\n");
	    return -1;
	}
	
	if ( (fd_in = open(argv[2], O_RDONLY)) < 0 ) {
		fprintf(stderr, "Error: el archivo %s no existe.\n", argv[2]);
		return -1;
	}
	
	if ( (fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (fd_three = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	lseek(fd_in, atoi(argv[1]) * -1, SEEK_END);
	while ( (bytes = read(fd_in, buffer, SIZE * sizeof(char))) != 0 ) {
		write(fd_out, buffer, bytes);
	}
	
	lseek(fd_in, 0, SEEK_SET);
	while ( (bytes = read(fd_in, buffer, SIZE * sizeof(char))) != 0 ) {
		write(fd_out, buffer, bytes);
	}
	
	
	close(fd_in);
	close(fd_out);
	
	return 0;
}