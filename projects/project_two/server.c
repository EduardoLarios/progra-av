#include "header.h"

char *ruta;
char *program;

void logger(char *msg){
	FILE *file;
	char *logg;
	
	logg = ruta;
	strcat(logg, "/logs/log");
	file = fopen(logg, 'a');
	if (file == NULL) {
		printf("Internal Error\n");
		exit(-1);
	}
	
	fscanf(file, "%s", msg);
	fclose(file);
}

void error(int nsfd, short error, char *msg) {
	short code = error;
	long msg_length = strlen(msg);
	
	write(nsfd, &code, sizeof(code));
	write(nsfd, &msg_length, sizeof(msg_length));
	write(nsfd, msg, msg_length);
	
	char logmsg[200];
	sprintf(logmsg,"error:%s\n",msg);
	logger(logmsg);
}

void response(int nsfd, short codes, char *msg) {
	short code = codes;
	long msg_length = strlen(msg);
	
	write(nsfd, &code, sizeof(code));
	write(nsfd, &msg_length, sizeof(msg_length));
	write(nsfd, msg, msg_length);
	
	char logmsg[200];
	sprintf(logmsg,"message:%s\n",msg);
	logger(logmsg);
}

void finish(int nsfd, short code, char *msg) {
	char logmsg[200];
	sprintf(logmsg, "message:%s\n", msg);
	logger(logmsg);
}

void get_file(int nsfd, short code, char *msg) {
	long msg_length;
	int fd_in, nbytes;
	char buffer;
	
	if(strstr(msg, "..") != NULL){
		error(nsfd,(short)DENIED,"La ruta nos es valida");
		return;
	}
	
	char *floc = malloc(sizeof(msg) + 2 + sizeof(ruta));
	strcat(floc, ruta);
	strcat(floc, msg);
	
	char logmsg[200];
	sprintf(logmsg,"file dir:%s\n",floc);
	logger(logmsg);
	
	if ( (fd_in = open(floc, O_RDONLY)) < 0 ) {
		error(nsfd, (short)FILE_NOT_FOUND, "File not found");
		free(floc);
		return;
	}
	
	struct stat path_stat;
	stat(floc, &path_stat);
	
	if(S_ISREG(path_stat.st_mode) == 0){
		error(nsfd, (short)IS_DIR, "File not found");
		free(floc);
		return;
	}
	
	code = (short)SENDING_FILE;
	char aux[100];
	response(nsfd, code, "NULL");
	read(nsfd, &code, sizeof(code));
	read(nsfd, &msg_length, sizeof(msg_length));
	read(nsfd, aux, msg_length);
	char letter[1] = {};
	
	do {
		buffer = '\0';
		nbytes = read(fd_in, &buffer, sizeof(buffer));
		memset(letter, '\0', sizeof(letter));
		strcpy(letter, &buffer);
		response(nsfd, code, letter);
	
		if(nbytes == 0){
			break;
		}
	
		read(nsfd, &code, sizeof(code));
		read(nsfd, &msg_length, sizeof(msg_length));
		read(nsfd, &aux, msg_length);
	} while (1);
	free(floc);
	close(fd_in);
}

void list_dir(int nsfd, short code, char *msg){
	long msg_length;
	char file[100]={};
	char logmsg[200];
	sprintf(logmsg, "list dir: %s\n", msg);
	logger(logmsg);
	
	if (strstr(msg, "..") != NULL){
		error(nsfd, (short)DENIED, "The route is not valid");
		return;
	}
	
	char *floc = malloc(sizeof(msg) + 2 + sizeof(ruta));
	strcat(floc, ruta);
	strcat(floc, msg);
	
	DIR *dir;
	struct dirent* direntry;
	struct stat buf;
	if( strlen(msg) <= 1){
		memset(floc, '\0', sizeof(floc));
		strncpy(floc, ruta, sizeof(ruta));
	}
	
	if((dir = opendir(floc)) == NULL){
		error(nsfd,(short)D_NOT_FOUND, "directory not found");
		free(floc);
		return;
	}
	
	code = SENDING_DIR;
	response(nsfd, code, "NULL");
	read(nsfd, &code, sizeof(code));
	read(nsfd, &msg_length, sizeof(msg_length));
	read(nsfd, file, msg_length);
	while( (direntry = readdir(dir)) != NULL) {
		if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") !=0){
			memset(file, '\0', sizeof(file));
			sprintf(file, "%s/%s", floc, direntry->d_name);
			response(nsfd, code, file);
			read(nsfd, &code, sizeof(code));
			read(nsfd, &msg_length, sizeof(msg_length));
			read(nsfd, &file, msg_length);
		}	
	}
	
	response(nsfd,code,"NULL");
	free(floc);
	closedir(dir);
}
void* server_client(void *param) {
	int nsfd = *( (int*) param);
	short code = (short) WELCOME;
	long msg_length;
	
	response(nsfd, code, "Hola");
	do {
		read(nsfd, &code, sizeof(code));
		read(nsfd, &msg_length, sizeof(msg_length));
		char *route = malloc(msg_length);
		read(nsfd, route, msg_length);
		switch(code){
			case (short) FILE_ROUTE:
				get_file(nsfd, code, route);
				break;
			case (short) DIRECTORY_ROUTE:
				list_dir(nsfd, code, route);
				break;
			case (short) END:
				finish(nsfd, code, route);
				break;
			default:
				code = (short) CMD_UNKNOWN;
				response(nsfd, code, "Comando no conocido");
				break;
		}
		free(route);
	} while (code != (short) END);

	close(nsfd);
}

void server(char* ip, int port) {
	int sfd, nsfd, pid;
	pthread_t thread_id;
	char logmsg[200];
	DIR *dir;
	struct sockaddr_in server_info, client_info;

	if ( (dir = opendir(ruta)) == NULL ) {
		perror(program);
		exit(-1);
	}

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
		
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		sprintf(logmsg,"Client: %i.%i.%i.%i date: %d-%d-%d %d:%d:%d \n",
			(int)(client_info.sin_addr.s_addr&0xFF),
			(int)((client_info.sin_addr.s_addr&0xFF00)>>8),
			(int)((client_info.sin_addr.s_addr&0xFF0000)>>16),
			(int)((client_info.sin_addr.s_addr&0xFF000000)>>24),
			tm.tm_year + 1900, tm.tm_mon + 1, 
			tm.tm_mday, 
			tm.tm_hour, 
			tm.tm_min, 
			tm.tm_sec);
		logger(logmsg);

		pthread_create(&thread_id, NULL, server_client, ((void *) &nsfd));
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
	program = argv[0];
	server(ip, port);

	return 0;
}
