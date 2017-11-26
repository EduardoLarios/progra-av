#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void rotation(char* directory, char* name, char* program, int number, int i) {
	FILE *file;
	FILE *new_file;
	char filename[NAME_MAX + 1], new_filename[NAME_MAX + 1], character;
	int j;

	for (j = 0; j < i; j++)
	{
		if (i == 0) sprintf(filename, "%s/%s.log", directory, name);
		else sprintf(filename, "%s/%s.%i", directory, name, j);
		sprintf(new_filename, "%s/%s.%i", directory, name, j + 1);

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

		while ( fscanf(file, "%c", &character) != EOF ) fprintf(new_file, "%c", character);

		if (i != number)
		{
		    fclose(file);
	    	fclose(new_file);
			rotation(directory, name, program, number, ++i);
		} else {
	    	fclose(file);
		    fclose(new_file);
			remove(new_filename);
			sprintf(new_filename, "%s/%s.log", directory, name);
			new_file = fopen(new_filename, "w");
		}
	}
}

void open_directory(char* directory, char* program, char* l, int t, int number) {
	DIR *dir;
	struct dirent *dir_entry;
	char filename[NAME_MAX + 1];
	int exists = 0;

	if( (dir = opendir(directory)) == NULL) {
		perror(program);
		exit(-1);
	}


	sprintf(filename, "%s.log", l);
	while ( (dir_entry = readdir(dir)) != NULL) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0)
			continue;

		if (strcmp(dir_entry->d_name, filename) == 0) exists = 1;
	}

	if (exists)
	{
 		sleep(t);
		rotation(directory, l, program, number, 0);
	} else fprintf(stderr, "Error: el archivo %s no existe\n", l);

	closedir(dir);
}

int main(int argc, char const *argv[]) {
	int number, t;
	if (argc != 5)
	{
		fprintf(stderr, "Forma de uso: %s directorio_log archivo_log numero tiempo\n", argv[0]);
		return -1;
	}

	number = atoi(argv[3]);
	t = atoi(argv[4]);
	if (number <= 0 || t <= 0)
	{
		fprintf(stderr, "Error: el programa recibe un numero entero positivo mayor a 0\n");
		return -1;
	}

	open_directory(argv[1], argv[0], argv[2], t, number);

	return 0;
}
