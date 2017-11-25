#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	int fd_in, fd_out, bytes, n;
	unsigned char buffer, aux, aux2;

  if (argc != 4) {
		fprintf(stderr, "Forma de uso: %s numero origen destino\n", argv[0]);
		return -1;
	}

	n = atoi(argv[1]);
	if (n <= 0) {
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

	while ( (bytes = read(fd_in, &buffer, sizeof(char))) != 0 ) {
		aux = buffer<<n;
		aux2 = buffer>>n;
		buffer = aux|aux2;
		write(fd_out, &buffer, bytes);
	}


	close(fd_in);
	close(fd_out);

	return 0;
}
