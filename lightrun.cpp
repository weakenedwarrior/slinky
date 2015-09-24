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
      _started = false;
      _pstrip = pstrip;
}

bool Lightrun::isDone() {
    return _current > _end; 
}

void Lightrun::moveToNext() {
     if (_started) {
      _undoCurrentColor();  
      _current += 1;
    } else {
      _started = true;
    }

    if (!isDone()) {
      _setNewColor();
    }  

    //Serial.print("Current val = ");
    //Serial.println(_current);
    
}


void Lightrun::_undoCurrentColor() {

 // uint32_t currentcolor = _pstrip->getPixelColor(_current);
 //   uint32_t newcolour = wrapsub(currentcolor, color);
  uint32_t black = _pstrip->Color(0, 0, 0);
  _pstrip->setPixelColor(_current, black);

  
}


void Lightrun::_setNewColor() {
  uint32_t red = _pstrip->Color(0, 255, 150);
  _pstrip->setPixelColor(_current, red);
}


