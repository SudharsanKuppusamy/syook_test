/*
This program controls movement(s) for player 2
program1 should be run first
*/
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
/*****Client*****/

int main(void)
{
    int msqid;
    int i=0,j=0, sock = 0;
    int posx,posy,choice;
    long int msgtr=0;
    struct sockaddr serv_addr;
    char *msg = "CLIENT"
    chk_box_t data_buf;

/*****************************************************************************/
    if((sock = socket(AF_INET, SOCK_STREAM, 0) == 0)
    {
	printf("SOCKET ERROR\r\n");
	return -1;
    } 

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if((inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr))<=0)
    {
        printf("INVALID ADDRESS\r\n");
        return -1;
    }
    if((connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("CONNNECTION ERROR\r\n");
        return -1;
    }
    send(sock, msg, strlen(msg), 0);
    printf("Message sent\r\n");

    read(sock, data_buf, sizeof(buf));
    printf("%s\r\n", data_buf);
/******************************************************************************/

    msqid = msgget((key_t)4321, 0666|IPC_CREAT);
    if(msqid == -1){
        printf("Message Queue attachment failed, Program will now abort!");

    }
    
    if ( data_buf == NULL )
    {
        printf("ERROR...Program will now abort!\n");
        exit();
    }
    
    
    //the program loops over infinitely
    while ( 1 )
    {
        while ( data_buf->turn == 1 ) {
            printf("Player 1 is moving,please wait.......\n");
            //sleep(2);
            //Message being received via Message Queue!
            if (msgrcv(msqid, (void *)data, 1024, msgtr, 0) == -1){
                printf("Message not received. Trying Again!");
            }
            else{
                //printf("Received Message : %d,%d",data->move,data->count);
                data_buf->turn = 2;
            }
        }
        display(data_buf);
        if (!check(data_buf))
        {
        //player2 will now move
            if (data_buf->count > 8)
            {
                printf("\nIt's a Tie!!\n");
                break;
            }
            else
            {
                do
                {
                    printf("Choose where to place mark : ");
                    scanf("%d", &choice);

                    posx = --choice / 3;
                    posy = choice % 3;
                } while (choice<0 || choice>9 || data_buf->grid[posx][posy] >'9');
                data_buf->grid[posx][posy] = 'o';
                 data_buf->count = data_buf->count + 1;
                
                //Message being sent by message Queue!
                    data_buf->move = choice;
                    if (msgsnd(msqid, (void *)data, 1024, 0) == -1){
                        printf("Failed to send message. Please Try again!");
                    }
				if (data_buf->count > 8 && !check(data_buf))
                {
                    display(data_buf);
                    printf("\nIt's a Tie!!\n");
                    break;
                }

                   //player2 has moved
                display(data_buf);
                                
               
                data_buf->turn = 1;
                
                if ( check(data_buf) )  {
                    printf("Congratulations!,you have won the game!\n");
                    break;
                }
               
            }
        }
        else
        {
            printf("\nPlayer 1 is the Winner!\n");
            data_buf->turn = 1;
            break;
        }
    }
    return 0;
}
