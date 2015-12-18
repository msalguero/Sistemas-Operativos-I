#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <regex.h> 

#define SOCK_PATH "echo_socket"
#define REQ_SIZE 100

struct Request
{
	int type;
	char content[96];
};

struct User
{
	char username[16];
	char name[20];
	char email[20];
	char id[15];
	char date[10];
	char photo[15];
};
void NewUser(void * content);

int main(){
	int s, t, len;
	struct sockaddr_un remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        perror("connect");
        exit(1);
    }

	char username[10];
	int option;
	while(1){
		printf("1. Add User\n2. List Users\n3. Erase User\n4. Send users to email\n5. Exit\n");
		scanf("%d", &option);
		struct Request req;
		if(option == 1){
			NewUser(&req.content);
			req.type = option;
		}else if(option == 2){
			req.type = option;
			strcpy(req.content, "");
		}else if(option == 3){
			printf("Enter username to erase\n");
			scanf("%s", username);
			req.type = option;
			strcpy(req.content, username);
		}else if(option == 4){
			printf("Enter username to send email to\n");
			scanf("%s", username);
			req.type = option;
			strcpy(req.content, username);
		}else if(option == 5){
			exit(0);
		}
		if (send(s,(void*) &req, REQ_SIZE, 0) == -1) {
            perror("send");
            exit(1);
        }
        if(option == 2){
        	struct User users[15];
        	char mes[1440];
			if ((t=recv(s, mes, sizeof(struct User)*15, 0)) > 0) {
	            //str[t] = '\0';
	            //printf("echo> %s", str);
	            memcpy(users, mes, 1440);
	            //printf("received %d\n", t);
	            int i;
	            //printf("%s\n", users[i].username);
				  for ( i = 0; i < 15; i++)
				  {
				    if(strncmp(users[i].username, "empty\n",5)){
				      printf("%s\n", users[i].username);
				    }
				  }
	        } else {
	            if (t < 0) perror("recv");
	            else printf("Server closed connection\n");
	            exit(1);
	        }
        }
	}
	return 0;
}

void NewUser(void * content){
	regex_t regex;
	int reti;
	struct User newUser;
	char username[16];
	printf("Enter username\n");
	scanf("%s", username);
	strcpy(newUser.username, username);
	char name[20];
	printf("Enter name\n");
	scanf("%s", name);
	strcpy(newUser.name, name);
	char email[20];
	reti = regcomp(&regex, "[A-Za-z0-9_][A-Za-z0-9_]*@[A-Za-z0-9_][A-Za-z0-9_]*.[A-Za-z][A-Za-z][A-Za-z]", 0);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}
	do{
		printf("Enter email\n");
		scanf("%s", email);
		reti = regexec(&regex, email, 0, NULL, 0);
		if(reti == REG_NOMATCH){
			printf("email no valido\n");
		}
	}while(reti == REG_NOMATCH);
	strcpy(newUser.email, email);
	char id[20];
	reti = regcomp(&regex, "[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9][0-9]", 0);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}
	do{
		printf("Enter id\n");
		scanf("%s", id);
		reti = regexec(&regex, id, 0, NULL, 0);
		if(reti == REG_NOMATCH){
			printf("id no valido\n");
		}
	}while(reti == REG_NOMATCH);
	strcpy(newUser.id, id);
	char birthdate[20];
	reti = regcomp(&regex, "[0-9][0-9]/[0-9][0-9]/[0-9][0-9][0-9][0-9]", 0);
	if (reti) {
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}
	do{
		printf("Enter birthdate\n");
		scanf("%s", birthdate);
		reti = regexec(&regex, birthdate, 0, NULL, 0);
		if(reti == REG_NOMATCH){
			printf("birthdate no valido\n");
		}
	}while(reti == REG_NOMATCH);
	strcpy(newUser.date, birthdate);
	char photo[20];
	printf("Enter photo\n");
	scanf("%s", photo);
	strcpy(newUser.photo, photo);
	memcpy(content, &newUser, 96);
}