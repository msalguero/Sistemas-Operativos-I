#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int msqid;
    int msgflg;
    key_t key;
    message_buf sbuf;
    char* username;
    size_t buf_length;
    int clientId;

public:
    Messenger(char*);
    void SendMessage(char *, char *);
    void ReceiveMessageBuffer();
private:
    void PrepareMessageBuffer(char*, char *, int);
    void SendMessageBuffer();
    void RequestClientId();
 };

Messenger::Messenger(char* username){
    key = 12345;
    this->username = username;
    msgflg = 0666;
    clientId = 4;
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    
    RequestClientId();
}

void Messenger::RequestClientId(){
    char * message = (char*)malloc(MSGSZ);
    char * sendTo = (char*)malloc(10);
    PrepareMessageBuffer(message, sendTo, 1);
    SendMessageBuffer();
    free(message);
    free(sendTo);

    newClientId_buf buffer;
     if (msgrcv(msqid, &buffer, sizeof(buffer) - sizeof(long), 3, 0) < 0) {
        perror("could not receive a client id");
        exit(1);
    }

    clientId = buffer.newClientId;
    printf("clientId: %d\n", clientId);
}

void Messenger::SendMessage(char* message, char *sendTo){
    PrepareMessageBuffer(message, sendTo, 2);
    SendMessageBuffer();
}

void Messenger::PrepareMessageBuffer(char* message, char *sendTo, int type){
    sbuf.mtype = type;

    (void) strcpy(sbuf.sendBy, username);
    (void) strcpy(sbuf.sendTo, sendTo);
    (void) strcpy(sbuf.mtext, message);
    
    buf_length = strlen(sbuf.mtext) + 1 + strlen(sbuf.sendBy) ;
}

void Messenger::SendMessageBuffer(){
     if (msgsnd(msqid, &sbuf, sizeof(message_buf) - sizeof(long), IPC_NOWAIT) < 0) {
        perror("msgsnd");
        exit(1);
    }
  // else 
  //    printf("Message: \"%s\" Sent by %s Sent to %s \n", sbuf.mtext, sbuf.sendBy, sbuf.sendTo);
}

void Messenger::ReceiveMessageBuffer(){
    message_buf buffer;
    if (msgrcv(msqid, &buffer, sizeof(buffer) - sizeof(long), clientId, 0) < 0) {
        perror("could not receive message");
        exit(1);
    }
    printf("Message: \"%s\" Sent by %s Sent to %s \n", buffer.mtext, buffer.sendBy, buffer.sendTo);
}