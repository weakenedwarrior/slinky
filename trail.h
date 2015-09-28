/*
  trail.h - A lightrun where the light leaves a trail that slowly fades
*/

#ifndef trail_h
#define trail_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "lightrun.h"

class Trail : public Lightrun {
  public:
    Trail(Adafruit_NeoPixel * strip, uint32_t pulsecolor, uint32_t fadecolor);
    bool isDone();
    
  protected:
    uint32_t _fadecolor;
    int _fade_cycles;
    void _incrementState();
};

#endif
