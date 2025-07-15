#include "Affichage.h"

byte green_led_pin = 13;
byte yellow_led_pin = 14;
byte red_led_pin = 12;

void Affichage::initAffichage(bool debug ){
  // Initialize the output variables as outputs
  pinMode(green_led_pin, OUTPUT);
  pinMode(yellow_led_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  
  this->eteindreToutesLesLed();
  this->debug = debug;
  Serial.println("Affichage initialisé");
  if (debug)
    Serial.println("Affichage en mode debug");
}

void Affichage::eteindreToutesLesLed(){
// Set outputs to LOW
  digitalWrite(red_led_pin, LOW);
  digitalWrite(yellow_led_pin, LOW);
  digitalWrite(green_led_pin, LOW);
  
}

void Affichage::afficherVotesPrisEnCompte(){
    Serial.println("Init de la séquence d'affichage des votes pris en compte");
    int index = 0;
    ledPinDefinition green_led_ok = 
    TableauSequence[index++] = {green_led_pin, 100, 100};
    TableauSequence[index++] = {green_led_pin, 100, 100};
    TableauSequence[index++] = {green_led_pin, 100, 100};
    TableauSequence[index++] = {green_led_pin, 500, 100};
    
    currentIndex = 0;
    maxIndexSequenceEnMemoire = index - 1;
    this->eteindreToutesLesLed();
    allumerLedEtMajTemps();
}

void Affichage::afficherVotesPrisEnCompte(byte ledPin){
    Serial.println("Init de la séquence d'affichage des votes pris en compte avec LedPin : " + String(ledPin));
    int index = 0;
    ledPinDefinition green_led_ok = 
    TableauSequence[index++] = {ledPin, 100, 100};
    TableauSequence[index++] = {ledPin, 100, 100};
    TableauSequence[index++] = {ledPin, 100, 100};
    TableauSequence[index++] = {ledPin, 300, 100};
    
    currentIndex = 0;
    maxIndexSequenceEnMemoire = index - 1;
    this->eteindreToutesLesLed();
    allumerLedEtMajTemps();
}

void Affichage::afficherErreurDeSaisie(){
    Serial.println("Init de la séquence d'affichage erreur de saisie");
    int index = 0;
    ledPinDefinition green_led_ok = 
    TableauSequence[index++] = {red_led_pin, 100, 100};
    TableauSequence[index++] = {yellow_led_pin, 100, 100};
    TableauSequence[index++] = {green_led_pin, 100, 100};
        
    currentIndex = 0;
    maxIndexSequenceEnMemoire = index - 1;
    this->eteindreToutesLesLed();
    allumerLedEtMajTemps();
}

void Affichage::afficherInitEnCours(){
    Serial.println("Init de la séquence d'affichage init en cours");
    int index = 0;
    ledPinDefinition green_led_ok = 
    TableauSequence[index++] = {red_led_pin, 100, 100};
    TableauSequence[index++] = {yellow_led_pin, 100, 100};
    TableauSequence[index++] = {green_led_pin, 100, 100};
    TableauSequence[index++] = {yellow_led_pin, 100, 100};
    TableauSequence[index++] = {red_led_pin, 100, 100};
    TableauSequence[index++] = {yellow_led_pin, 100, 100};
    TableauSequence[index++] = {green_led_pin, 100, 100};
    TableauSequence[index++] = {green_led_pin, 200, 200};
    TableauSequence[index++] = {green_led_pin, 300, 100};
    
    currentIndex = 0;
    maxIndexSequenceEnMemoire = index - 1;
    this->eteindreToutesLesLed();  
    allumerLedEtMajTemps();
}

void Affichage::processAffichage() {
    // si l'index est à -1, on a rien a afficher
    if (currentIndex < 0) {
        //if (debug)
        //    Serial.println("Aucun affichage en cours");
        return;
    }

    // on est arrivé à la fin de la séquence
    if (currentIndex >= maxIndexSequenceEnMemoire) {
        // on reset l'index pour recommencer la séquence
        currentIndex = -1;
        if (debug)
            Serial.println("Fin de la séquence d'affichage");
        return;
    }


    unsigned long currentTime = millis();
    if (timetoGoNext < currentTime) {
        // on passe à l'index suivant
        if (debug)
            Serial.println("incrément de l'index : " + String(currentIndex));
        currentIndex++;
        
        // si on a une autre led à afficher
        if (currentIndex < maxIndexSequenceEnMemoire) {
            allumerLedEtMajTemps();
        }
    } else if (timetoLightOff  < currentTime) {
        // on éteint la led
        digitalWrite(TableauSequence[currentIndex].pin, LOW);
        if (debug)
            Serial.println("Extinction de la led : " + String(TableauSequence[currentIndex].pin));
    }
}

void Affichage::allumerLedEtMajTemps() {
    if (debug)
        Serial.println("Allumage de la led : " + String(TableauSequence[currentIndex].pin));
    unsigned long currentTime = millis();
    // on allume la led associée à l'index courant
    digitalWrite(TableauSequence[currentIndex].pin, HIGH);
    // on met à jour le temps pour éteindre la led
    timetoLightOff = currentTime + TableauSequence[currentIndex].delaiOn;
    // on met à jour le temps pour passer à l'index suivant
    timetoGoNext = timetoLightOff + TableauSequence[currentIndex].delaiOff;
}