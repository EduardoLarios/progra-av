#include "header.h"

void error(char *e_msg){
	printf("Error: %s\n", e_msg);
}

void menu() {
  printf("Un mensaje de envio de archivo [get] [ruta]\n");
  printf("Un mensaje de desplegado de directorio [list] [ruta]\n");
  printf("Un mensaje de envio de archivo [bye]\n");
}

void send_msg(int sfd, short code, char *msg){
	long msg_length = sizeof(msg);
	write(sfd, &code, sizeof(code));
	write(sfd, &msg_length, sizeof(msg_length));
	write(sfd, msg,msg_length);
}

void getting_file (int sfd, char *msg) {
	short code;
	long msg_length;
	char file [100]={}, aux[100];
	char *response, *fname;
	int fd_out, nbytes;
	send_msg(sfd, FILE_ROUTE, msg);
	read(sfd, &code, sizeof(code));
	read(sfd, &msg_length, sizeof(msg_length));
	read(sfd, response, msg_length);
	switch (code) {
		case ((short)SENDING_FILE):
			fname = &msg[0];
			while(strstr(fname,"/") != NULL){
				fname = &fname[1];
			}
			strncpy(file, fname, strlen(fname));
			if ( (fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
				printf("Not possible to write file\n");
				return ;
			}
			send_msg(sfd,code,"NULL");
			do{
				memset(aux, '\0', sizeof(aux));
				read(sfd, &code, sizeof(code));
				read(sfd, &msg_length, sizeof(msg_length));
				read(sfd, &aux, msg_length);
				if(aux[0] == '\0'){
					close(fd_out);
					break;
				}
				nbytes = (int)msg_length;
				write(fd_out, &aux[0], sizeof(aux[0]));
				send_msg(sfd, code, "NULL");
			} while(1);
			return;
			break;
		case ((short)DENIED):
			error(response);
			return;
			break;
		case  ((short)FILE_NOT_FOUND):
			error(response);
			return;
			break;
		case  ((short)CMD_UNKNOWN):
			error(response);
			return;
			break;
	}
}

void list_directory (int sfd, char *msg) {
	short code;
	long msg_length;
	char *response;
	char aux[100];
	send_msg(sfd, DIRECTORY_ROUTE, msg);
	read(sfd, &code, sizeof(code));
	read(sfd, &msg_length, sizeof(msg_length));
	read(sfd, response, msg_length);
	switch (code) {
		case (short)SENDING_DIR:
			send_msg(sfd, code, "NULL");
			do{
				memset(aux, '\0', sizeof(aux));
				read(sfd, &code, sizeof(code));
				read(sfd, &msg_length, sizeof(msg_length));
				read(sfd, &aux, msg_length);
				if(strcmp(aux,"NULL") == 0){
					break;
				}
				printf("%s\n",aux);
				send_msg(sfd,code, "NULL");
			} while (1);
			break;
		case (short)DENIED:
			error(response);
			break;
		case (short)D_NOT_FOUND:
			error(response);
			break;
		case (short)CMD_UNKNOWN:
			error(response);
			break;
	}
}

void client_server(int sfd) {
  short code;
  long msg_length;
  char option[NAME_MAX + 1], split[NAME_MAX + 1], command[4];
  int i = 0, j = 0;
  char *route;
  
  read(sfd, &code, sizeof(code));
  read(sfd, &msg_length, sizeof(msg_length));
	char *msg = malloc(sizeof(msg_length)+1);
	read(sfd, msg, msg_length);
	printf("%s\n", msg);
  
  do {
    menu();
		memset(split, 0, sizeof(split));
		memset(option, 0, sizeof(option));
		memset(command, 0, sizeof(command));
		printf("Command: ");
		fgets(option, sizeof(option), stdin);
		while (option[i] != ' ') {
			split[i] = option[i];
			i++;
		}
		i++;
		strcpy(command, split);
		if (strcmp(command, BYE) != 0) {
			memset(split, 0, sizeof(split));
			while (option[i] != '\0') {
				split[j] = option[i];
				i++;
				j++;
			}
			route = split;
		}
		if (strcmp(command, GET) == 0) {
			if (strlen(route) <= 0) {
				printf("Comando no valido: [get] [ruta]\n");
			} else {
				getting_file(sfd, route);
			}
		}
		
		if (strcmp(command, LIST) == 0) {
			if (strlen(route) <= 0) {
				printf("Comando no valido: [list] [ruta]\n");
			} else {
				list_directory(sfd, route);
			}
		}
		
		if (strcmp(command, BYE) == 0) {
			send_msg(sfd, END, "adios");
    		code = (short) END;
		}
  } while(code != (short) END);
}

int main(int argc, char* argv[]) {
  int sfd, port;
	struct sockaddr_in server_info;

	if (argc != 3) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -1;
	}

	port = atoi(argv[2]);
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}

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

  client_server(sfd);

	close(sfd);
	return 0;
}
