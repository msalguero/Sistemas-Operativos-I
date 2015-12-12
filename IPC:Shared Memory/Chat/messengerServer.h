#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include "mailbox.h"

#define MSGSZ     128
#define MAXCLIENTS     10
#define MAXMSG     10

//message types table
//1 - login request
//2 - client id assignation

/*
 * Declare the message structure.
 */
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
	int shmid;
    key_t key;
    int shmflg;
    int nextClientId;
    client clients[MAXCLIENTS];
    int clientCount;
    MailBox *mailbox;
    int idAssignationSpace;
    char *sharedMemory;

public:
	MessengerServer();
    ~MessengerServer();
	void waitForMessage();
private:
	void AssignNewClientId(char* username);
    void RouteMessage(Message *message);
    int GetClientId(char* username);
    void* getSharedMemory(key_t key,int size,int shmflg);
    void DeleteSharedMemory();
    void* GetEmptyMailBox();
};

MessengerServer::MessengerServer(){
    shmflg = 0644 | IPC_CREAT;
    key = 1234;
    nextClientId = 1;
    clientCount = 0;
    int size = ((MAXCLIENTS + 1) * (MAXMSG * sizeof(Message))) + sizeof(newClientId_buf);
    printf("%d\n", size);
    //int size = 10 * sizeof(Message);
    char *sharedMemory = (char*)getSharedMemory(key, size, shmflg);
    mailbox = new MailBox((Message*)sharedMemory);
    idAssignationSpace = (MAXCLIENTS + 1) * (MAXMSG * sizeof(Message));
    printf("%d\n", idAssignationSpace);
}

void MessengerServer::DeleteSharedMemory(){
    shmctl(shmid, IPC_RMID, NULL);
}

void* MessengerServer::getSharedMemory(key_t key,int size,int shmflg){
    char* data;
    if ((shmid = shmget (key, size, shmflg)) == -1) {
        perror("shmget: shmget failed"); 
        exit(1); 
    } 
    data = (char*)shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1))
        perror("shmat");
    return data;
}

void MessengerServer::waitForMessage(){
    while(true){
    	Message *message = mailbox->ReadMailBox();
        if(message != 0){
            if(message->mtype == 0){
                AssignNewClientId(message->sendBy);
                free(message);
            }else{
//                RouteMessage(message);
                free(message);
                printf("Message: \"%s\" Sent by %s Sent to %s \n", message->mtext, message->sendBy, message->sendTo);
            }
        }
    }
}

void MessengerServer::AssignNewClientId(char* username){
    newClientId_buf *client_buffer = (newClientId_buf*)malloc(sizeof(newClientId_buf));
    client_buffer->mtype = 1;
    client_buffer->newClientId = nextClientId;
    char * assignationMemory1 = &sharedMemory[idAssignationSpace];
    newClientId_buf* assignationMemory = (newClientId_buf*) sharedMemory;
    *assignationMemory1 = 'cc';
    //memcpy(sharedMemory, client_buffer, sizeof(newClientId_buf));
    client newClient = {nextClientId++, username};
    clients[clientCount++] = newClient;
}

void* MessengerServer::GetEmptyMailBox(){

}

void MessengerServer::RouteMessage(Message *message){
    int clientId = GetClientId(message->sendTo);
    int offset = MAXMSG * clientId;
    MailBox temporaryMailbox((Message*)(sharedMemory + offset));
}

int MessengerServer::GetClientId(char* username){
    for(int i = 0; i < clientCount; i++){
        if(strcmp(clients[i].username, username)){
            printf("%d\n", clients[i].clientId);
            return clients[i].clientId;
        }
    }
}

MessengerServer::~MessengerServer(){
    DeleteSharedMemory();
}