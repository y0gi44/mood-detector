#include "KeyBoard.h"

// Clavier matriciel 4x4
const byte ROWS = 4; 
const byte COLS = 4; 
// Definition des touches du clavier
char keys[ROWS][COLS] = {
{'1','2','3', 'A'},
{'4','5','6', 'B'},
{'7','8','9', 'C'},
{'*','0','#', 'D'}
};

// Configuration des pin du GPIO pour lignes et colonnes du clavier 
byte pin_rows[ROWS]   = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLS] = {16, 4, 2, 15};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

// Creation de l'objet Keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROWS, COLS );

// Fonction de Gestion de l'evenement du clavier
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      Serial.print("   => PRESSED : ");
      Serial.println(key);
      break;

    case RELEASED:
      Serial.print("   => RELEASED : ");
      Serial.println(key);
      
      break;

    case HOLD:
      Serial.print("   => HOLD : ");
      Serial.println(key);
      
      break;
  }
}

void Keyboard::initKeyboard(bool debug ) {
  if (debug) {
  // Initialisation du clavier
    keypad.addEventListener(keypadEvent);
  }
  Serial.println("Clavier initialisé");
}

char Keyboard::recupererTouchePresseOu_(){
    String touches = "";
    for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
    {
        if ( keypad.key[i].stateChanged && keypad.key[i].kstate == PRESSED )   // Only find keys that have changed state.
        {
            touches += keypad.key[i].kchar;
        }
    }
    
    if (touches.length() == 1)
    {
      
        return touches.charAt(0);
    }
    else
    {
        // gestion des touches pourries que je comprend pas pourquoi il en sort 4 quand j'en appuie sur une seule touche pour la  3 6 et # 
        if (touches.indexOf('3') >= 0)
        {
            return '3'; 
        }
        else if (touches.indexOf('6') >= 0)
        {
            return '6'; 
        }
        else if (touches.indexOf('#') >= 0)
        {
            return '#'; 
        }
        return '_';
    }
}

char Keyboard::gererTouches(){
    // lecture des touches du clavier
    if(keypad.getKeys())
    {
        return recupererTouchePresseOu_();
    }
    return '_'; // Pas de touche pressée

}

