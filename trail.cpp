#include "trail.h"

Trail::Trail(Adafruit_NeoPixel * pstrip, uint32_t pulsecolor, uint32_t fadecolor) : Lightrun(pstrip, pulsecolor) {
    _pattern = TRAIL; 
    _fadecolor = fadecolor;
    _fade_cycles = 100;
    
    for (int i = 1; i < _spread; i++) {
      uint32_t newcolor = _coloradd(_fadecolor, _colorvector[i]);
      _colorvector[i] = newcolor;
    }    
}

void Trail::_incrementState() { 
  // First part: Light pulse with trailing LEDS
  if (_current <= _end + _spread) {
    _current += 1;
    _colorvector.push_back(_fadecolor);
  } 
  
  // Second part: Fade of light trail
  else {
    for (int i = 0; i < _colorvector.size(); i++) {
      _colorvector[i] = _colorscale(_colorvector[i], 0.9);
    }
    _fade_cycles -= 1;
  }
}

bool Trail::isDone() {
    return _fade_cycles <= 0; 
}
