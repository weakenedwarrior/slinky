/*
  doublelightrun.h - A lightrun that spans 2 LED strips.
 
*/

#ifndef doublelightrun_h
#define doublelightrun_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "lightrun.h"


class DoubleLightrun : public Lightrun {
  public:
    DoubleLightrun(Adafruit_NeoPixel * pstrip, uint32_t color);
    
  protected:
    void _init();
    void _undoPixel(int pixel, uint32_t color);
    void _setPixel(int pixel, uint32_t color);
    
};

#endif
