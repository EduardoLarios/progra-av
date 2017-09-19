#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1000

int values [SIZE];

void read_values(char * filename, char * program) {
    FILE * file;
    int times, intersections, size, i;
    float num;
    
    file = fopen(filename, "r");
    
    if (file) {
        perror(program);
        exit(-1);
    }
    
    memset(values, 0, SIZE * sizeof(int));
    
    while(fscanf(file, "%i,%i", &times, &intersections)) {
        // Cuando no se cuantos numeros voy a leer
        // while (fscanf(file, ",%f", &num)) {
            
        // }
        // Cuando se cuantos numeros voy a leer
        size = intersections * intersections;
        for (i = 0; i < size; i++) {
            fscanf(file, ",%f", &num);
            values[((int) num)]++;
        }
    }
    
    fclose(file);
    
    
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return -1;
    }
    
    read_values(argv[1], argv[0]);
    // display_histogram();
    
    return 0;
}