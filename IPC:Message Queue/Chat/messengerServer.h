#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ     128
#define MAXCLIENTS     10

//message types table
//1 - login request
//2 - client id assignation

/*
 * Declare the message structure.
 */

typedef struct {
    long    mtype;
    char    mtext[MSGSZ];
    char    sendBy[10];
    char    sendTo[10];
} message_buf;

typedef struct 
{
	long mtype;
	int newClientId;
} newClientId_buf;

typedef struct 
{
    int clientId;
    char* username;
} client;

class MessengerServer{
	int msqid;
    key_t key;
    message_buf  rbuf;
    int nextClientId;
    client clients[MAXCLIENTS];
    int clientCount;

public:
	MessengerServer();
	void waitForMessage();
private:
	void AssignNewClientId(char* username);
    void RouteMessage(message_buf message);
    int GetClientId(char* username);
};

MessengerServer::MessengerServer(){
	key = 12345;
	nextClientId = 4;
    clientCount = 0;

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        perror("msgget");
        exit(1);
    }
}

void MessengerServer::waitForMessage(){
	while(true){
		
    if (msgrcv(msqid, &rbuf, sizeof(message_buf) - sizeof(long), -2, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    if(rbuf.mtype == 1){
    	AssignNewClientId(&(rbuf.mtext[0]));
    	printf("Login Successfull\n");
    }else{
        RouteMessage(rbuf);
    }

    printf("Message: \"%s\" Sent by %s Sent to %s \n", rbuf.mtext, rbuf.sendBy, rbuf.sendTo);
	}
}

void MessengerServer::AssignNewClientId(char* username){
    if(clientCount < MAXCLIENTS){
        newClientId_buf buf = { 3, nextClientId};
        if (msgsnd(msqid, &buf, sizeof(buf) - sizeof(long), IPC_NOWAIT) < 0) {
            perror("Could not sent new Id");
            exit(1);
        }
        client newClient = {nextClientId++, username};
        clients[clientCount++] = newClient;
    }	
}

void MessengerServer::RouteMessage(message_buf message){
    int targetClientId = GetClientId(message.sendTo);
    message.mtype = targetClientId;
    if (msgsnd(msqid, &message, sizeof(message) - sizeof(long), IPC_NOWAIT) < 0) {
        perror("Could not route message");
        exit(1);
    }
}

int MessengerServer::GetClientId(char* username){
    for(int i = 0; i < clientCount; i++){
        if(strcmp(clients[i].username, username)){
            printf("%d\n", clients[i].clientId);
            return clients[i].clientId;
        }
    }
}