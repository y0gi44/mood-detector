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
    Votes() : itemCount(0) {};

    void init(String items[], int itemCount){
        this->itemCount = itemCount < MAX_ITEMS_VOTES ? itemCount : MAX_ITEMS_VOTES;
        for (int i = 0; i < this->itemCount; i++){
            this->items[i].setItemName(items[i]);
            this->items[i].resetVotes();
        }
    };
    VoteItem * getItemName(int index){
        return (index >= 0 && index < itemCount) ? &items[index] : nullptr;
    };

    void resetVotes(){
        for (int i = 0; i < itemCount; i++){
            items[i].resetVotes();
        }
    };

    void load_from_String(String chaine){
        resetVotes();
        char buf[300];
        if (chaine.length() >= sizeof(buf)) {
            return;
        }
        chaine.toCharArray(buf, sizeof(buf));
        char *p = buf;
        char *str;
        int i =0;
        while ((str = strtok_r(p, "|", &p)) != NULL && i < itemCount) {
            this->items[i].load_from_string(str);
            i++;
        }
    }

    String to_string(){
        String s ="";
        for (int i = 0 ; i < itemCount ; i ++){
            s+=this->items[i].to_string() + "|";
        }
        return s;
    }
};



#endif