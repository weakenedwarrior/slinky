
#include "jumpy.h"

Jumpy::Jumpy(Adafruit_NeoPixel * pstrip, uint32_t color) : Lightrun(pstrip, color) { 
    _pattern = JUMPY;
    _jumps = 0;  
    _current = (_start + _end)/2; 

    int vsize = _colorvector.size();
    int mid = vsize/2;

    for (int i = 0; i < mid; i++) {
      uint32_t newcolor = _colorscale(color, 1/float(1<<(mid-i)));
      _colorvector[i] = newcolor;
      _colorvector[vsize - i] = newcolor;
    }
    

    
}

void Jumpy::_incrementState() {
  
    // Fade for each bounce
   // for (int i = 0; i < _colorvector.size(); i++) {
    //  _colorvector[i] = _colorscale(_colorvector[i], 0.5);
    //}    
      
  _current = (_current + random(3) - random(3)) % _end;

  _jumps += 1;
}

bool Jumpy::isDone() {
    return _jumps >= 100; 
}
