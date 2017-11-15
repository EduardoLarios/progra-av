#include "header.h"
#include <string.h>

#define SIZE 100

char ruta[SIZE];

void* serves_client(void *param) {
	int nsfd = *( (int*) param);

  printf("PID = %i\n", getpid());
  printf("%s\n", ruta);
	close(nsfd);
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, pid;
	pthread_t thread_id;
	struct sockaddr_in server_info, client_info;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}

	listen(sfd, 1);
	while (1) {
		int len = sizeof(client_info);
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}

		pthread_create(&thread_id, NULL, serves_client, ((void *) &nsfd));
	}
}

int main(int argc, char* argv[]) {

	char ip[15];
  int port;

  strcpy(ip, DEFAULT_IP);
  port = DEFAULT_PORT;
	if (argc != 2) {
    printf("Usage: %s [dir]\n", argv[0]);
		return -1;
	}

  ruta = argv[1];

	server(ip, port, argv[0]);

	return 0;
}
