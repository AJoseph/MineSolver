#include "board.h"
#include <cstdlib> //for srand function
#include <vector> //for vector use in saving closed block positions

//structure to store move variables
struct AImove
{
    int row;
    int col;
};

/*
The randommove function goes thru the gameboard and returns the first closed block it encounters.
*/
AImove randommove(board& gb)
{
    AImove rmove;

    bool openprev = false;
    //as long as the block is not opened previously
    while (!openprev)
    {
        int row = rand()%9;
        int col = rand()%9;

        if (gb.getstatus(row,col) == CLOSED) //if the block is closed, send the position back to the main.
        {
            rmove.row = row;
            rmove.col = col;
            std::cout << "Row = " << row << " Col = " << col << std::endl;
            return rmove;
        }
        else
            openprev = false; //block is opened or flagged, so find another one.
    }
}

/*
This function gathers info about surrounding blocks - number of closed blocks and number of flagged blocks
and push closed blocks positions to a vector.
*/
void checkneighbour(board& gb, int row, int col, int& closedcnt, int& flaggedcnt, std::vector<AImove>& closedvec)
{
    if (gb.getstatus(row,col) == CLOSED)
    {
        closedcnt++;

        AImove temp;
        temp.row = row;
        temp.col = col;
        closedvec.push_back(temp); //push position into vector
    }

    if (gb.getstatus(row,col) == FLAGGED)
    {
        flaggedcnt++;
    }
}

/*
This function checks whether the opened block is a mine or a condition to win
the game.
*/

void checkfate(board& gb)
{
    if(gb.getminehit()) //the opened block contained a mine
    {
        std::cout << "You have hit a mine!!!" << std::endl;
        gb.displayboard();
        exit(-1);
    }
    else
    {
        if (gb.checkwin()) //all mines are either flagged or not opened and other blocks are opened
        {
            gb.displayboard();
            exit(0);
        }
    }
}

/*
Main function
*/

int main(int argc, char * argsv[])
{

    board gameboard;

    std::cout << "Minesweeper Game - Solver" << std::endl;

    //Fill mines and the other blocks with respect to the mines
    gameboard.populateboard();

    gameboard.displayboard();

    /*
    boolean to check whether to go thru the board or go for random position.
    first move is always random.
    */
    bool noway = true;

    while (true)
    {
        if (noway) //no other possible move only a random one is needed
        {
            std::cout << "Choosing random position " << std::endl;

            AImove randpos = randommove(gameboard); //get random closed block

            gameboard.makemove('O', randpos.row, randpos.col); //open random block on gameboard

            noway = false; //a move has been made leading to other possibilities

            checkfate(gameboard); //does the move bite the dust or goes for glory ?

            gameboard.displayboard(); //display the board after a move
        }

        /*
        The loop below works as follows
         $ loop until there is no other block to open or run out of options
            for every block in the board going row-wise
            $ get the following (checkneighbour function)
                - clue (if closed block, clue is -1, else 0 or the number)
                - number of closed blocks around it
                - number of flagged blocks around it
            $ if a block is closed, push it into a vector.
            $ compare the clue and the flagged count and closed count. Open or flag blocks based on it
            $ The vector helps to find the closed blocks position to flag or open.

        Do note that the boolean noway will be set to false if a block is opened, that it may help in
        opening other blocks. If no block is opened after one complete pass of the board, then a random
        position is chosen.
        */
        while(!noway)
        {
            noway = true; //set the boolean to no other way possible and if there is a way, set it to false.

            //going row-wise
            for (int i = 0; i < MAXROW; i++)
            {
                for (int j = 0; j < MAXCOL; j++)
                {
                    int a_clue = gameboard.getclue(i,j); //clue in the block
                    int a_closed = 0; //number of closed blocks around it
                    int a_flagged = 0; //number of flags around it

                    std::vector<AImove> closedvec; //vector to hold the closed block positions

                    if (a_clue > 0) //the block is neither closed(-1) nor a blank(0)
                    {
                        if ((i-1)>=0)
                            checkneighbour(gameboard,i-1,j,a_closed,a_flagged,closedvec);

                        if (((i-1)>=0) && ((j+1)<MAXCOL))
                            checkneighbour(gameboard,i-1,j+1,a_closed,a_flagged,closedvec);

                        if ((j+1) < MAXCOL)
                            checkneighbour(gameboard,i,j+1,a_closed,a_flagged,closedvec);

                        if (((i+1)<MAXROW) && ((j+1)<MAXCOL))
                            checkneighbour(gameboard,i+1,j+1,a_closed,a_flagged,closedvec);

                        if ((i+1)<MAXROW)
                            checkneighbour(gameboard,i+1,j,a_closed,a_flagged,closedvec);

                        if (((i+1)<MAXROW) && ((j-1)>=0))
                            checkneighbour(gameboard,i+1,j-1,a_closed,a_flagged,closedvec);

                        if ((j-1)>=0)
                            checkneighbour(gameboard,i,j-1,a_closed,a_flagged,closedvec);

                        if (((i-1)>=0) && ((j-1)>=0))
                            checkneighbour(gameboard,i-1,j-1,a_closed,a_flagged,closedvec);

                        //next line for debugging only

                        std::cout << "Row " << i << " Col " << j << " flag " << a_flagged << " closed " << a_closed << " clue " << a_clue << std::endl;

                        //after gathering required info, compare with clue in the block

                        // the clue and flagged count are same -> the mines surround the block are already flagged, so open closed blocks.
                        if (a_clue == a_flagged)
                        {
                            while (!closedvec.empty())
                            {
                                AImove chosen = closedvec.back(); //get last vector element
                                closedvec.pop_back(); //delete last vector element

                                /*
                                We are dealing only with closed blocks but the OPENED check is there
                                cos sometimes, opening a blank block triggers other blocks to open and
                                they might already be in our vector. So we shouldn't try to open an already
                                opened block.
                                */
                                if (gameboard.getstatus(chosen.row,chosen.col) != OPENED)
                                {
                                    gameboard.makemove('O', chosen.row, chosen.col); //make move on gameboard

                                    noway = false; //a move has been made leading to other possibilities

                                    checkfate(gameboard); //does the move bite the dust or goes for glory ?

                                    gameboard.displayboard(); //display board after making a move
                                }
                            }
                        }
                        else
                        {
                            //the clue is equal to the sum of closed and flagged blocks -> all necessary blocks are open and
                            //closed blocks should be flagged
                            if (a_clue == (a_closed + a_flagged))
                            {
                                while (!closedvec.empty())
                                {
                                    AImove chosen = closedvec.back(); //get last vector element
                                    closedvec.pop_back(); //delete last vector element

                                    gameboard.makemove('F', chosen.row, chosen.col); //make move on gameboard

                                    noway = false; //a move has been made leading to other possibilities

                                    checkfate(gameboard); //does the move bite the dust or goes for glory ?

                                    gameboard.displayboard(); //display board after making a move
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

