/*
  jumpy.h - A lightrun where the light appears randomly for a period and then fades
*/

#ifndef jumpy_h
#define jumpy_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "lightrun.h"

class Jumpy : public Lightrun {
  public:
    Jumpy(Adafruit_NeoPixel * strip, uint32_t color);
    bool isDone();
    
  protected:
    uint32_t _color;
    int _jumps;
    void _incrementState();
};

#endif
