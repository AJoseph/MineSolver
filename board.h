#ifndef BOARD_H
#define BOARD_H

#include "block.h"

#define MAXROW 9
#define MAXCOL 9
#define MAXMINE 10

/*
The board has MAXROW * MAXCOL blocks. And there are MAXMINE mines.
WINCOUNT is the number of blocks that are to be opened in the board i.e.
((MAXROW * MAXCOL)-MAXMINE)
*/
#define WINCOUNT ((MAXROW * MAXCOL) - MAXMINE)

class board
{
    public:

        board() //constructor
        {
            openblock_count = 0;
            minehit = false;
        }

        void displayboard(); //display board

        void populateboard(); //populate the mines and the numbers after placing the mines

        bool makemove(char opt, int row, int col); //make a particular move on the board.

        void displaybareboard(); //display the board with open blocks - for debugging

        bool checkwin(); //check if the player has won the game

        void openblank(int row, int col); //if player opens a blank, open up all blanks adjacent to it

        bool getminehit() //check if the player has hit a mine using this variable
        {
            return minehit;
        }

        void setminehit() //set the minehit to make the game know that player has hit a mine
        {
            minehit = true;
        }

        int getstatus(int row, int col) //to get the status of a particular block
        {
            return g_board[row][col].getstatus();
        }

        int getvalue(int row, int col)
        {
                return g_board[row][col].getvalue(); //will return -1 if closed or value if opened.
        }

        int getclue(int row, int col)
        {
                return g_board[row][col].getclue(); //will return -1 if closed or value if opened.
        }

    private:

        block g_board[MAXROW][MAXCOL];
        int openblock_count;
        bool minehit;


};

#endif // BOARD_H
