/**
* To compile: gcc root.c -o root -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define SIZE 100

double array[SIZE];

void square_root() {
  double i;

  for (i = 0; i < 100; i++) {
    array[i] = sqrt(i);
  }
};

void* task(void* param) {
  int thread = *( (int *) param );
  int i;
  if (thread) {
    square_root();
  } else {
    printf("Wait for the result\n");
    if (array[SIZE - 1]) {
      for (i = 0; i < 100; i++) {
        printf("[ %d ]\n", array[i]);
      }
    }
  }

	return ( (void*) 0);
}


int main(int arg, char* argv[]) {
	pthread_t pthread_id[2];
	void* ret;
  int t = 1, t2 = 0;
  char message[] = "Wait for the result";

	printf("PID = %i - creating threads\n", getpid());
  pthread_create(&pthread_id[0], NULL, task, (void *) &t);
  pthread_create(&pthread_id[1], NULL, task, (void *) &t2);

	pthread_join(pthread_id[0], &ret);
	printf("PID = %i - all threads are terminated\n", getpid());

	return 0;
}
