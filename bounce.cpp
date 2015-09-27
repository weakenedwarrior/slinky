/*
  bounce.cpp - Create a light run that bounces from the end
*/

#include "bounce.h"

Bounce::Bounce(Adafruit_NeoPixel * pstrip, uint32_t color) : Lightrun(pstrip, color) { 
    _bounces = 0;   
    //Serial.println("Started Bounce" );   
}

void Bounce::_incrementState() {
  if (_current > _end) {
    _bounces += 1;
    _dir = -1;
  } else if (_current < _start) {
    _bounces += 1;
    _dir = +1;
  }     
  _current += _dir;
}

bool Bounce::isDone() {
    return _bounces >= 4; 
}
