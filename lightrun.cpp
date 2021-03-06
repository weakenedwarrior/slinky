#include "lightrun.h"

Lightrun::Lightrun(Adafruit_NeoPixel * pstrip) : Lightrun(pstrip, pstrip->Color(255, 0, 0)) {}; //the constructor

Lightrun::Lightrun(Adafruit_NeoPixel * pstrip, uint32_t color) { //the constructor
      _pattern = LIGHTRUN;
      _pstrip = pstrip;
      _color = color;
      _init();
}

void Lightrun::_init() {
  _start = 0;
  _end = 50;
  _current = 0;
  _spread = 20;
  _dir = 1;
  _started = false;

  for (int i = 0; i < _spread; i++) {
    //uint32_t scaled_color = _colorscale(_color, 1/float(1<<i));
    uint32_t scaled_color = _colorscale(_color, 1 - float(i)/_spread);
    _colorvector.push_back(scaled_color);
  }
}

int Lightrun::getPattern() {
  return _pattern;
}

bool Lightrun::isDone() {
    return _current > _end + _spread; 
}

void Lightrun::moveToNext() {
     if (_started) {
      _undoCurrentState(); 
      _incrementState();
    } else {
      _started = true;
    }

    if (!isDone()) {
      _setNewState();
    } 
}

void Lightrun::_undoCurrentState() {
  for (int i = 0; i < _colorvector.size(); i ++) {
    _undoPixel(_current - i*_dir, _colorvector[i]); 
  }
}

void Lightrun::_undoPixel(int pixel, uint32_t color) {
  uint32_t current_color = _pstrip->getPixelColor(pixel);
  uint32_t old_color = _colorsubtract(current_color, color);
  _pstrip->setPixelColor(pixel, old_color);
}

void Lightrun::_incrementState() {
  _current += 1;
}

void Lightrun::_setNewState() {
  for (int i = 0; i < _colorvector.size(); i++) {
    _setPixel(_current - i*_dir, _colorvector[i]); 
  }
}

void Lightrun::_setPixel(int pixel, uint32_t color) {
  uint32_t current_color = _pstrip->getPixelColor(pixel);
  uint32_t new_color = _coloradd(current_color, color);
  _pstrip->setPixelColor(pixel, new_color);
}

uint32_t Lightrun::_colorsubtract(uint32_t colorA, uint32_t colorB) {
    uint32_t newred = (0x100 + _getColorByte(colorA, 2) - _getColorByte(colorB, 2)) % 0x100;
    uint32_t newgreen = (0x100 + _getColorByte(colorA, 1) - _getColorByte(colorB, 1)) % 0x100;
    uint32_t newblue = (0x100 + _getColorByte(colorA, 0) - _getColorByte(colorB, 0)) % 0x100;
    return _pstrip->Color(newred, newgreen, newblue);
}

uint32_t Lightrun::_coloradd(uint32_t colorA, uint32_t colorB) {
    uint32_t red = (_getColorByte(colorA, 2) + _getColorByte(colorB, 2)) % 0x100;
    uint32_t green = (_getColorByte(colorA, 1) + _getColorByte(colorB, 1)) % 0x100;
    uint32_t blue = (_getColorByte(colorA, 0) + _getColorByte(colorB, 0)) % 0x100;
    return _pstrip->Color(red, green, blue);
}

uint32_t Lightrun:: _colorscale(uint32_t color, float scale) {
    uint32_t red = int((_getColorByte(color, 2) * scale)) % 0x100;
    uint32_t green = int((_getColorByte(color, 1) * scale)) % 0x100;
    uint32_t blue = int((_getColorByte(color, 0) * scale)) % 0x100;
    return _pstrip->Color(red, green, blue);
}

byte Lightrun::_getColorByte(uint32_t fullcolor, int index) {
    byte colorpiece = (fullcolor >> (8*index)) & 0xff;    
    return colorpiece;
}



