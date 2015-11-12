#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "messenger.h"

#define MSGSZ     128

main()
{
    bool isOpen = true;
    int option = 0;
    char * message = (char*)malloc(MSGSZ);
    char * username = (char*)malloc(10);
    char * receiverUsername = (char*)malloc(10);
    printf("Enter your username:\n");
    scanf("%s", username);

    Messenger messenger(username);
    while(isOpen){
        printf("Select option:\n1. Send Message\n2.Check received messages\n3.Exit\n");
        scanf("%d", &option);

        if(option == 1){
            printf("Enter message to send:\n");
            scanf("%s", message);
            printf("Enter username to send to:\n");
            scanf("%s", receiverUsername);
            messenger.SendMessage(message, receiverUsername);
        }else if(option == 2){
            messenger.ReceiveMessageBuffer();
        }else if(option == 3){
            isOpen = false;
        }
    }
      
    exit(0);
}