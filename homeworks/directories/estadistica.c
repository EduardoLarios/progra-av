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

int count = 0;
int blk = 0;
int chr = 0;
int idir = 0;
int fifo = 0;
int lnk = 0;
int reg = 0;
int sock = 0;

void show_types(char* directory, char* name, char *program) {
	struct stat ss;

	if (lstat(filename, &ss) == -1) {
		perror(program);
		exit(-1);
	}

  switch (sb.st_mode & S_IFMT) {
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
  float percentage;

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

		show_types(directory, dir_entry->d_name, program);
	}
	printf("\n");
  printf("Tipo \t Porcentaje\n");
  if (blk > 0) {
    percentage = (blk / count) * 100;
    printf("BLK \t %f\n", percentage);
  }
  if (chr > 0) {
    percentage = (chr / count) * 100;
    printf("CHR \t %f\n", percentage);
  }
  if (idir > 0) {
    percentage = (idir / count) * 100;
    printf("DIR \t %f\n", percentage);
  }
  if (fifo > 0) {
    percentage = (fifo / count) * 100;
    printf("FIFO \t %f\n", percentage);
  }
  if (reg > 0) {
    percentage = (reg / count) * 100;
    printf("REG \t %f\n", percentage);
  }
  if (sock > 0) {
    percentage = (sock / count) * 100;
    printf("SOCK \t %f\n", percentage);
  }
	rewinddir(dir);
	while ( (dir_entry = readdir(dir)) != NULL ) {
		if (strcmp(dir_entry->d_name, ".") == 0 ||
			strcmp(dir_entry->d_name, "..")  == 0) {
			continue;
		}
		sprintf(filename, "%s/%s", directory, dir_entry->d_name);
		stat(filename, &ss);
		if (S_ISDIR(ss.st_mode)) {
			list(filename, recursive, program);
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
	recursive = 0;
	directory = dir_name;
	if (argc == 2) {
    directory = argv[1];
	}

	list(directory, argv[0]);
	return 0;
}
