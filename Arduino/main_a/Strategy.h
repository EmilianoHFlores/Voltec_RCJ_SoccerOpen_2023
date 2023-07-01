#include <Arduino.h>
#include "buzzer.h"
#include "Camera.h"
#include "Compass.h"
#include "Motor.h"
#include "Oled.h"
#include "Qrd.h"
#include "Ultrasonic.h"
#include "Xbee.h"
#include "Pid.h"
#include "Pixy.h"
#include "Dribbler.h"

#ifndef STRATEGY_H
#define STRATEGY_H

class Strategy {
  private: 
    uint8_t attackAction = 0;
    uint8_t deffendAction = 0;
    int ball_angle = 0;

    uint32_t loopId = 0;
    String TEAM;

    String ballX = "ox";
    String ballY = "oy";
    String goalX;
    String goalY;
    String goalIntercept;

    Pid::PidPackage center;
    Pid::PidPackage deffense;
    Pid::PidPackage score;

    int passiveDeffense_direction = 2; // 0 = Left, 1 = Right, 2 = Stop
    bool ignore_ultrasonic = false;
    bool scoregoal_first = true;
    bool firstPassive = true;
    int _errorThreshold;
  public:
    uint8_t defaultSpeed = 240;
    uint8_t lowestSpeed = 70;
    uint8_t scoreSpeed = 255;
    uint8_t horizontalSpeed = 210;
    uint8_t fastRotationSpeed = 140;
    uint8_t rotationSpeed = 100;
    uint8_t positionSpeed = 90;

    Strategy();
    void begin(String team);
    void attack();
    void deffend();
    bool passiveDeffense();

    void score_goal();

    bool locate_behind();
    bool center_ball();
    void goto_ball(int speed);
    void reset();

    bool checkIntercept();
    int checkWeight();

    void endAction();

    int pixyGoalX() { if (TEAM == "blue") return pixy.bx(); return pixy.yx(); };
    int pixyGoalY() { if (TEAM == "blue") return pixy.by(); return pixy.yy(); };

    template <typename T>
    void format(T data, uint8_t spaces) {
      if(String(data).length() < spaces) printSpaces(spaces - String(data).length());
    };
    void printSpaces(uint8_t spaces) {
      for (uint8_t i = 0; i < spaces; i++) {
        Serial.print(" ");
      }
    }
    float floatMap(float input, float input_start, float input_end, float output_start, float output_end) {
      float slope = 1.0 * (output_end - output_start) / (input_end - input_start);
      return output_start + slope * (input - input_start);
    }
    bool first_iteration = true;
    Motor motor;
    Oled oled;
    Camera camera;
    Compass compass;
    Xbee xbee;
    Buzzer buzzer;    
    Ultrasonic ultrasonic;
    Pid pid;
    Pixy pixy;
    Dribbler dribbler;
};

#endif