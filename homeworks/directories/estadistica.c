#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

float count = 0;
float blk = 0;
float chr = 0;
float idir = 0;
float fifo = 0;
float lnk = 0;
float reg = 0;
float sock = 0;

void show_types(char* directory, char* name, char *program) {
	struct stat ss;
	char filename[NAME_MAX + 1];

	sprintf(filename, "%s/%s", directory, name);
	if (lstat(filename, &ss) == -1) {
		perror(program);
		exit(-1);
	}

  switch (ss.st_mode & S_IFMT) {
    case S_IFBLK: blk++; break;
    case S_IFCHR: chr++; break;
    case S_IFDIR: idir++; break;
    case S_IFIFO: fifo++; break;
    case S_IFLNK: lnk++; break;
    case S_IFREG: reg++; break;
    case S_IFSOCK: sock++; break;
    default: printf("unknown?\n"); break;
  }
  count++;
}

void list(char *directory, char *program) {
	char filename[NAME_MAX + 1];
	DIR *dir;
	struct dirent *dir_entry;
	struct stat ss;
  int percentage;

	if ( (dir = opendir(directory)) == NULL ) {
		perror(program);
		exit(-1);
	}

	printf("directorio: %s:\n", directory);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}

		show_types(directory, dir_entry->d_name, program);
	}
	printf("\n");
  printf("Tipo \t Porcentaje\n");
  if (blk > 0) {
    percentage = (int)((blk / count) * 100);
    printf("BLK \t %i%%\n", percentage);
  }
  if (chr > 0) {
    percentage = (int)((chr / count) * 100);
    printf("CHR \t %i%%\n", percentage);
  }
  if (idir > 0) {
    percentage = (int)((idir / count) * 100);
    printf("DIR \t %i%%\n", percentage);
  }
  if (fifo > 0) {
    percentage = (int)((fifo / count) * 100);
    printf("FIFO \t %i%%\n", percentage);
  }
  if (reg > 0) {
    percentage = (int)((reg/count) * 100);
    printf("REG \t %i%%\n", percentage);
  }
  if (sock > 0) {
    percentage = (int)((sock / count) * 100);
    printf("SOCK \t %i%% \n", percentage);
  }
  count = 0;
  blk = 0;
  chr = 0;
  idir = 0;
  fifo = 0;
  lnk = 0;
  reg = 0;
  sock = 0;
	rewinddir(dir);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);
		stat(filename, &ss);
		if (S_ISDIR(ss.st_mode)) {
			list(filename, program);
		}
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	char dir_name[NAME_MAX + 1];
	char *directory;

	if (argc > 2) {
		fprintf(stderr, "usage: %s [directory]\n", argv[0]);
		return -1;
	}

	strcpy(dir_name, ".");
	directory = dir_name;
	if (argc == 2) {
    directory = argv[1];
	}

	list(directory, argv[0]);
	return 0;
}
