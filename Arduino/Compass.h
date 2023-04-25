#include "Oled.h"

#ifndef COMPASS_H
#define COMPASS_H

class Compass {
  private:
    Oled compassOled;
  public:
    Compass ();
    void init ();
    void reset();
    void test ();
    void begin ();
    
    float checkAngle ();       
};

#endif
