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
    size_t buf_length;
    int clientId;
    MailBox* mailbox;

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
    //int size = ((MAXCLIENTS + 1) * (MAXMSG * sizeof(message_buf)));
    int size = 10 * sizeof(Message);
    char *arr = (char*)getSharedMemory(key, size, shmflg);
    mailbox = new MailBox((Message*)arr);
    this->username = username;
}

void Messenger::RequestClientId(){
    
}

void Messenger::SendMessage(char* message, char *sendTo){
    Message* messageBuffer = PrepareMessageBuffer(message, sendTo, 2);
    mailbox->WriteMailBox(*messageBuffer);
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