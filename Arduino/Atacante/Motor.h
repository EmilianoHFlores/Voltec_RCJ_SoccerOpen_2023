#ifndef MOTOR_H
#define MOTOR_H

#include "Utils.h"
#include "Compass.h"
#include "Camera.h"

class Motor {
  private:
    byte NWpwm;
    byte NWa;
    byte NWb;
    byte NEpwm;
    byte NEa;
    byte NEb;
    byte SWpwm;
    byte SWa;
    byte SWb;
    byte SEpwm;
    byte SEa;
    byte SEb;
    Utils utilsMotor;
    Compass compassMotor;
    Camera cameraMotor;
  public:
    Motor ();
    void init (byte NWpwm, byte NWa, byte NWb, byte NEpwm, byte NEa, byte NEb, byte SWpwm, byte SWa, byte SWb, byte SEpwm, byte SEa, byte SEb);
    void reset (); // Equivalent to the STOP() function
    void reset (int d);
    void test();
    void begin (byte NWpwm, byte NWa, byte NWb, byte NEpwm, byte NEa, byte NEb, byte SWpwm, byte SWa, byte SWb, byte SEpwm, byte SEa, byte SEb);

    void attachUtils (byte buzzer, int width, int height);
    void attachCompass ();
    // Raw movement functions
    void _NW(int id, int speed);
    void _NW(int speed);
    void _NE(int id, int speed);
    void _NE(int speed);
    void _SW(int id, int speed);
    void _SW(int speed);
    void _SE(int id, int speed);
    void _SE(int speed);

    // Simple movement functions
    void North(int speed);
    void South(int speed);
    void East(int speed);
    void West(int speed);
    void NorthWest(int speed);
    void NorthEast(int speed);
    void SouthWest(int speed);
    void SouthEast(int speed);
    void TurnLeft(int speed);
    void TurnRight(int speed);
  
    void forceStop();
    void hardReset();

    void North(int speed, int d);
    void South(int speed, int d);
    void East(int speed, int d);
    void West(int speed, int d);
    void NorthWest(int speed, int d);
    void NorthEast(int speed, int d);
    void SouthWest(int speed, int d);
    void SouthEast(int speed, int d);
    void TurnLeft(int speed, int d);
    void TurnRight(int speed, int d);
    // Complex movement functions
    void moveToAngle(int initAngle, int angle, int speed);
    void moveToAngle(int initAngle, int angle, int speed, int d);
    void rotateToAngle(float initAngle, int destinyAngle, int speed, bool stop); // TODO: Incomplete
    void rotateToAngle0(float initAngle, int speed, bool stop);

    // bool goalOutRange(int gcy);
};

#endif