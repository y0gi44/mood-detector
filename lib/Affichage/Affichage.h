#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <Arduino.h>


struct 
{
    byte pin;
    long delaiOn;
    long delaiOff;
}typedef ledPinDefinition;


class Affichage {

public:
    Affichage(){};
    
    void initAffichage(bool debug = false);
    void eteindreToutesLesLed();
    void afficherVotesPrisEnCompte(byte ledpin);
    void afficherVotesPrisEnCompte();
    void afficherErreurDeSaisie();
    void afficherInitEnCours();

    void processAffichage();
    
private:
    ledPinDefinition TableauSequence[50] ;

    int currentIndex = -1;
    int maxIndexSequenceEnMemoire = 0;
    unsigned long timetoLightOff = 0;
    unsigned long timetoGoNext = 0;

    void allumerLedEtMajTemps();
    bool debug;
};

#endif // AFFICHAGE_H
