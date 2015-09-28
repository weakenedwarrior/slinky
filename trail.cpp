/*
  trail.cpp - Create a light run with a main pulse and a fading trail
*/

#include "trail.h"

Trail::Trail(Adafruit_NeoPixel * pstrip, uint32_t pulsecolor, uint32_t fadecolor) : Lightrun(pstrip, pulsecolor) { 
    _fadecolor = fadecolor;
    _cycles_per_inc = 4; 
    _fade_cycles = 100;
    
    for (int i = 1; i < _spread; i++) {
      uint32_t newcolor = _coloradd(_fadecolor, _colorvector[i]);
      _colorvector[i] = newcolor;
    }
     
}

void Trail::_incrementState() { 
  if (_current <= _end + _spread) {
    _current += 1;
    _colorvector.push_back(_fadecolor);
  } else {

    for (int i = 0; i < _colorvector.size(); i++) {
      _colorvector[i] = _colorscale(_colorvector[i], 0.9);
    }
    _fade_cycles -= 1;
  }
}

bool Trail::isDone() {
    if (_fade_cycles <= 0) {
      trailOK = true;  // This is a global!!
      return true;
    } else {
      return false;
    } 
}
