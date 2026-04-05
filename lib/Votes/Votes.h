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

    void load_from_String(String chaine){
        char buf[300];
        chaine.toCharArray(buf, sizeof(buf));
        char *p = buf;
        char *str;
        int i =0;
        while ((str = strtok_r(p, "|", &p)) != NULL && i < MAX_ITEMS_VOTES) {
            this->items[i].load_from_string(str);
            i++;
        }
    }

    String to_string(){
        String s ="";
        for (int i = 0 ; i < MAX_ITEMS_VOTES ; i ++){
            s+=this->items[i].to_string() + "|";
        }
        return s;
    }
};



#endif