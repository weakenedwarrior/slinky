/*
  lightrun.h - A Library for creating lightruns
  A lightrun is a sequence of color changes that give the perception of
  light travelling along a string of LEDs
*/

#ifndef lightrun_h
#define lightrun_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

#define WAITLEDPIN    13

class Lightrun {
  public:
    Lightrun(Adafruit_NeoPixel *);
    Lightrun(Adafruit_NeoPixel *, uint32_t);
    bool isDone();
    void moveToNext();
    
  private:
    int _start;
    int _end;
    int _current;
    bool _started;
    uint32_t _color;
    
    Adafruit_NeoPixel * _pstrip;

    void _init();
    void _undoCurrentColor();
    void _setNewColor();
    
 
};





#endif
