#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char* in;
char* out;
char* program;

void handler(int sig) {
  int i, random;
  FILE *file;

  srand(12345);

  switch (sig) {
    case SIGUSR1:

      file = fopen(in, "w");

      if (file == NULL) {
        perror(program);
        exit(-1);
      }
      for (i = 0; i < 1000; i++) {
        random = 1 + (rand() % 100);
        fprintf(file, "%i\n", random);
      }
      break;
    case SIGUSR2:
      break;
  }
}

void create_process() {
  int i, pid;
  for (i = 0; i < 2; i++) {
    if ( (pid = fork()) < 0 ){
      perror(program);
      exit(-1);
    } else if (pid == 0) {
      while(1) {
        printf("PID %i - PPID %i\n", getpid(), getppid());
      }
      exit(0);
    } else {
      printf("PID %i killing to PID %i\n", getpid(), pid);
      kill(pid, SIGUSR1);
    }
  }
}

int main(int argc, char *argv[]) {
  int timeout, i;

  if (argc != 4) {
		fprintf(stderr, "Forma de uso: %s timeout prefix_input prefix_output\n", argv[0]);
    return -1;
  }

  timeout = atoi(argv[1]);
  if (timeout <= 0) {
		fprintf(stderr, "Error: el programa recibe un numero entero positivo mayor a 0\n");
    return -1;
  }

  program = argv[0];
  in = argv[2];
  out = argv[3];

  for (i = 0; i < 3; i++) {
    create_process(argv[0]);
    sleep(timeout);
  }

  return 0;
}
