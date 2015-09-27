/*
  lightrun.cpp - A Library for creating lightruns
*/
#include "lightrun.h"


Lightrun::Lightrun(Adafruit_NeoPixel * pstrip) { //the constructor
      _pstrip = pstrip;
      _color = pstrip->Color(255, 0, 0);
      _init();
}

Lightrun::Lightrun(Adafruit_NeoPixel * pstrip, uint32_t color) { //the constructor
      _pstrip = pstrip;
      _color = color;
      _init();
}

void Lightrun::_init() {
  _start = 0;
  _end = 27;
  _current = 0;
  _started = false;
}

bool Lightrun::isDone() {
    return _current > _end; 
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
  uint32_t current_color = _pstrip->getPixelColor(_current);
  uint32_t old_color = _colorsubtract(current_color, _color);
  _pstrip->setPixelColor(_current, old_color);
}

void Lightrun::_incrementState() {
  _current += 1;
}

void Lightrun::_setNewState() {
  uint32_t current_color = _pstrip->getPixelColor(_current);
  uint32_t new_color = _coloradd(current_color, _color);
  _pstrip->setPixelColor(_current, new_color);
}

uint32_t Lightrun::_colorsubtract(uint32_t colorA, uint32_t colorB) {
    uint32_t newred = (0xff + _getColorByte(colorA, 2) - _getColorByte(colorB, 2)) % 0xff;
    uint32_t newgreen = (0xff + _getColorByte(colorA, 1) - _getColorByte(colorB, 1)) % 0xff;
    uint32_t newblue = (0xff + _getColorByte(colorA, 0) - _getColorByte(colorB, 0)) % 0xff;
    return _pstrip->Color(newred, newgreen, newblue);
}

uint32_t Lightrun::_coloradd(uint32_t colorA, uint32_t colorB) {
    uint32_t red = (_getColorByte(colorA, 2) + _getColorByte(colorB, 2)) % 0xff;
    uint32_t green = (_getColorByte(colorA, 1) + _getColorByte(colorB, 1)) % 0xff;
    uint32_t blue = (_getColorByte(colorA, 0) + _getColorByte(colorB, 0)) % 0xff;
    return _pstrip->Color(red, green, blue);
}

uint32_t Lightrun::_getColorByte(uint32_t fullcolor, int index) {
    uint32_t colorpiece = (fullcolor >> (8*index)) & 0xff;    
    return colorpiece;
}



