#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define SIZE 1000

int values[SIZE];
int x_max;
struct stat st = {0};

void find_max() {
	int i;
	i = SIZE;
	while (values[i] == 0) {
		i--;
	}
	x_max = i;
}

float normalization(float val) {
	return val/x_max;
}

void convert(char* directory, char* name, char* program, char* new_directory) {
	FILE *file; 
	FILE *new_file;
	int time, intersections, size, i;
	float num;
	char filename[NAME_MAX + 1], new_filename[NAME_MAX + 1];
	
	sprintf(filename, "%s/%s", directory, name);
	sprintf(new_filename, "%s/%s", new_directory, name);
	
	file = fopen(filename, "r");
	new_file = fopen(new_filename, "w");
	
	if (file == NULL) {
		perror(program);
		exit(-1);
	}
	
	if (new_file == NULL) {
		perror(program);
		exit(-1);
	}
	
	while ( fscanf(file, "%i,%i", &time, &intersections) != EOF ) {
		fprintf(new_file, "%i,%i", time, intersections);
		size = intersections * intersections;
		for (i = 0; i < size; i++) {
			fscanf(file, ",%f", &num);
			fprintf(new_file, ",%f", normalization(num));
			// fprintf(stdout, "%f\n", normalization(num));
		}
		fprintf(new_file, "%d\n", rand() % 100);
	}
	
	fclose(file);
	fclose(new_file);
}

void read_values(char* directory, char* name, char* program) {
	FILE *file;
	int time, intersections, size, i;
	float num;
	char filename[NAME_MAX + 1];
	
	sprintf(filename, "%s/%s", directory, name);
	file = fopen(filename, "r");
	if (file == NULL) {
		perror(program);
		exit(-1);
	}
	
	memset(values, 0, SIZE * sizeof(int));
	
	while ( fscanf(file, "%i,%i", &time, &intersections) != EOF ) {
		size = intersections * intersections;
		for (i = 0; i < size; i++) {
			fscanf(file, ",%f", &num);
			values[ ((int) num) ]++;
		}
	}
	
	fclose(file);
}

void open_directory(char* directory, char* program, int read, char* new_directory) {
	DIR *dir;
	struct dirent *dir_entry;
	
	if( (dir = opendir(directory)) == NULL) {
		perror(program);
		exit(-1);
	}
	
	
	while ( (dir_entry = readdir(dir)) != NULL) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		
		if (read) {
			read_values(directory, dir_entry->d_name, program);
		} else {
			if (stat(new_directory, &st) == -1) {
				mkdir(new_directory, 0700);
			}
			convert(directory, dir_entry->d_name, program, new_directory);
		}
	}
	
	closedir(dir);
}

int main (int argc, char* argv[]) {
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s [directory] [new directory]\n", argv[0]);
		return -1;
	}
	
	open_directory(argv[1], argv[0], 1, NULL);
	find_max();
	open_directory(argv[1], argv[0], 0, argv[2]);
	
	return 0;
}
