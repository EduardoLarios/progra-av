#include "header.h"
#include <time.h>

int mi_table[5][5];
int oponent_table[5][5];

void display(int table[5][5]) {
  int i, j;
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      if (j == 0 || i == 0) {
        printf(" %i ", table[i][j]);
      } else {
        printf("[%i]", table[i][j]);
      }
    }
    printf("\n");
  }
}
void create_tables() {
  int i, j, random1, random2;
  for (i = 1; i < 5; i++) {
    mi_table[0][i] = i;
    mi_table[i][0] = i;
    oponent_table[0][i] = i;
    oponent_table[i][0] = i;
  }
  for (i = 1; i < 5; i++) {
    for (j = 1; j < 5; j++) {
      mi_table[i][j] = 0;
      oponent_table[i][j] = 0;
    }
  }
  srand(time(NULL));
  i = 0;
  while (i < 4) {
    random1 = (rand() % 4) + 1;
    random2 = (rand() % 4) + 1;
    if (!mi_table[random1][random2]) {
      mi_table[random1][random2] = 1;
      i++;
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

  create_tables();
  display(mi_table);
  printf("\n");
  display(oponent_table);
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(argv[1]);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

  gets(&nickname);
  write(sfd, &nickname, sizeof(nickname));
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
	close(sfd);
	return 0;
}
