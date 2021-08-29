#include <stdio.h>
#include "header.h"
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

void display(chk_box_t * data)
{
    printf("Resultant Buff:\n\n");
	printf(" %c | %c | %c\n", data->grid[0][0], data->grid[0][1], data->grid[0][2]);
	printf("---|---|---\n");
	printf(" %c | %c | %c\n", data->grid[1][0], data->grid[1][1], data->grid[1][2]);
	printf("---|---|---\n");
	printf(" %c | %c | %c\n", data->grid[2][0], data->grid[2][1], data->grid[2][2]);
}

void initialize(chk_box_t* data)
{
    int i=0,j=0,c=0;
    
    for ( i=0 ; i < 3 ; i++ )
    {
        for ( j=0 ; j < 3 ; j++ )
        {
            data->grid[i][j] = 49 + c++;
        }
    }
    data->count = 0;
    printf("data_buf init done\r\n");	
}


int check(chk_box_t* data)
{
	int i;
	
	if (data->grid[0][0] == data->grid[1][1] && data->grid[0][0] == data->grid[2][2] || 
	   (data->grid[0][2] == data->grid[1][1] && data->grid[0][2] == data->grid[2][0])){
		return 1;
        }
		
	for (i = 0; i < 3; i++){
		if (data->grid[i][0] == data->grid[i][1] && data->grid[i][0] == data->grid[i][2] ||
		   (data->grid[0][i] == data->grid[1][i] && data->grid[0][i] == data->grid[2][i]))
	    return 1;
	}
	
	return 0;

}
