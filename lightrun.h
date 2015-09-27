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
    virtual bool isDone();
    void moveToNext();
    
  protected:
    int _start;
    int _end;
    int _current;
    bool _started;
    uint32_t _color;
    
    Adafruit_NeoPixel * _pstrip;

    void _init();

    uint32_t _colorsubtract(uint32_t, uint32_t);
    uint32_t _coloradd(uint32_t, uint32_t);
    uint32_t _getColorByte(uint32_t fullcolor, int index);
    
    virtual void _undoCurrentState();
    virtual void _setNewState();
    virtual void _incrementState();


};





#endif
