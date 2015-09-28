/*
  lightrun.h - A Library for creating lightruns
  A lightrun is a sequence of color changes that give the perception of
  light travelling along a string of LEDs
*/

#ifndef lightrun_h
#define lightrun_h

#include <StandardCplusplus.h>
#include <vector>
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

enum LightPattern : int { LIGHTRUN, BOUNCE, TRAIL };

class Lightrun {
  public:
    Lightrun(Adafruit_NeoPixel *);
    Lightrun(Adafruit_NeoPixel *, uint32_t);
    int getPattern();
    virtual bool isDone();
    virtual void moveToNext();
    
  protected:
    int _pattern;
    int _start;
    int _end;
    int _current;
    int _spread;
    int _dir;
    bool _started;
    uint32_t _color;
    std::vector<uint32_t> _colorvector;
    Adafruit_NeoPixel * _pstrip;
    
    virtual void _incrementState();
    void _init();

    uint32_t _colorsubtract(uint32_t, uint32_t);
    uint32_t _coloradd(uint32_t, uint32_t);
    uint32_t _colorscale(uint32_t color, float scale);
    uint32_t _getColorByte(uint32_t fullcolor, int index);
    
    void _undoCurrentState();
    void _setNewState();
    
    void _undoPixel(int pixel, uint32_t color);
    void _setPixel(int pixel, uint32_t color);
};





#endif
