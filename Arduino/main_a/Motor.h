#ifndef MOTOR_H
#define MOTOR_H

#include "Compass.h"
#include "Camera.h"
#include "Pid.h"

class Motor {
  private:
    struct motor {
      byte a;
      byte b;
      byte pwm;
      int speedDeviation;
      byte originalA;
      byte originalB;
    };

    motor _NE = {0, 0, 0, 0, 0, 0};
    motor _NW = {0, 0, 0, 0, 0, 0};
    motor _SE = {0, 0, 0, 0, 0, 0};
    motor _SW = {0, 0, 0, 0, 0, 0};

    Camera cameraMotor;
    Compass* compass;
    Pid pid;
    Pid::PidPackage compassP;
    Pid::PidPackage stability;

    template <typename T>
    void format(T data, int spaces) {
      if(String(data).length() < spaces) printSpaces(spaces - String(data).length());
    };
    void printSpaces(int spaces) {
      for (int i = 0; i < spaces; i++) {
        Serial.print(" ");
      }
    }

    unsigned long long previousRefresh = 0;
  public:
    Motor ();
    void begin (byte NWpwm, byte NWa, byte NWb, byte NEpwm, byte NEa, byte NEb, byte SWpwm, byte SWa, byte SWb, byte SEpwm, byte SEa, byte SEb);
    void attachCompass (Compass * c);
    // void attachQrd (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4);
    void active (bool state);


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
    void Stop();  

    // Complex movement functions
    void moveToAngle(int initAngle, int angle, int speed);
    void moveToAngle(int initAngle, int angle, int speed, int d);
    void rotateToAngle(float initAngle, int destinyAngle, bool stop);

    // Pid
    float pidCalculate(float input, float error, float target);
    void pidNorth(int initAngle, int speed);
    void pidSouth(int initAngle, int speed);
    void pidWest(int initAngle, int speed);
    void pidEast(int initAngle, int speed);

    void wardOff();

    int rotation; 
    void fNW(int id, int speed);
    void fNW(int speed);
    void fNE(int id, int speed);
    void fNE(int speed);
    void fSW(int id, int speed);
    void fSW(int speed);
    void fSE(int id, int speed);
    void fSE(int speed);

    Compass helper;
};

#endif