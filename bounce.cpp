/*
  bounce.cpp - Create a light run that bounces from the end
*/

#include "bounce.h"

Bounce::Bounce(Adafruit_NeoPixel * pstrip, uint32_t color) : Lightrun(pstrip, color) { 
    _bounces = 0;
    _inc = 1;   
    //Serial.println("Started Bounce" );   
}


/*void Bounce::moveToNext() {
     if (_started) {
      _undoCurrentState(); 
      _incrementState();
    } else {
      _started = true;
    }

    if (!isDone()) {
      _setNewState();
    }  
    Serial.print("Bounce:moveToNext " ); 
    Serial.println(_current);
    
}*/


void Bounce::_incrementState() {
  if (_current > _end) {
    _inc = -1;
    _bounces += 1;
  } else if (_current < _start) {
    _inc = +1;
    _bounces += 1;
  }     
  _current += _inc;
}

bool Bounce::isDone() {
    return _bounces >= 4; 
}
