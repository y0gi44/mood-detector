#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>


#include <Keypad.h>



class Keyboard {
    public:
        Keyboard(){};

    void initKeyboard(bool debug = false);
    void gererToucheCandidat(char c);
    char gererTouches();
    
private:
    char recupererTouchePresseOu_( );


};

#endif