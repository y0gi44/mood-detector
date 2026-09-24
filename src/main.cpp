#include <Arduino.h>
#include "Mood.h"

Mood app;

void setup()
{
  app.init();
}

void loop()
{
  app.loop();
}
