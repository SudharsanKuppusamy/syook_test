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
#include <arpa/inet.h>
#include <string.h>


/**********Server********/

int main()
{


    chk_box_t *data_buf;
    char read_buf[100] = {0};

    int posx,posy,choice;
    int ret;

/**************************************************************************************/
    int i=0,j=0, socket_fd, new_socket;
    struct sockaddr_in varAdd;
    int addr_len;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd < 0)
    {
	printf("SOCKET ERROR\r\n");
	return -1;
    } 
    printf("Sockfd=%d\r\n",socket_fd);
    varAdd.sin_family = AF_INET;           //IP address set
    varAdd.sin_addr.s_addr = inet_addr("0.0.0.0");
    varAdd.sin_port = htons(3000);
    addr_len = sizeof(varAdd);

    ret = bind(socket_fd,(struct sockaddr*) &varAdd, addr_len);
    if(ret < 0)
    {
         printf("BIND ERROR\r\n");       //Bind with specific socket
         return -1;
    }

    ret = listen(socket_fd, 4);

    if(ret < 0)
    {				         //Wait until connection happens
	printf("LISTENING ERROR\r\n");
        return -1;
    }

    new_socket = accept(socket_fd, (struct sockaddr*)&varAdd, (socklen_t*)&addr_len);

    if(new_socket<0)
    {
        printf("ACCEPTANCE ERROR\r\n");  //Accept the connection
	return 0;
    }


/**************************************************************************************/  
    initialize(data_buf);
    display(data_buf);
    long int rand_choice = rand();
    if ( rand_choice % 2 == 0 )  {
        data_buf->turn=1;
        printf("Player1 goes first,sign for player 1 is 'x'\n");
    }
    else    {
    
        data_buf->turn=2;
        printf("Player2 goes first,sign for player 2 is 'o'\n");
    }
    
	while (1)
	{
		while (data_buf->turn == 2)   {
			printf("Player 2 is moving,please wait.....\n");
			//sleep(2);

            //Message being received!
	    int ret = recv(new_socket, (chk_box_t *)&data_buf, sizeof(struct chk_box), 0); 
            if (ret == -1){
                printf("Message not received. Trying Again!");
            }   
            else{
				data_buf->turn = 1;
			}
		}
		display(data_buf);
		if (!check(data_buf))
		{
			//player1 moves  
			if (data_buf->count > 8)
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
					data_buf->move=choice;
				
					posx = --choice / 3;
					posy = choice % 3;
				} while (choice<0 || choice> 9 || data_buf->grid[posx][posy] > '9');
				data_buf->grid[posx][posy] = 'x';
				data_buf->count = data_buf->count + 1;
				
				//Message being sent!
                                int ret = send(new_socket, (chk_box_t *)&data_buf, sizeof(struct chk_box), 0);  
				if ( ret == -1){            
					printf("Failed to send message. Please Try again!");
				}
				
				if (data_buf->count > 8 && !check(data_buf))
				{
				    display(data_buf);
				    printf("\nIts a Tie!!\n");
				    break;
				}
				
                
				//player 1 has moved

				display(data_buf);
			
				data_buf->turn = 2;
				
				if ( check(data_buf) )  {
                    printf("Congratulations!,you have won the game!\n");
                    break;
                }
                
			}
		}
		else
		{
			printf("\nPlayer 2 is the Winner!\n");
			data_buf->turn = 2;
			break;
		}
		
		
	}

    return 0;
}
