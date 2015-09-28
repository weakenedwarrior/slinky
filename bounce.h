/*
  bounce.h - A lightrun where the light bounces off the LED array.
 
*/

#ifndef bounce_h
#define bounce_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "lightrun.h"


class Bounce : public Lightrun {
  public:
    Bounce(Adafruit_NeoPixel * strip, uint32_t color);
    bool isDone();
    
  protected:
    int _bounces;
    int _inc;
    void _incrementState();
};

#endif
