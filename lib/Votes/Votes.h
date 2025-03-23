#ifndef VOTES
#define VOTES

#include <Arduino.h>
#include "VoteItem.h"


#define MAX_ITEMS_VOTES 10



class Votes {
private:   
    VoteItem items[MAX_ITEMS_VOTES];
    int itemCount;

public:
    Votes(){};

    void init(String items[], int itemCount){
        this->itemCount = itemCount;
        for (int i = 0; i < itemCount; i++){
            this->items[i].setItemName(items[i]);
            this->items[i].resetVotes();
        }
    };
    VoteItem * getItemName(int index){ return &items[index]; };
};



#endif