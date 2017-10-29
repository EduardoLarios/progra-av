#include "header.h"

void a_barber(char* program) {
	int i, k, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}

	while (1) {
		printf("Barber %i is attending a client %i\n", getpid(), semctl(semid, CLIENTS, GETVAL, 0));
		sem_wait(semid, CLIENTS, 1);
		printf("Number or chairs updated.\n");
		sem_wait(semid, SHAVING_ROOM, 1);
		printf("Chair is available.\n");
		sem_signal(semid, WAITING_ROOM, 1);
		printf("Barber %i is ready\n", getpid());
		sem_signal(semid, BARBER, 1);
		printf("Chairs blocked\n");
		sem_signal(semid, SHAVING_ROOM, 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount, pid, i;

	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}

	if ( (pid = fork()) < 0 ) {
		perror(argv[0]);
		return -1;
	} else if (pid == 0) {
		a_barber(argv[0]);
	} else {
		//do_nothing
	}

	return 0;
}
