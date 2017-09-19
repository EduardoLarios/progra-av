#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void file_name(char* directory, char* name, char* program, char* word) {
	int i;
	char filename[NAME_MAX + 1];
	char str[512];
	struct stat ss;
	FILE * fp;
	
	sprintf(filename, "%s/%s", directory, name);	
	if (lstat(filename, &ss) == -1) {
		perror(program);
		exit(-1);
	}
	
	fp = fopen(name, "r");
	while(fgets(str, 512, fp) != NULL) {
		if((strstr(str, word)) != NULL) {
			printf(" %s\n", name);
		}
	}
	fclose(fp);
}

void find(char* directory, char* program, char* word) {
	char filename[NAME_MAX + 1];
    DIR *dir;
	struct dirent *dir_entry;
	struct stat ss;
    
    if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}
	
	printf("%s:\n", directory);
	
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		
		file_name(directory, dir_entry->d_name, program, word);
	}
	printf("\n");
	rewinddir(dir);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);	
		stat(filename, &ss);
		if (S_ISDIR(ss.st_mode)) {
			find(filename, program, word);
		}
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;
	char *word;
	
    if (argc > 3 || argc == 1) {
		fprintf(stderr, "usage: %s word [directory]\n", argv[0]);
		return -1;
	}
	strcpy(dir_name, ".");
	directory = dir_name;
	if (argc == 3) {
	    directory = argv[2];
	}
	find(directory, argv[0], argv[1]);
	return 0;
}
