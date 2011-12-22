#include "block.h"


void block::display(bool showmine)
{
    //CLOSED BLOCK contains -
    //FLAGGED BLOCK contains F
    //MINE BLOCK contains X
    //BLANK BLOCK contains B
    //All others contain numbers relating to mine information

    if (b_status == CLOSED) //Closed blcok
    {
        if (showmine && (b_value == WTF))
            std::cout << 'X';
        else
            std::cout << '-';
    }
    else
    {
        if(b_status == FLAGGED) //Flagged block
            std::cout << 'F';
        else                    //Open block
        {
            if (b_value == HAPPY)  //Blank block
                std::cout << 'B';
            else
                if(b_value == QUESTION)  //Block with a number
                    std::cout << clue;
                else
                    std::cout << 'X'; // Block with a mine
        }
    }
}

bool block::openblock()
{
    if (b_status == OPENED)
    {
        std::cerr << "Its an already open block. Try another." << std::endl;
        return false;
    }
    else
    {
        if (b_status == FLAGGED)
        {
            std::cerr << "Its a flagged block." << std::endl;
            return false;
        }
        else
        {
            b_status = OPENED;
            std::cout << "Block Opened." <<std::endl;
            return true;
        }
    }
}

bool block::flagblock()
{
    if (b_status == OPENED)
    {
        std::cerr << "Its an already open block. Try another." << std::endl;
        return false;
    }
    else
    {
        if (b_status == FLAGGED)
        {
            std::cerr << "Its a flagged block." << std::endl;
            return false;
        }
        else
        {
            b_status = FLAGGED;
            std::cout << "Block Flagged." <<std::endl;
            return true;
        }
    }
}

bool block::unflagblock()
{
    if (b_status == OPENED)
    {
        std::cerr << "Its an already open block. Try another." << std::endl;
        return false;
    }
    else
    {
        if (b_status == CLOSED)
        {
            std::cerr << "Its a closed block." << std::endl;
            return false;
        }
        else
        {
            b_status = CLOSED;
            std::cout << "Block Unflagged." <<std::endl;
            return true;
        }
    }
}



