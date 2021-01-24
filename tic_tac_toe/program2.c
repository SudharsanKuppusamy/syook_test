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
#include <arpa/inet.h>
#include <string.h>

/*****Client*****/

int main(void)
{
    int posx,posy,choice;
    char read_buf[100] = {0};
    chk_box_t *data_buf;

/*****************************************************************************/
    int i=0,j=0, sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if( sock < 0)
    {
	printf("SOCKET ERROR\r\n");			//Create socket
	return -1;
    } 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3000);

    printf("socket_created\r\n");
    if((inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr))<=0)
    {
        printf("INVALID ADDRESS\r\n");
        return -1;
    }
    if((connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("CONNNECTION ERROR\r\n");              //Connect to the IP
        return -1;
    }
printf("Connection done\r\n");
/******************************************************************************/
    
    
    //the program loops over infinitely
    while ( 1 )
    {
	printf("Entered into while1\r\n");
        while ( data_buf->turn == 1 ) {
            printf("Player 1 is moving,please wait.......\n");
            //sleep(2);
            //Message being received!
            int ret = recv(sock, (chk_box_t *)&data_buf, sizeof(struct chk_box), 0);
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
		    int ret = send(sock, (chk_box_t *)&data_buf, sizeof(struct chk_box), 0);
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
