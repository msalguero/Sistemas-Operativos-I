#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "echo_socket"

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

struct User users[15];

void writeFile();

int main(void)
{
  initializeUsers();
    int s, s2, t, len;
    struct sockaddr_un local, remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    for(;;) {
        int done, n;
        printf("Waiting for a connection...\n");
        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
            perror("accept");
            exit(1);
        }

        printf("Connected.\n");

        done = 0;
        do {
            n = recv(s2, str, 100, 0);
            if (n <= 0) {
                if (n < 0) perror("recv");
                done = 1;
            }

            if (!done){
               struct Request req;
               memcpy(&req, str, 100);
               printf("%s\n", req.content);
               printf("%d\n", req.type);

               if(req.type == 1){
                struct User newUser;
                memcpy(&newUser, req.content, 96);
                printf("%s\n", newUser.username);
                addUser(newUser);
               }else if(req.type == 2){
                if (send(s2, users, sizeof(struct User)*15, 0) < 0) {
                    perror("send");
                    done = 1;
                }
               }else if(req.type == 3){
                  removeUser(req.content);
               }else if(req.type == 4){

               }
            }
        } while (!done);

        close(s2);
    }

    return 0;
}

int removeUser(char *username){
  int i;
  for ( i = 0; i < 15; i++)
  {
    if(!strncmp(users[i].username, username, 16)){
      printf("removed: %s\n", users[i].username);
      struct User emptyUser;
      char *emptyUsername = "empty";
      strcpy(emptyUser.username, emptyUsername);
      users[i] = emptyUser;
      writeFile();
      return 0;
    }
  }
  return -1;
}

int addUser(struct User user){
  int i;
  for ( i = 0; i < 15; i++)
  {
    if(!strncmp(users[i].username, "empty\n", 5)){
      printf("added: %s\n", user.username);
      users[i] = user;
      writeFile();
      return 0;
    }
  }
  return -1;
}


int initializeUsers(){
  int i;
  for ( i = 0; i < 15; i++)
  {
    struct User emptyUser;
      char *username = "empty";
      strcpy(emptyUser.username, username);
      users[i] = emptyUser;
  }
}

void writeFile(){
  FILE *f = fopen("users.txt", "w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  const char *text = (char*)users;
  fprintf(f, "%s\n", text);

  fclose(f);
}

void ReadFile(){
  FILE *fp;
  
  fp = fopen("users.txt", "r");
  fscanf(fp, "%s", (char*)users);
  
  fclose(fp);
}