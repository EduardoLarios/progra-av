/**
* To compile: gcc root.c -o root -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

void square_root() {
  printf("Finished\n");
};

void* task(void* param) {
  int thread = *( (int *) param );
  if (thread) {
    square_root();
  } else {
    printf("Wait for the result\n");
  }

	return ( (void*) 0);
}


int main(int arg, char* argv[]) {
	pthread_t pthread_id[2];
	void* ret;
  int t = 1;
  char message[] = "Wait for the result";

	printf("PID = %i - creating threads\n", getpid());
  pthread_create(&pthread_id[0], NULL, task, (void *) &t);
  t = 0;
  pthread_create(&pthread_id[1], NULL, task, (void *) &t);

	pthread_join(pthread_id[0], &ret);
	printf("PID = %i - all threads are terminated\n", getpid());

	return 0;
}
