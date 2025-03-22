#ifndef HISTORISEUR_H
#define HISTORISEUR_H

#include <Arduino.h>

#define MAX_ITEMS 60

template <class T>


class Historiseur {
private:   
    T items[MAX_ITEMS];
    String datesEnregistrement[MAX_ITEMS];
    int currentItem;

public:
    Historiseur();

    void recordItem(T items, String dateEnregistrement){
        currentItem++;
        if (currentItem == MAX_ITEMS){
            currentItem = 0;
        }

        items[currentItem] = items;
        datesEnregistrement[currentItem] = dateEnregistrement;
        
    };

    T getCurrentItem(){ return items[currentItem]; };
    String getCurrentDateEnregistrement(){ return datesEnregistrement[currentItem]; };
    T getItem(int index){ return items[index]; };
    String getDateEnregistrement(int index){ return datesEnregistrement[index]; };
    int getCurrentItemIndex(){ return currentItem; };
    int getItemsCount(){ return MAX_ITEMS; };

};

#endif