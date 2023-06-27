#include "pixy.h"
#include <Wire.h>

Pixy::Pixy () {}

void Pixy::begin () {
    Wire.begin();
    pixy.init();
}

void Pixy::getBlocks () {
    uint16_t blocks = pixy.getBlocks();
    _ox = -1;
    _oy = -1;
    _yx = -1;
    _yy = -1;
    _bx = -1;
    _by = -1;
    if (blocks) {
        
        for (int i = 0; i < blocks; i++) {
            if (ballId == pixy.blocks[i].signature) {
                _ox = pixy.blocks[i].x + pixy.blocks[i].width / 2;
                _oy = pixy.blocks[i].y + pixy.blocks[i].height / 2;
            }
            if (yellowId == pixy.blocks[i].signature) {
                _yx = pixy.blocks[i].x + pixy.blocks[i].width / 2;
                _yy = pixy.blocks[i].y + pixy.blocks[i].height / 2;
            }
            if (blueId == pixy.blocks[i].signature) {
                _bx = pixy.blocks[i].x + pixy.blocks[i].width / 2;
                _by = pixy.blocks[i].y + pixy.blocks[i].height / 2;
            }
        }
    };
}