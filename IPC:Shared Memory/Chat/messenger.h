#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mailbox.h"

#define MSGSZ     128


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

class Messenger{
    int shmid;
    int shmflg;
    key_t key;
    char* username;
    char* sharedMemory;
    size_t buf_length;
    int clientId;
    MailBox* serverMailbox;
    MailBox* clientMailbox;
    int idAssignationSpace;

public:
    Messenger(char*);
    ~Messenger();
    void SendMessage(char *, char *);
    void ReceiveMessageBuffer();
private:
    Message* PrepareMessageBuffer(char*, char *, int);
    void SendMessageBuffer();
    void RequestClientId();
    void*getSharedMemory(key_t key,int size,int shmflg);
    void DeleteSharedMemory();
 };

Messenger::Messenger(char* username){
    shmflg = 0644 | IPC_CREAT;
    key = 1234;
    int size = ((MAXCLIENTS + 1) * (MAXMSG * sizeof(Message))) + sizeof(newClientId_buf);
    //int size = 10 * sizeof(Message);
    idAssignationSpace = (MAXCLIENTS + 1) * (MAXMSG * sizeof(Message));
    sharedMemory = (char*)getSharedMemory(key, size, shmflg);
    serverMailbox = new MailBox((Message*)sharedMemory);
    this->username = username;
    RequestClientId();
}

void Messenger::RequestClientId(){
    Message* messageBuffer = PrepareMessageBuffer(username, username, 0);
    serverMailbox->WriteMailBox(*messageBuffer);

    while(true){
        newClientId_buf* assignationMemory = (newClientId_buf*) &sharedMemory[idAssignationSpace];
        if(assignationMemory->mtype == 1){
            clientId = assignationMemory->newClientId;
            int offset = MAXMSG * clientId;
            clientMailbox = new MailBox((Message*)(sharedMemory + offset));
            printf("%d\n", clientId);
            return;
        }
    }
}

void Messenger::SendMessage(char* message, char *sendTo){
    Message* messageBuffer = PrepareMessageBuffer(message, sendTo, 2);
    serverMailbox->WriteMailBox(*messageBuffer);
}

Message* Messenger::PrepareMessageBuffer(char* message, char *sendTo, int type){
    Message *messageBuffer = (Message*)malloc(sizeof(Message));
    (void) strcpy(messageBuffer->sendBy, username);
    (void) strcpy(messageBuffer->sendTo, sendTo);
    (void) strcpy(messageBuffer->mtext, message);
    messageBuffer->mtype = type;
    return messageBuffer;
}

void Messenger::SendMessageBuffer(){
    
}

void Messenger::ReceiveMessageBuffer(){
    Message *message = clientMailbox->ReadMailBox();
        if(message != 0){
            printf("Message: \"%s\" Sent by %s Sent to %s \n", message->mtext, message->sendBy, message->sendTo);
        }
}

void* Messenger::getSharedMemory(key_t key,int size,int shmflg){
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

Messenger::~Messenger(){
    DeleteSharedMemory();
}

void Messenger::DeleteSharedMemory(){
    shmctl(shmid, IPC_RMID, NULL);
}