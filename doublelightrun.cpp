#include "doublelightrun.h"
                
DoubleLightrun::DoubleLightrun(Adafruit_NeoPixel * pstripA, Adafruit_NeoPixel * pstripB, uint32_t color) : Lightrun(pstripA, color) { 
  _pattern = DOUBLELIGHTRUN;
  _pstripA = pstripA;
  _pstripB = pstripB;
  _end = 100;
}

// Maps pixel into either stripA or stripB as well as pin.
//
//   Pixel #        0-----------49  50-----------99
//
//   Pin #          49-----------0  0------------49
//
//   Strips         |------------|  |-------------|
//                     stripA           stripB
//
void DoubleLightrun::_undoPixel(int pixel, uint32_t color) {
  Adafruit_NeoPixel * pstrip;  
  int pin;
  int mid = _end/2;
  
  if (pixel < mid) {
    pstrip = _pstripA;
    pin = mid - pixel - 1;
  } else {
    pstrip = _pstripB;
    pin = pixel - mid;
  }
  
  uint32_t current_color = pstrip->getPixelColor(pin);
  uint32_t old_color = _colorsubtract(current_color, color);
  pstrip->setPixelColor(pin, old_color);
}

void DoubleLightrun::_setPixel(int pixel, uint32_t color) {
  Adafruit_NeoPixel * pstrip;  
  int pin;
  int mid = _end/2;
  
  if (pixel < mid) {
    pstrip = _pstripA;
    pin = mid - pixel - 1;
  } else {
    pstrip = _pstripB;
    pin = pixel - mid;
  }

  //Serial.print(pixel);
  //Serial.print("\t");
  //Serial.println(pin);
  
  uint32_t current_color = pstrip->getPixelColor(pin);
  uint32_t new_color = _coloradd(current_color, color);
  pstrip->setPixelColor(pin, new_color);
}




