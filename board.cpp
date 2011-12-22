#include "board.h"
#include <ctime> //for time function in srand
#include <cstdlib> //for srand function

/*
This function is to display the board in blocks.
If there is a mine hit, then the blocks so far open are open and the mines are opened.
*/
void board::displayboard()
{
    for (int i = 0; i < MAXROW; i++)
    {
        std::cout << std::endl << std::endl;
        for (int j=0; j < MAXCOL; j++)
        {
            std::cout << '\t';
            g_board[i][j].display(minehit);
        }
    }
    std::cout << std::endl << std::endl;
}

/*
Function that randomizes row column to place mines on the board.
*/
void board::populateboard()
{
    srand((unsigned)time (NULL));
    for (int i = 0; i < MAXMINE; i++)
    {
        bool setprev = false;
        while (!setprev)
        {
            int row = rand()%9;
            int col = rand()%9;
            if (g_board[row][col].getvalue() == WTF) //randomize to previous set location, loop back.
                setprev = false;
            else
            {
                g_board[row][col].setvalue(WTF);
                std::cout << "row = " << row << " col = " << col << std::endl; //debug - mine position

                /*
                For every mine that is set, add +1 to the clue of the 8 blocks surrounding it, unless there is
                a mine already in that block. Also set that block's b_value to QUESTION.
                */
                if (((row-1)>=0) && (g_board[row-1][col].getvalue() != WTF))
                {
                    g_board[row-1][col].clueincrement();
                    g_board[row-1][col].setvalue(QUESTION);
                }
                if (((row-1)>=0) && ((col+1)<MAXCOL) && (g_board[row-1][col+1].getvalue() != WTF))
                {
                    g_board[row-1][col+1].clueincrement();
                    g_board[row-1][col+1].setvalue(QUESTION);
                }
                if (((col+1)<MAXCOL) && (g_board[row][col+1].getvalue() != WTF))
                {
                    g_board[row][col+1].clueincrement();
                    g_board[row][col+1].setvalue(QUESTION);
                }
                if (((row+1)<MAXROW) && ((col+1)<MAXCOL) && (g_board[row+1][col+1].getvalue() != WTF))
                {
                    g_board[row+1][col+1].clueincrement();
                    g_board[row+1][col+1].setvalue(QUESTION);
                }
                if (((row+1)<MAXROW) && (g_board[row+1][col].getvalue() != WTF))
                {
                    g_board[row+1][col].clueincrement();
                    g_board[row+1][col].setvalue(QUESTION);
                }
                if (((row+1)<MAXROW) && ((col-1)>=0) && (g_board[row+1][col-1].getvalue() != WTF))
                {
                    g_board[row+1][col-1].clueincrement();
                    g_board[row+1][col-1].setvalue(QUESTION);
                }
                if (((col-1)>=0) && (g_board[row][col-1].getvalue() != WTF))
                {
                    g_board[row][col-1].clueincrement();
                    g_board[row][col-1].setvalue(QUESTION);
                }
                if (((row-1)>=0) && ((col-1)>=0) && (g_board[row-1][col-1].getvalue() != WTF))
                {
                    g_board[row-1][col-1].clueincrement();
                    g_board[row-1][col-1].setvalue(QUESTION);
                }
                setprev = true;
            }
        }
    }

    /*
    By default, all blocks are in OPENED status. Now that the mines are set,
    they are reset to CLOSED.
    */
    for (int i = 0; i < MAXROW; i++)
        for (int j = 0; j < MAXCOL; j++)
            g_board[i][j].setstatus(CLOSED);
}



//this function is to set the corresponding block as per the user's input
bool board::makemove(char opt, int row, int col)
{
    bool set;
    switch (opt)
    {
        //case 'U':
        //case 'u':
        //    set = g_board[row][col].unflagblock(); //unflag block
        //    break;

        case 'F':
        case 'f':
            set = g_board[row][col].flagblock(); //flag block
            break;

        case 'O':
        case 'o':
            set = g_board[row][col].openblock();
            if (g_board[row][col].getvalue() == WTF) //open block hits a mine
            {
                setminehit();
                set = false;
            }
            else
                if(g_board[row][col].getvalue() == HAPPY) //open block is a blank
                {
                    openblock_count++;
                    std::cout << "Starting blank..." << std::endl;
                    openblank(row,col);
                }
                else
                    openblock_count++; //open block is a clue.
            break;

        default:
            std::cerr << "Pls enter a valid option for a block" << std::endl;
            set = false;
    }

    return set;
}

/*
Function that checks if the user has won the game.
*/

bool board::checkwin()
{
    if (openblock_count == WINCOUNT)
    {
        std::cout << " You have won the game !!!!" << std::endl;
        return true;
    }
    else
        return false;
}

/*
This function is to recursively open blanks.
It opens a blank block and finds other blank blocks around it and opens them recursively till all blank blocks are open
and also all number blocks surrounding the blanks are open.
*/

void board::openblank(int row, int col)
{
    if ((row-1)>=0)
    {
        if (g_board[row-1][col].getstatus() == CLOSED)
        {
            g_board[row-1][col].openblock();
            openblock_count++;

            if (g_board[row-1][col].getvalue() == HAPPY)
            {
                openblank(row-1,col);
            }
        }
    }

    if (((row-1)>=0) && ((col+1)<MAXCOL))
    {
        if (g_board[row-1][col+1].getstatus() == CLOSED)
        {
            g_board[row-1][col+1].openblock();
            openblock_count++;

            if (g_board[row-1][col+1].getvalue() == HAPPY)
            {
                openblank(row-1,col+1);
            }
        }
    }

    if ((col+1)<MAXCOL)
    {
        if (g_board[row][col+1].getstatus() == CLOSED)
        {
            g_board[row][col+1].openblock();
            openblock_count++;

            if (g_board[row][col+1].getvalue() == HAPPY)
            {
                openblank(row,col+1);
            }
        }
    }

    if (((row+1)<MAXROW) && ((col+1)<MAXCOL))
    {
        if (g_board[row+1][col+1].getstatus() == CLOSED)
        {
            g_board[row+1][col+1].openblock();
            openblock_count++;

            if (g_board[row+1][col+1].getvalue() == HAPPY)
            {
                openblank(row+1,col+1);
            }
        }
    }

    if ((row+1)<MAXROW)
    {
        if (g_board[row+1][col].getstatus() == CLOSED)
        {
            g_board[row+1][col].openblock();
            openblock_count++;

            if (g_board[row+1][col].getvalue() == HAPPY)
            {
                openblank(row+1,col);
            }
        }
    }

    if (((row+1)<MAXROW) && ((col-1)>=0))
    {
        if (g_board[row+1][col-1].getstatus() == CLOSED)
        {
            g_board[row+1][col-1].openblock();
            openblock_count++;

            if (g_board[row+1][col-1].getvalue() == HAPPY)
            {
                openblank(row+1,col-1);
            }
        }
    }

    if ((col-1)>=0)
    {
        if (g_board[row][col-1].getstatus() == CLOSED)
        {
            g_board[row][col-1].openblock();
            openblock_count++;

            if (g_board[row][col-1].getvalue() == HAPPY)
            {
                openblank(row,col-1);
            }
        }
    }

    if (((row-1)>=0) && ((col-1)>=0))
    {
        if (g_board[row-1][col-1].getstatus() == CLOSED)
        {
            g_board[row-1][col-1].openblock();
            openblock_count++;

            if (g_board[row-1][col-1].getvalue() == HAPPY)
            {
                openblank(row-1,col-1);
            }
        }
    }

}



// This function is just for debugging purposes
void board::displaybareboard()
{
    for (int i = 0; i < MAXROW; i++)
    {
        std::cout << std::endl << std::endl;
        for (int j=0; j < MAXCOL; j++)
        {
            std::cout << '\t';
            if (g_board[i][j].getvalue()==WTF)
                std::cout << 'X';
            else
                std::cout << g_board[i][j].getclue();
                //std::cout << 'B';
        }
    }
    std::cout << std::endl << std::endl;
}
