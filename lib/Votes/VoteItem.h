#ifndef VOTE_ITEM_H
#define VOTE_ITEM_H

#include <Arduino.h>

class VoteItem {
    public:
        VoteItem();
        void setItemName(String name){ this->itemName = name; };
        void setVotes(uint8_t votes){ this->votes = votes; };
        void resetVotes(){ this->votes = 0; };
        void incrementVotes(){ this->votes++; };
        
        String getItemName(){ return this->itemName; };
        uint8_t getVotes(){ return this->votes; };

    private:
        String itemName;
        uint8_t votes;
};

#endif