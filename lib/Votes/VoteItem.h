#ifndef VOTE_ITEM_H
#define VOTE_ITEM_H

#include <Arduino.h>

#define MAX_MOODS 3

class VoteItem {
    public:
        VoteItem(){ resetVotes(); };
        void setItemName(String name){ this->itemName = name; };
        void resetVotes(){ for (int i = 0 ; i < MAX_MOODS; i ++ ) mood[i] = 0; };
        void incrementVotes(int key){
            if (key >= 1 && key <= MAX_MOODS && this->mood[key - 1] < 65535) {
                this->mood[key - 1]++;
            }
        };
        
        String getItemName(){ return this->itemName; };
        uint16_t getMoods(int key){ return (key >= 1 && key <= MAX_MOODS) ? this->mood[key - 1] : 0; };

        void load_from_string(String chaine){
            resetVotes();
            char buf[20];
            if (chaine.length() >= sizeof(buf)) {
                return;
            }
            chaine.toCharArray(buf, sizeof(buf));
            char *p = buf;
            char *str;
            int i =0;
            while ((str = strtok_r(p, ";", &p)) != NULL && i < MAX_MOODS) {
                String token(str);
                if (token.length() == 0) {
                    resetVotes();
                    return;
                }
                for (unsigned int j = 0; j < token.length(); j++) {
                    if (!isDigit(token.charAt(j))) {
                        resetVotes();
                        return;
                    }
                }
                long value = token.toInt();
                if (value < 0 || value > 65535) {
                    resetVotes();
                    return;
                }
                this->mood[i] = static_cast<uint16_t>(value);
                i++;
            }
            if (i != MAX_MOODS) {
                resetVotes();
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