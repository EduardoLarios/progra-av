#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h>


int main(int argc, char *argv[]){
    FILE *file;
    int fd_in, fd_out, i, size;
    unsigned char *buffer;
    unsigned char aux;

    if (argc != 3){
        printf("usage: %s source destination\n", argv[0]);
        return -1;
    }

    if ((fd_in = open(argv[1], O_RDONLY)) < 0 ) {
        printf("decrypt: : %s No such file or directory.\n", argv[1]);
        return -1;
    }
    
    if ( (fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
        perror(argv[0]);
        return -1;
    }


    file = fopen(argv[1], "r");
    fseek(file, 0, SEEK_END);
    size = ftell(file);

    buffer = (unsigned char*) malloc(sizeof(unsigned char) * size);
    read(fd_in, buffer, size);

    for (i = 0; i < size; i++){
        aux = buffer[i];
        buffer[i] = ( aux << 4 ) | ( aux >> 4 );  
    }

    write(fd_out, buffer, size);

    free(buffer);
    close(fd_in);
    close(fd_out);
    printf("Done\n");

    return 0;
}
