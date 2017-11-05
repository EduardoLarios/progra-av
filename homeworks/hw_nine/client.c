#include "header.h"
#include <time.h>

int mi_table[4][4];
int oponent_table[4][4];

void display(int table[4][4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      printf("[%i]", table[i][j]);
    }
    printf("\n");
  }
}
void create_table() {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      mi_table[i][j] = 0;
      oponent_table[i][j] = 0;
    }
  }
}

int main(int argc, char* argv[]) {
  int sfd, game = 1, answer;
	int port;
	struct sockaddr_in server_info;
  char* nickname, play;

	if (argc != 3) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -1;
	}

	port = atoi(argv[2]);
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}

  create_table();
  display(mi_table);
  printf("\n");
  display(oponent_table);
	// if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	// 	perror(argv[0]);
	// 	return -1;
	// }
  //
	// server_info.sin_family = AF_INET;
	// server_info.sin_addr.s_addr = inet_addr(argv[1]);
	// server_info.sin_port = htons(port);
	// if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
	// 	perror(argv[0]);
	// 	return -1;
	// }

  printf("Give me your nick name: ");
  gets(&nickname);
  // write(sfd, &nickname, sizeof(nickname));
  // while (game) {
  //
  //   gets(&play)
  //   write(sfd, &play, sizeof(play));
  //
  //   read(sfd, &answer, sizeof(answer));
	// 	switch (answer) {
	// 		case HIT: printf("You sank a ship.\n"); break;
	// 		case NOTHING: printf("Nothing happend.\n"); break;
	// 		default     : printf("You win\n"); game = 1; break;
	// 	}
	// }
	// close(sfd);
	return 0;
}
