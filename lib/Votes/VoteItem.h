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
        uint16_t getMoods(int key){ return this->mood[key]; };

        void load_from_string(String chaine){
            char buf[20];
            chaine.toCharArray(buf, sizeof(buf));
            char *p = buf;
            char *str;
            int i =0;
            while ((str = strtok_r(p, ";", &p)) != NULL && i < MAX_MOODS) {
                this->mood[i] = String(str).toInt();
                i++;
            }
        }

        String to_string(){
            String s = "";
            for(int i =0 ;i< MAX_MOODS ; i++)
                s += String(this->mood[i])+";";
            return s;
        }

    private:
        String itemName;
        uint16_t mood[MAX_MOODS];
};

#endif