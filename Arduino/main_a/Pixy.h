#include "PixyI2C.h"

#ifndef PIXY_H
#define PIXY_H

class Pixy {
  private:
    PixyI2C pixy;
    int _ox;
    int _oy;
    int _yx;
    int _yy;
    int _bx;
    int _by;

    const int ballId = 1;
    const int yellowId = 2;
    const int blueId = 3;
  public:
    Pixy ();
    void begin();
    void getBlocks();

    int ox() { return _ox; }
    int oy() { return _oy; }
    int yx() { return _yx; }
    int yy() { return _yy; }
    int bx() { return _bx; }
    int by() { return _by; }
};

#endif