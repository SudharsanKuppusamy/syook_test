/********************** Server side C program to demonstrate Socket programming ***************************/

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <stdlib.h>
#include "header.h"
#include <time.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    chk_box_t data1;
    chk_box_t *data_buf = &data1;
	
    char read_buf[100] = {0};

    int posx,posy,choice;
    int ret;
	
    initialize(data_buf);
    display(data_buf);
	
    long int rand_choice = rand();
    if ( rand_choice % 2 == 0 )  {
        data_buf->turn=1;
        printf("Player1 goes first,sign for player 1 is 'x'\n");
    }
    else{
    
        data_buf->turn=2;
        printf("Player2 goes first,sign for player 2 is 'o'\n");
    }
    send(new_socket , data_buf , sizeof(chk_box_t) , 0 );
    printf("Grid message sent\n");
	while (1)
	{
		while (data_buf->turn == 2)   {
			printf("Player 2 is moving,please wait.....\n");
			//sleep(2);

            //Message being received   
	    valread = read( new_socket , data_buf, sizeof(chk_box_t));
            if (valread == -1){
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
				int ret = send(new_socket , data_buf , sizeof(chk_box_t) , 0 );				
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
