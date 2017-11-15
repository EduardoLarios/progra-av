/**
* To compile: gcc pizza.c -lpthread
* El problema se trata de un producer-consumer.
**/

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t more_pizza = PTHREAD_COND_INITIALIZER;
pthread_cond_t pizza_available = PTHREAD_COND_INITIALIZER;

int size = 0;

void* all_night_long_pizza(void *arg) {
	int slices;
  slices = *( (int *) arg );
	printf("All night long pizza starting...\n");
	while (1) {
  	pthread_mutex_lock(&mutex);
  	if (size != 0) {
     	pthread_cond_wait(&more_pizza, &mutex);
  	}
  	printf("All night long pizza send pizza...\n");
		size += slices;
		pthread_cond_signal(&pizza_available);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void* student(void *arg) {
	int i,v;
	printf("student starting...\n");
  while (1) {
    pthread_mutex_lock(&mutex);
    if (size == 0) {
      pthread_cond_wait(&pizza_available, &mutex);
    }
    size--;
    printf("student getting a slice...\n");
    pthread_cond_signal(&more_pizza);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int i, rebanadas, estudiantes;

  if (argc != 3) {
  	fprintf(stderr, "Forma de uso: %s num_estudiantes num_rebanadas\n", argv[0]);
    return -1;
  }

  rebanadas = atoi(argv[2]);
  estudiantes = atoi(argv[1]);
  if (rebanadas <= 0 || estudiantes <= 0) {
    fprintf(stderr, "Error: el programa recibe un numero entero positivo mayor a 0\n");
    return -1;
  }

  pthread_t all_night_long_pizza_thread;
	pthread_t student_thread[estudiantes];

  pthread_create(&all_night_long_pizza_thread, NULL, all_night_long_pizza, (void *) &rebanadas);
  for (i = 0; i < estudiantes; i++) {
    pthread_create(&student_thread[i], NULL, student, NULL);
  }

  for (i = 0; i < estudiantes; i++) {
    pthread_join(student_thread[i], NULL);
  }
  return 0;
}
