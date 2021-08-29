/********************** Client side C program to demonstrate Socket programming ***************************/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
#include "header.h"
#include <time.h>
#include <stdlib.h>
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
	
    int posx,posy,choice;
    char read_buf[100] = {0};
    chk_box_t data2;   
    chk_box_t *data_buf = &data2;

    valread = read( sock , data_buf, sizeof(chk_box_t));
    display(data_buf);
	
    while ( 1 )
    {
        while ( data_buf->turn == 1 ) {
            printf("Player 1 is moving,please wait.......\n");
            sleep(2);
            //Message being received!
	    int ret = read( sock , data_buf, sizeof(chk_box_t));
            if (ret < 0){
                printf("Message not received. Trying Again!");
            }
            else{
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
                
                    //Message being sent!
                    data_buf->move = choice;
		    int ret = send(sock , data_buf , sizeof(chk_box_t) , 0 );	
                    if ( ret == -1){
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
