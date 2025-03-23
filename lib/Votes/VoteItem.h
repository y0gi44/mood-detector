#ifndef VOTE_ITEM_H
#define VOTE_ITEM_H

#include <Arduino.h>

#define MAX_MOODS 3

class VoteItem {
    public:
        VoteItem(){};
        void setItemName(String name){ this->itemName = name; };
        void resetVotes(){ for (int i = 0 ; i < MAX_MOODS; i ++ ) mood[i] = 0; };
        void incrementVotes(int key){ this->mood[key]++; };
        
        String getItemName(){ return this->itemName; };
        uint8_t getMoods(int key){ return this->mood[key]; };

    private:
        String itemName;
        uint8_t mood[MAX_MOODS];
};

#endif