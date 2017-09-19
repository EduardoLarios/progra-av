#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
		fprintf(stderr, "Forma de uso: %s num\n", argv[0]);
		return -1;
	}
	
    return 0;
}