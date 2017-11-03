#include "header.h"

void a_cliente(char* program, int wait_time) {
	int i, semid;
	key_t key;

	if ( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}

	if ( (semid = semget(key, 4, 0666)) < 0 ) {
		perror(program);
		exit(-1);
	}

	for (i = 0; i < 4; i++) {
		printf("Client %i trying to access the barber.\n", getpid());
		sem_wait(semid, SHAVING_ROOM, 1);
		if (semctl(semid, WAITING_ROOM, GETVAL, 0) > 0) {
			printf("Client %i accesing the barber.\n", getpid());
			sem_signal(semid, WAITING_ROOM, 1);
			printf("Client %i is in the waiting room.\n", getpid());
			sem_signal(semid, CLIENTS, 1);
			printf("Client %i is in the shaving room.\n", getpid());
			sem_signal(semid, SHAVING_ROOM, 1);
			sem_wait(semid, BARBER, 1);
			printf("Client %i is going to sleep.\n", getpid());
			sleep(wait_time);
		} else {

			printf("Barbershop is FULL.\n");
			sem_signal(semid, SHAVING_ROOM, 1);
			printf("Client %i is going to sleep.\n", getpid());
			sleep(wait_time);
		}
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount, pid, i, wait_time;

	if (argc != 3) {
		fprintf(stderr, "usage: %s numero_de_clientes periodo_de_regreso\n", argv[0]);
		return -1;
	}

	amount = atoi(argv[1]);
	if (amount < 1) {
		fprintf(stderr, "%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	wait_time = atoi(argv[2]);
	if (wait_time < 1) {
		fprintf(stderr, "%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}

	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			a_cliente(argv[0], wait_time);
		} else {
			//do_nothing
		}
	}

	return 0;
}
