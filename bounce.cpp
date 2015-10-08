/*
  bounce.cpp - Create a light run that bounces from the end
*/

#include "bounce.h"

Bounce::Bounce(Adafruit_NeoPixel * pstrip, uint32_t color) : Lightrun(pstrip, color) { 
    _pattern = BOUNCE;
    _bounces = 0;   
    //Serial.println("Started Bounce" );   
}

void Bounce::_incrementState() {
  if (_current > _end || _current < _start) {
    _bounces += 1;
    _dir = -_dir; 

    // Fade for each bounce
    for (int i = 0; i < _colorvector.size(); i++) {
      _colorvector[i] = _colorscale(_colorvector[i], 0.5);
    }    
  }    
  _current += _dir;
  Serial.println(_current);
}

bool Bounce::isDone() {
    return _bounces >= 10; 
}
