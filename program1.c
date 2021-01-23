/*
    this program controls movement for player 1
    also decides who moves first and go,should be the first program to execute
*/
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"
#include <time.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

/**********Server********/

int main()
{

    int msqid;

/**********************/
    int i=0,j=0, socket_fd, new_socket;
    struct sockaddr address;
    int addr_len = sizeof(address);
    chk_box_t *recv_buf;
    char *msg = "Hello World";
/**********************/

    int posx,posy,choice;
    long int msgtr=0;

/**************************************************************************************/
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0) == 0)
    {
	printf("SOCKET ERROR\r\n");
	return -1;
    } 
    address.sin_family = AF_INET;           //IP address set
    address.sin_addr.s_addr = INADDR_ANY;   //inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);
    
    if((bind(socket_fd,(sockaddr*)&address, addr_len))<0)
    {
         printf("BIND ERROR\r\n");
         return -1;
    }

    if((listen(socket_fd, 2)<0)
    {
	printf("LISTENING ERROR\r\n");
        return -1;
    }

    if((new_socket = accept(socket_fd, (sockaddr*)&address, addr_len))<0)
    {
        printf("ACCEPTANCE ERROR\r\n");
    }
    read(new_socket, recv_buf, sizeof(recv_buf));
    printf("%s\r\n", recv_buf);

    send(new_socket, msg, strlen(msg), 0);
    printf("Message sent\r\n");

/**************************************************************************************/
    srand(time(NULL));   

    msqid = msgget((key_t)4321, 0666|IPC_CREAT);
	if(msqid == -1){
		printf("Message Queue attachment failed, Program will now abort!");
	}
    
    if ( recv_buf == NULL )
    {
        printf("No data...Program will now abort!\n");
        exit();
    }

    //clear();
    initialize(recv_buf);
     
    if ( rand() % 2 == 0 )  {
        recv_buf->turn=1;
        printf("Player1 goes first,sign for player 1 is 'x'\n");
    }
    else    {
    
        recv_buf->turn=2;
        printf("Player2 goes first,sign for player 2 is 'o'\n");
    }
    
	while (1)
	{
		while (recv_buf->turn == 2)   {
			printf("Player 2 is moving,please wait.....\n");
			//sleep(2);
            //Message being received via message queue!
            if (msgrcv(msqid, (void *)data, 1024, msgtr, 0) == -1){
                printf("Message not received. Trying Again!");
            }   
            else{
				//printf("Received Message : %d,%d",data->move,data->count);
				recv_buf->turn = 1;
			}
		}
		display(recv_buf);
		if (!check(recv_buf))
		{
			//player1 moves  
			if (recv_buf->count > 8)
			{
				printf("\nIts a Tie !!\n");
				break;
			}
			else
			{
				do
				{
					printf("Choose where to place mark : ");
					scanf("%d", &choice);
					recv_buf->move=choice;
					msgtr = 0; 
				
					posx = --choice / 3;
					posy = choice % 3;
				} while (choice<0 || choice> 9 || recv_buf->grid[posx][posy] > '9');
				recv_buf->grid[posx][posy] = 'x';
				recv_buf->count = recv_buf->count + 1;
				
				//Message being sent by message Queue!
				if (msgsnd(msqid, (void *)data, 1024, 0) == -1){
					printf("Failed to send message. Please Try again!");
				}
				
				if (recv_buf->count > 8 && !check(recv_buf))
                {
                    display(recv_buf);
                    printf("\nIts a Tie!!\n");
                    break;
                }
                
                
				//player 1 has moved

				display(recv_buf);
			
				recv_buf->turn = 2;
				
				if ( check(recv_buf) )  {
                    printf("Congratulations!,you have won the game!\n");
                    break;
                }
                
			}
		}
		else
		{
			printf("\nPlayer 2 is the Winner!\n");
			recv_buf->turn = 2;
			break;
		}
		
		
	}

    return 0;
}
