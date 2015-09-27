/*
  bounce.cpp - Create a light run that bounces from the end
*/

#include "bounce.h"

Bounce::Bounce(Adafruit_NeoPixel * pstrip) : Lightrun(pstrip) { 
    _bounces = 0;
    _inc = 1;      
}

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
