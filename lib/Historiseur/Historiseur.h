#ifndef HISTORISEUR_H
#define HISTORISEUR_H

#include <Arduino.h>

#define MAX_ITEMS_HISTORY 60

template <class T>


class Historiseur {
private:   
    T items[MAX_ITEMS_HISTORY];
    String datesEnregistrement[MAX_ITEMS_HISTORY];
    int currentItem;

public:
    Historiseur() : currentItem(-1) {};

    void recordItem(T item, String dateEnregistrement){
        currentItem++;
        if (currentItem == MAX_ITEMS_HISTORY){
            currentItem = 0;
        }

        this->items[currentItem] = item;
        datesEnregistrement[currentItem] = dateEnregistrement;
        
    };

    T getCurrentItem(){ return currentItem >= 0 ? items[currentItem] : T(); };
    String getCurrentDateEnregistrement(){ return currentItem >= 0 ? datesEnregistrement[currentItem] : String(); };
    T getItem(int index){ return (index >= 0 && index < MAX_ITEMS_HISTORY) ? items[index] : T(); };
    String getDateEnregistrement(int index){ return (index >= 0 && index < MAX_ITEMS_HISTORY) ? datesEnregistrement[index] : String(); };
    int getCurrentItemIndex(){ return currentItem; };
    int getItemsCount(){ return MAX_ITEMS_HISTORY; };

};

#endif