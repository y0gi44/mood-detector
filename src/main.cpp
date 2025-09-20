#include <Arduino.h>
#include "Mood.h"
#include "Creneau.h"

Mood app;
//Creneau app;

void setup()
{
  app.init();
}

void loop()
{
  app.loop();
}
