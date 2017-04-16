#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
#define MAX 10
#include <stdbool.h>
// background color
#define KNRM  "\x1B[0m"
#define backRed  "\x1B[41m"
#define backGreen  "\x1B[42m"
#define backYellow  "\x1B[43m"
#define backBlue  "\x1B[44m"
#define backMagenta  "\x1B[45m"
#define backCyan  "\x1B[46m"
#define backWhite  "\x1B[47m"
// text color
#define textRed  "\x1B[31m"
#define textGreen  "\x1B[32m"
#define textYellow  "\x1B[33m"
#define textBlue  "\x1B[34m"
#define textMagenta  "\x1B[35m"
#define textCyan  "\x1B[36m"
#define textWhite  "\x1B[37m"


// global variables
// game table
unsigned char table_array[MAX][MAX];
// location of cursor
int x=0, y=0;
// flag: input mode = 0, flag mode = 1, check mode = 2
int game_mode=0;

bool isValid(int rows,int columns){
	bool valid = (rows >= 0 && rows < MAX) && 
	(columns >= 0 && columns < MAX);
	return valid;
}

/*This is a recursive function which uncovers blank cells while they are adjacent*/
int uncover_blank_cell(int row, int col) {
    int value, rows[8], columns[8], i;

    if(table_array[row][col] != 0)
        return 0; // error

    table_array[row][col] += 10; // uncover current cell

    // Get position of adjacent cells of current cell
    rows[0] = row - 1;
    columns[0] = col + 1;
    rows[1] = row;
    columns[1] = col + 1;
    rows[2] = row + 1;
    columns[2] = col + 1;
    rows[3] = row - 1;
    columns[3] = col;
    rows[4] = row + 1;
    columns[4] = col;
    rows[5] = row - 1;
    columns[5] = col - 1;
    rows[6] = row;
    columns[6] = col - 1;
    rows[7] = row + 1;
    columns[7] = col - 1;

    for(i = 0; i < 8; i++) {
        value = table_array[rows[i]][columns[i]];
        if( isValid(rows[i],columns[i]))  {	   // to prevent negative index and out of bounds
            if(value > 0 && value <= 8)
                table_array[rows[i]][columns[i]] += 10;										// it is a cell with 1-8 number so we need to uncover
            else if(value == 0)
                uncover_blank_cell(rows[i], columns[i]);
        }

    }

    return 1; // success!
}

void print_table() {
    // clear screen
    system("clear");

    int i, j, value;
    for(i = 0; i < MAX; i++) {
        for(j = 0; j < MAX; j++) {
            if(x == j && y == i) {
                if(game_mode == 1) {
                    printf("|%sF%s",backMagenta,KNRM);
                    continue;
                } else if(game_mode == 2) {
                    printf("|%sC%s",backMagenta,KNRM);
                    continue;
                }

            }
            value = table_array[i][j];

            if((value >= 0 && value <= 8) || value == 0 || value == 99)
                printf("|X");
            else if(value == 10) // clean area
                printf("|%s%d%s",textCyan, value - 10,KNRM);
            else if(value == 11) // the number of near mine is 1
                printf("|%s%d%s",textYellow, value - 10,KNRM);
            else if(value > 11 && value <= 18) // the number of near mine is greater than 1
                printf("|%s%d%s",textRed, value - 10,KNRM);
            else if((value >= 20 && value <= 28) || value == 100)
                printf("|%sF%s",textGreen,KNRM);
            else
                printf("ERROR"); // test purposes

        }
        printf("|\n");
    }

    printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n",textCyan,KNRM,textGreen,KNRM);
    if(game_mode == 0) {
        printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
    } else if(game_mode == 1) {
        printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
    } else if(game_mode == 2) {
        printf("Enter (select to check cell), q (Exit selection): ");
    }


}


int main(int argc, char *argv[]) {

    char ch;
    int numberMines; // the number of the remaining mines
    int i,j,r,c,value, rows[8], columns[8];
new_game:
    // the number of mines
    numberMines = 10;
    if(argc == 2) {
        numberMines = atoi(argv[1]);
    }
    srand (time(NULL));                                         // random seed
    // setting cursor
    x = 0;
    y = 0;
    // set all cells to 0
    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
            table_array[i][j] = 0;

    for(i = 0; i < numberMines; i++) {
        /* initialize random seed: */

        r = rand() % 10;                                        // it generates a integer in the range 0 to 9
        c = rand() % 10;

        // put mines
        if(table_array[r][c] != 99) {
            table_array[r][c] = 99;

            // Get position of adjacent cells of current cell
            rows[0] = r - 1;
            columns[0] = c + 1;
            rows[1] = r;
            columns[1] = c + 1;
            rows[2] = r + 1;
            columns[2] = c + 1;
            rows[3] = r - 1;
            columns[3] = c;
            rows[4] = r + 1;
            columns[4] = c;
            rows[5] = r - 1;
            columns[5] = c - 1;
            rows[6] = r;
            columns[6] = c - 1;
            rows[7] = r + 1;
            columns[7] = c - 1;

            for(j = 0; j < 8; j++) {
                value = table_array[rows[j]][columns[j]];
                if( isValid ) {        // to prevent negative index and out of bounds
                    if(value != 99)                                                                                                                             // to prevent remove mines
                        table_array[rows[j]][columns[j]] += 1;                                                                  // sums 1 to each adjacent cell
                }
            }

        } else {                                                        // to make sure that there are the properly number of mines in table
            i--;
            continue;
        }
    }
    //
    while(numberMines != 0) {			// when nMines becomes 0 you will win the game
        print_table();

        ch = getch();
        // cursor direction
        char direction;
        switch (ch) {

        // flag
        case 'f':
        case 'F':
            game_mode = 1;
            do {
                print_table();
                direction = getch();
                // arrow direction
                if(direction == '8') {
                    // up
                    y = (MAX + --y) % MAX;
                } else if(direction == '2') {
                    // down
                    y = ++y % MAX;
                } else if(direction == '4') {
                    x = (MAX + --x) % MAX;
                } else if(direction == '6') {
                    x = ++x % MAX;
                } else if(direction == 'c' || direction == 'C') {
                    continue;
                } else if(direction == '\n') {
                    value = table_array[y][x];

                    if (value == 99) {				// mine case
                        table_array[y][x] += 1;
                        numberMines -= 1;				// mine found
                    } else if(value >= 0 && value <= 8) {	// number of mines case (the next cell is a mine)
                        table_array[y][x] += 20;
                    } else if(value >= 20 && value <= 28) {
                        table_array[y][x] -= 20;
                    }

                    if(numberMines == 0)
                        break;
                }
            } while (direction != 'q' && direction != 'Q');
            game_mode = 0;

            break;

        // check cell
        case 'c':
        case 'C':
            game_mode = 2;
            do {
                print_table();
                direction = getch();

                // arrow direction
                if(direction == '8') {
                    // up
                    y = (MAX + --y) % MAX;
                } else if(direction == '2') {
                    // down
                    y = ++y % MAX;
                } else if(direction == '4') {
                    x = (MAX + --x) % MAX;
                } else if(direction == '6') {
                    x = ++x % MAX;
                } else if(direction == 'f' || direction == 'F') {
                    continue;
                }

                else if(direction == '\n') {
                    value = table_array[y][x];
                    if(value == 0)						// blank case
                        uncover_blank_cell(y, x);
                    else if(value == 99)				// mine case
                        continue;
                    else if(value > 0 && value <= 8)	// number case (the next cell is a mine)
                        table_array[y][x] += 10;

                    //	break;
                }
            } while (direction != 'q' && direction != 'Q');
            game_mode = 0;

            break;



        // jump to a new game
        case 'n':
        case 'N':
            goto new_game;
            break;

        // exit
        case 'q':
        case 'Q':
                game_mode = 0;
		print_table();
    		printf("\nGAME OVER\n");

    		if(numberMines == 0)
        		printf("you won!!!!\n");

    		else
        		printf("BOOM! you LOOSE!\n");

    		do {
        	    printf("Are you sure to exit? (y or n)? ");
                	ch = getch();
        		putchar('\n');
        		if(ch == 'y' || ch == 'Y') {
            			break;
        		} else if(ch == 'n' || ch == 'N') {
           			goto new_game;
				break;
       			}
                	 printf("Please answer y or n\n");
   		 } while(1);
   		 printf("See you next time!\n");

		 fflush(stdin);
		 break;
    		

        default:
            break;
        }
	break;
    }
    return 0;
}
