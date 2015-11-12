#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ     128
#define MAXCLIENTS     10
#define MAXMSG     10

typedef struct {
	long    mtype;
	char    mtext[MSGSZ];
	char    sendBy[10];
	char    sendTo[10];
 } Message;


class MailBox{
    int shmid;
    int shmflg;
    key_t key;
    Message *messages;
    char* username;
    size_t buf_length;
    int clientId;

public:
	MailBox(Message *messages);
	Message* ReadMailBox();
	int WriteMailBox(Message message);

private:
	void ClearMailBox();
};

Message* MailBox::ReadMailBox(){
	for(int i = 0; i < MAXMSG; i++){
		Message* message = (Message*)malloc(sizeof(Message));
		*message = messages[i];
		if(message->mtype != -1){
			messages[i].mtype = -1;
			return message;
		}
	}
	
	return 0;
}

int MailBox::WriteMailBox(Message message){
	for(int i = 0; i < MAXMSG; i++){
		if(messages[i].mtype == -1){
			messages[i] = message;
			return 0;
		}
	}
	return -1;
}

void MailBox::ClearMailBox(){
	for(int i = 0; i < MAXMSG; i++){
		messages[i].mtype = -1;
	}
}

MailBox::MailBox(Message *messages){
	this->messages = messages;
	ClearMailBox();
}