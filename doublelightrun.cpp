#include "doublelightrun.h"
                
DoubleLightrun::DoubleLightrun(Adafruit_NeoPixel * pstrip, uint32_t color) : Lightrun(pstrip, color) { 
      _pattern = LIGHTRUN;
      _pstrip = pstrip;
      _color = color;
      _init();
}

void DoubleLightrun::_init() {
  _start = 0;
  _end = 50;
  _current = 0;
  _spread = 10;
  _dir = 1;
  _started = false;

  for (int i = 0; i < _spread; i++) {
    uint32_t scaled_color = _colorscale(_color, 1/float(1<<i));
    _colorvector.push_back(scaled_color);
  }
}

void DoubleLightrun::_undoPixel(int pixel, uint32_t color) {
  uint32_t current_color = _pstrip->getPixelColor(pixel);
  uint32_t old_color = _colorsubtract(current_color, color);
  _pstrip->setPixelColor(pixel, old_color);
}

void DoubleLightrun::_setPixel(int pixel, uint32_t color) {
  uint32_t current_color = _pstrip->getPixelColor(pixel);
  uint32_t new_color = _coloradd(current_color, color);
  _pstrip->setPixelColor(pixel, new_color);
}



