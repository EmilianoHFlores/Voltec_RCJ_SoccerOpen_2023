#ifndef COMPASS_H
#define COMPASS_H

class Compass {
  public:
    Compass ();
    void init ();
    void reset();
    void test ();
    void begin ();
    
    float checkAngle ();       
};

#endif
