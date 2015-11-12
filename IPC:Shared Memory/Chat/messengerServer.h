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
};

MessengerServer::MessengerServer(){
    shmflg = 0644 | IPC_CREAT;
    key = 1234;
    //int size = ((MAXCLIENTS + 1) * (MAXMSG * sizeof(message_buf)));
    int size = 10 * sizeof(Message);
    char *arr = (char*)getSharedMemory(key, size, shmflg);
    mailbox = new MailBox((Message*)arr);
    
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
            if(message->mtype == 1){
                AssignNewClientId(message->sendBy);
                free(message);
            }else{
                //RouteMessage(message);
                printf("Message: \"%s\" Sent by %s Sent to %s \n", message->mtext, message->sendBy, message->sendTo);
            }
        }
    }
}

void MessengerServer::AssignNewClientId(char* username){
    	
}

void MessengerServer::RouteMessage(Message *message){
    
}

int MessengerServer::GetClientId(char* username){
    
}

MessengerServer::~MessengerServer(){
    DeleteSharedMemory();
}