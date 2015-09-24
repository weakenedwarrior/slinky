/*
  lightrun.cpp - A Library for creating lightruns
*/

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "lightrun.h"


Lightrun::Lightrun(Adafruit_NeoPixel * pstrip) { //the constructor
      _start = 0;
      _end = 27;
      _current = 0;
      _active = false;
      _pstrip = pstrip;
}


void Lightrun::flash() {
     digitalWrite(WAITLEDPIN, HIGH);    
     delay(10);
     digitalWrite(WAITLEDPIN, LOW); 
     delay(10);
     Serial.println(_current);
}
