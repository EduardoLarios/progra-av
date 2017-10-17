#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char *command0;
char *command1;
char *command2;
char *command3;
int arguments;

void handler(int signum) {

  int pid;
  // execlp("ls","ls", "-l" ..., (char *) 0)
  // execvp(argv[1], &argv[1])

  if ((pid = fork()) < 0) {
    perror(command0);
    exit(-1);
  } else if (pid == 0) {
  
    switch (signum) {
      // kill -s 1 pid
      case 1:
        if (arguments == 4) {
          execlp(command1, command1, (char *) 0);
        } else {
          execlp("ls", "ls", (char *) 0);
        }
        break;
      // kill -s 2 pid
      case 2:
        if (arguments == 4) {
          execlp(command2, command2, (char *) 0);
        } else {
          execlp("ps", "ps", (char *) 0);
        }
        break;
      // kill -s 3 pid
      case 3:
        if (arguments == 4) {
          execlp(command3, command3, (char *) 0);
        } else {
          execlp("pwd", "pwd", (char *) 0);
        }
        break;
    }
  
  } else {
    wait(NULL);
  }
}

int main(int argc, char *argv[]) {

  if (argc != 1 && argc != 4) {
		fprintf(stderr, "Forma de uso: %s orden1 orden2 orden3\n", argv[0]);
  	fprintf(stderr, "Forma de uso: %s\n", argv[0]);
		return -1;
  }

  command0 = argv[0];
  command1 = argv[1];
  command2 = argv[2];
  command3 = argv[3];
  arguments = argc;

  // kill -s SIGKILL pid - Kill the process
  signal(1, handler);
  signal(2, handler);
  signal(3, handler);

  while (1) {
    printf("PID %i wating for a signal\n", getpid());
    pause();
  }

  return 0;
}
