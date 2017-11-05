#include "header.h"
#include <string.h>

void serves_client(int nsfd, int ndsfd) {
	int turn = 1, game = 1, answer;
	char *nickname1, *nickname2, *play;


	write(nsfd, "Give me your nickname:", 22);
	read(nsfd, &nickname1, sizeof(nickname1));
	write(ndsfd, "Give me your nickname:", 22);
	read(ndsfd, &nickname2, sizeof(nickname2));

	// while (game) {
	// 	if (turn) {
	// 		write(nsfd, &nickname1, sizeof(nickname1));
	// 		write(nsfd, " your turn:", 12);
	// 		read(nsfd, &play, sizeof(play));
	// 		write(ndsfd, &play, sizeof(play));
	// 		write(ndsfd, &nickname2, sizeof(nickname2));
	// 		write(ndsfd, ":", 1);
	// 		read(ndsfd, &answer, sizeof(answer));
	// 		if (answer == WON) {
	// 			game = 0;
	// 		}
	// 		write(nsfd, &answer, sizeof(answer));
	// 		turn = 0;
	// 	} else {
	// 		write(nsfd, &nickname2, sizeof(nickname2));
	// 		write(ndsfd, " your turn:", 12);
	// 		read(ndsfd, &play, sizeof(play));
	// 		write(nsfd, &play, sizeof(play));
	// 		write(nsfd, &nickname1, sizeof(nickname1));
	// 		write(nsfd, ":", 11);
	// 		read(nsfd, &answer, sizeof(answer));
	// 		if (answer == WON) {
	// 			game = 0;
	// 		}
	// 		write(ndsfd, &answer, sizeof(answer));
	// 		turn = 1;
	// 	}
	// }
	// if (turn) {
	// 	write(ndsfd, 'Winner ->', sizeof(answer));
	// 	write(ndsfd, &nickname2, sizeof(nickname2));
	// 	write(nsfd, 'Winner ->', sizeof(answer));
	// 	write(nsfd, &nickname2, sizeof(nickname2));
	// } else {
	// 	write(ndsfd, 'Winner ->', sizeof(answer));
	// 	write(ndsfd, &nickname1, sizeof(nickname2));
	// 	write(nsfd, 'Winner ->', sizeof(answer));
	// 	write(nsfd, &nickname1, sizeof(nickname2));
	// }

	close(nsfd);
	close(ndsfd);
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, ndsfd, pid;
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
		printf("Connection accepted\n");
		if ( (ndsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}
		printf("Connection accepted\n");

		/* CONCURRENTE
		if ( (pid = fork()) < 0 ) {
			perror(program);
		} else if (pid == 0) {
			close(sfd);
			serves_client(nsfd);
			exit(0);
		} else {
			close(nsfd);
		}
		CONCURRENTE */

		serves_client(nsfd, ndsfd); // ITERATIVO
	}
}

int main(int argc, char* argv[]) {
	char ip[15];
	int port;

	strcpy(ip, DEFAULT_IP);
	port = DEFAULT_PORT;
	if (argc == 3) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc == 5) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
			if (strcmp(argv[3], "-p") == 0) {
				port = atoi(argv[4]);
				if (port < 5000) {
            		printf("%s: The port must be greater than 5000.\n", argv[0]);
            		return -1;
            	}
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}
			if (strcmp(argv[3], "-d") == 0) {
				strcpy(ip, argv[4]);
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc != 1) {
		printf("usage: %s [-d dir] [-p port]\n", argv[0]);
		return -1;
	}

	server(ip, port, argv[0]);

	return 0;
}
