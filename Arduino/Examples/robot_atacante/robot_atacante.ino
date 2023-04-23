#include <Wire.h>
#include "Motor.h"
#include "Utils.h"
#include "Camera.h"
#include "Compass.h"
#include "Ultrasonic.h"
#include "Qrd.h"
#include "Oled.h"

#define TEAM "blue"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

#define maxSpeed 255
#define aproachSpeed 225
#define angleSpeed 150
#define rotationSpeed 75
#define positionSpeed 50
#define lowestSpeed 35
#define COMPASS_DEVIATION 15

Motor motor;
Utils utils;
Camera camera;
Compass compass;
Ultrasonic ultrasonic;
Qrd qrd;
Oled oled;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  oled.begin();
  oled.print(10, 10, "Starting...");
  oled.show();

  motor.begin(4, 34, 35, 3, 32, 33, 2, 30, 31, 5, 37, 36);
  motor.attachCompass();
  qrd.attachCompass();
  camera.begin(115200, 1, TEAM);
  utils.begin(53, 320, 240);
  compass.begin();
  //        N1.  N2. N3.   N4.  S1.  S2.  S3.  S4.  E1.  E2.  E3.  E4.  W1.  W2.  W3.  W4.
  qrd.begin(A11, A8, A10,  A9,  A3,  A0,  A2,  A1,  A7,  A4,  A6,  A5,  A15, A12, A14, A13);
  qrd.attachMotors(4, 34, 35, 3, 33, 32, 2, 30, 31, 5, 36, 37);
  motor.attachUtils(53, 320, 240);

  motor.reset(2500);  // Wait for NAVX to turn on TODO: Rewrite this function
};

int j = 0;
int i = 0;
int ocx = -1;
int gcx = -1;
int step = -1;
float angle = -1;
bool whileLoop = true;
/**
 * scenario 0 -> ball is between 0 and 45 degrees
 * scenario 1 -> ball is between 45 and 90 degrees
 * scenario 2 -> ball is between 90 and 135 degrees
 * scenario 3 -> ball is between 135 and 150 degrees
 * scenario 4 -> ball is between 150 and 180 degrees
*/

void loop() {
  if (step == -1) {
    oled.print(0, 0, "Starting loop again in step 0", 2); oled.show();
    motor.hardReset();
    step = 0;
  }

  if (step == 0) {  // Find ball
    Serial.println("Entered step 0");
    ocx = camera.xOrange();
    int preciseLocation = utils.checkPreciseDirection(ocx, 7);
    angle = compass.checkAngle();
    oled.print(105, 0, "S0", 1);
    oled.print(105, 54, i, 1);
    oled.print(0, 20, "ocx: ", 1);
    oled.print(30, 20, ocx, 1);
    oled.print(0, 40, "deg: ", 1);
    oled.print(30, 40, angle, 1);
    oled.show();
    switch (preciseLocation) {
      case 0:
      case 1:
        motor.TurnLeft(lowestSpeed);
        break;
      case 2:
      case 3:
      case 4:
        step = 1;
        motor.hardReset();
        Serial.println("Ended step 0");
        break;
      case 5:
      case 6:
        motor.TurnRight(lowestSpeed);
        break;
      case -1:
        motor.TurnRight(lowestSpeed);
    }
  }

  if (step == 1) {  // Decide strategy
    oled.print(95, 0, "S1", 1);
    oled.print(105, 54, i, 1);
    oled.print(0, 0, "deg: ", 1);
    oled.print(30, 0, angle, 1);
    oled.show();

    if (angle >= 30) step = 2; // Ball between 180 and 30 degrees
    else if (angle <= 0 && angle >= -30) step = 4; // Ball between 0 and -30 degrees
    else step = 3; // Ball between -30 and -180 degrees
  }

  if (step == 2) {
    ocx = camera.xOrange();
    gcx = camera.enemyGoal();
    oled.print(95, 0, "S2", 1); oled.print(105, 54, i, 1);
    oled.print(0, 0, "deg: ", 1); oled.print(30, 0, angle, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, ocx, 1);
    oled.print(0, 40, "gcx: ", 1); oled.print(30, 40, gcx, 1);
    oled.show();

    switch (utils.checkPreciseDirection(ocx, 13)) {
      case 0:
      case 1:
      case 2:
      case 3:
        motor.TurnLeft(lowestSpeed);
        break;
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        if (utils.checkDirection(gcx) == 1) {
          oled.print(0, 0, "TRYING TO SCORE", 3); oled.show();
          motor.moveToAngle(0, maxSpeed, 1000);
          resetVariables();
        } else {
          motor.moveToAngle(60, angleSpeed);
        }
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        motor.TurnRight(lowestSpeed);
        break;
      case -1:
        motor.North(maxSpeed, 1000);
        motor.hardReset();
        resetVariables();
    }
  }

  if (step == 3) {
    ocx = camera.xOrange();
    gcx = camera.enemyGoal();
    oled.print(95, 0, "S2", 1); oled.print(105, 54, i, 1);
    oled.print(0, 0, "deg: ", 1); oled.print(30, 0, angle, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, ocx, 1);
    oled.print(0, 40, "gcx: ", 1); oled.print(30, 40, gcx, 1);
    oled.show();

    switch (utils.checkPreciseDirection(ocx, 13)) {
      case 0:
      case 1:
      case 2:
      case 3:
        motor.TurnLeft(lowestSpeed);
        break;
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        if (utils.checkDirection(gcx) == 1) {
          oled.print(0, 0, "TRYING TO SCORE", 3); oled.show();
          motor.moveToAngle(0, maxSpeed, 1000);
          resetVariables();
        } else {
          motor.moveToAngle(300, angleSpeed);
        }
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        motor.TurnRight(lowestSpeed);
        break;
      case -1:
        motor.North(maxSpeed, 500);
        motor.South(maxSpeed, 500);
        motor.hardReset();
        resetVariables();
    }
  }

  if (step == 4) { // Positioning
    ocx = camera.xOrange();
    oled.print(95, 0, "S4", 1);
    oled.print(105, 54, i, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, ocx, 1);
    oled.show();

    switch (utils.checkPreciseDirection(ocx, 13)) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
        motor.TurnLeft(lowestSpeed);
        break;
      case 5:
      case 6:
      case 7:
        motor.hardReset();
        angle = compass.checkAngle();
        step = 5;
        break;
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
        motor.TurnRight(lowestSpeed);
        break;
      case -1:
        motor.hardReset();
        resetVariables();
        break;
    }
  }

  if (step == 5) {
    ocx = camera.xOrange();

    oled.print(95, 0, "S5", 1);
    oled.print(105, 54, i, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, ocx, 1);
    oled.print(0, 40, "deg: ", 1); oled.print(30, 40, angle, 1);
    oled.show();

    motor.moveToAngle(angle, maxSpeed);
    if (ocx < 0) {
      motor.moveToAngle(angle, maxSpeed, 500);

      int _angle = 0;
      if (angle > 0) _angle = angle - 180;
      else _angle = angle + 180; 

      motor.moveToAngle(_angle, maxSpeed, 500);
      resetVariables();
    }
  }

  i++;
}

void resetVariables() {
  j = 0;
  ocx = -1;
  gcx = -1;
  step = -1;
  angle = -1;
  whileLoop = true;
}
