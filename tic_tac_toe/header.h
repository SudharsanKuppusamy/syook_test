
/***************structures and function prototypes************/



typedef struct chk_box
{
    char grid[3][3] ;
    int turn ;
    int msgtype;
    int count;	//counts the no. of turns
    int move;
}chk_box_t;


//function that will display the board which we have stored 
void display(chk_box_t* data);

//function to fill the grid with all blank spaces
void initialize(chk_box_t* data);

//function to check the winner, return 1 for success and 0 for failure
int check(chk_box_t* data);

