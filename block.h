#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <stdexcept>

enum enum_status {CLOSED, OPENED, FLAGGED}; // whether a block is opened, closed or flagged

enum enum_value {HAPPY, WTF, QUESTION};
/*
    HAPPY - Block with a blank
    WTF - Block with a Mine
    QUESTION - Block with a number
*/

class block
{
    public:

        block() //constuctor
        {
            b_status = OPENED;
            b_value = HAPPY;
            clue = 0;
        }

        void display(bool); // display the block based on its status and value

        bool flagblock(); // to flag an unflagged unopened block

        bool unflagblock(); // to unflag an already flagged block

        bool openblock(); // to open a block

        int getvalue()
        {
            if (b_status == OPENED)
                return b_value;
            else
                return -1;
        }

        int getstatus()
        {
            return b_status;
        }

        int getclue()
        {
            if (b_status == OPENED)
                return clue;
            else
                return -1;
        }

        void setvalue(int val)
        {
            b_value = (enum_value)val;
        }

        void setstatus(int sta)
        {
            b_status = (enum_status)sta;
        }

        void setclue(int num)
        {
            clue = num;
        }

        void clueincrement()
        {
            clue++;
        }

    private:

        enum_status b_status;

        enum_value b_value;

        int clue; //the number that appears on the block surrounding a mine


};



#endif // BLOCK_H
