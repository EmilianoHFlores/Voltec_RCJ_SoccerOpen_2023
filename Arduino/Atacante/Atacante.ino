#include <Wire.h>
#include "Motor.h"
#include "Utils.h"
#include "Camera.h"
#include "Compass.h"
#include "Ultrasonic.h"
#include "Oled.h"
#include "Qrd.h"

#define TEAM "blue"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4

#define maxSpeed 255
#define aproachSpeed 200
#define angleSpeed 150
#define rotationSpeed 100
#define positionSpeed 75
#define lowestSpeed 50
#define COMPASS_DEVIATION 20

#define Button 7

Motor motor;
Utils utils;
Camera camera;
Compass compass;
Ultrasonic ultrasonic;
Oled oled;
Qrd qrd;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  pinMode(Button, INPUT);
  compass.begin();

  oled.begin();
  oled.print(10, 10, "Starting...");
  oled.show();

  motor.begin(4, 34, 35, 3, 32, 33, 2, 31, 30, 5, 37, 36);
  motor.attachCompass();
  motor.attachUtils(53, 320, 240);

  camera.begin(115200, 1, TEAM);
  utils.begin(53, 320, 240);
  //        N1.  N2. N3.   N4.  S1.  S2.  S3.  S4.  E1.  E2.  E3.  E4.  W1.  W2.  W3.  W4.
  qrd.begin(A9,  A8, A10,  A11, A3,  A0,  A2,  A1,  A7,  A4,  A6,  A5,  A15, A12, A14, A13);
  qrd.attachCompass();
  qrd.attachUtils(53, 320, 240);
  qrd.setThresholds();
  delay(2500);
  oled.print(0, 0, "Setup finished, waiting for button", 1); 
  oled.print(0, 32, compass.checkAngle(), 2);
  oled.show();
  while(true) {
    if (digitalRead(Button) == 1) break;
  }
};

int j = 0;
int i = 0;
int ocx = -1;
int ocy = -1;
int gcx = -1;
int gcy = -1;
int step = -1;
float angle = -1;
float ball_angle = -1;
bool whileLoop = true;
/**
 * scenario 0 -> ball is between 0 and 45 degrees
 * scenario 1 -> ball is between 45 and 90 degrees
 * scenario 2 -> ball is between 90 and 135 degrees
 * scenario 3 -> ball is between 135 and 150 degrees
 * scenario 4 -> ball is between 150 and 180 degrees
*/

int n_min = 1023;
int qrd_step = 0;
void loop() {
  if (step == -1) { // This step is used as a checkpoint, this helps to debug but will have no effect on real matches
    oled.print(0, 0, "Starting loop again in step 0", 2); oled.show();
    motor.hardReset();
    step = 0;
  }

  if (step == 0) {  // Find ball
    ocx = camera.xOrange();
    int preciseLocation = utils.checkPreciseDirection(ocx, 7);
    angle = compass.checkAngle();
    oled.print(105, 0, "S0", 1);   oled.print(105, 54, i, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, ocx, 1);
    oled.print(0, 40, "deg: ", 1); oled.print(30, 40, angle, 1);
    oled.show();
    switch (preciseLocation) {
      case 0:
      case 1:
        motor.TurnLeft(lowestSpeed); // If ball is detected in the left of the robot, it will turn to the left
        break;
      case 2:
      case 3:
      case 4: // Once the ball is in the center, it will go to the next step
        step = 1;
        ball_angle = angle;
        motor.hardReset();
        break;
      case 5:
      case 6: // If the ball is detected at the right side of the robot, it will turn to the right
        motor.TurnRight(lowestSpeed);
        break;
      case -1: // When no ball is in vision camp, it will rotate until it can see it.
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

  if (step == 2) { // The ball is between 180 and 30 degrees
    ocx = camera.xOrange();
    angle = compass.checkAngle();
    oled.print(95, 0, "S2", 1); oled.print(105, 54, i, 1);
    oled.print(0, 0, "deg: ", 1); oled.print(30, 0, angle, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, utils.checkPreciseDirection(ocx, 3), 1);
    oled.print(0, 40, "gcx: ", 1); oled.print(30, 40, utils.checkDirection(gcx), 1);
    oled.show();

    switch (utils.checkPreciseDirection(ocx, 13)) {
      case 0: // When the ball is at the left side of the robot, it will rotate to the left
      case 1: // This may happen because
      case 2: // Something moved the ball
      case 3: // The robot passed the ball when trying to aproach it.
      // If when rotating, the angle is between 15 and -15, it will stop and move to step 6 where the robot will be placed in front of the ball.
        if (utils.inRangeCompass(compass.checkAngle(), 0, COMPASS_DEVIATION)) {
          step = 6;
          motor.hardReset();
        } else {
          motor.TurnLeft(rotationSpeed);
        }
        break;
      case 4: // This case will be triggered when the ball is in the center of the camera's vision camp
      case 5: // If the enemy team goal is behind the ball, it will go to step 5
      case 6: // If the enemy goal is not behind the ball, the robot will move 60 degrees on the ball to try to see it
      case 7: // If during the movement, the angle is between 15 and -15, it will move to step 6 and place the robot behind the ball
      case 8:
        if (utils.inRangeCompass(compass.checkAngle(), 0, COMPASS_DEVIATION)) {
          step = 6;
          motor.hardReset();
        } else{
          motor.moveToAngle(ball_angle, 60, aproachSpeed);
        }

        break;
      case 9: // When the ball is at the right side of the robot, it will rotate to the right
      case 10: // This may happen because
      case 11: // Something moved the ball
      case 12: // The robot passed the ball when trying to aproach it.
      // If when rotating, the angle is between 15 and -15, it will stop and move to step 6 where the robot will be placed in front of the ball.
        if (utils.inRangeCompass(compass.checkAngle(), 0, COMPASS_DEVIATION)) {
          step = 6;
          motor.hardReset();
        } else {
          motor.TurnRight(rotationSpeed);
        }
        break;
      case -1:
        motor.hardReset();
        resetVariables();
    }
  }

  if (step == 3) {
    ocx = camera.xOrange();
    angle = compass.checkAngle();
    oled.print(95, 0, "S3", 1); oled.print(105, 54, i, 1);
    oled.print(0, 0, "deg: ", 1); oled.print(30, 0, angle, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, ocx, 1);
    oled.print(0, 40, "gcx: ", 1); oled.print(30, 40, gcx, 1);
    oled.show();
    // Same logic as step 2 is aplied for all this switch cases
    switch (utils.checkPreciseDirection(ocx, 13)) {
      case 0:
      case 1:
      case 2:
      case 3:
        if (utils.inRangeCompass(compass.checkAngle(), 0, COMPASS_DEVIATION)) {
          step = 6;
          motor.hardReset();
        } else {
          motor.TurnLeft(rotationSpeed);
        }
        break;
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        if (utils.inRangeCompass(compass.checkAngle(), 0, COMPASS_DEVIATION)) {
          motor.hardReset();
          step = 6;
        } else {
          motor.moveToAngle(ball_angle, 300, aproachSpeed);
        }
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        if (utils.inRangeCompass(compass.checkAngle(), 0, COMPASS_DEVIATION)) {
          step = 6;
          motor.hardReset();
        } else {
          motor.TurnRight(rotationSpeed);
        }
        break;
      case -1:
        motor.hardReset();
        resetVariables();
    }
  }

  if (step == 4) { // put the robot behind the ball
    ocx = camera.xOrange();
    angle = compass.checkAngle();
    oled.print(95, 0, "S4", 1);
    oled.print(105, 54, i, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, ocx, 1);
    oled.show();

    if (angle > 30 && angle < -30) {
      resetVariables(); // Add a filter to avoid the robot trying to score when it is not looking between the angles 30 and -30 to avoid auto goals. This many happen because of the robot being hit and its trayectory being modified
      // ocx variable will be -1 so switch/case will not be executed and will go to step -1
    }

    switch (utils.checkPreciseDirection(ocx, 13)) { // Try to place the ball more in the center of the vision camp and also adjust if the ball is moved
      case 0:
      case 1:
      case 2:
      case 3:
        motor.TurnLeft(positionSpeed);
        break;
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        motor.hardReset();
        step = 6;
        angle = compass.checkAngle();
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        motor.TurnRight(positionSpeed);
        break;
      case -1:
        motor.hardReset();
        resetVariables();
        break;
    }
  }

  if (step == 5) { // Triggered when ball is between 30 and -30
    ocx = camera.xOrange();
    oled.print(95, 0, "S5", 1);
    oled.print(105, 54, i, 1);
    oled.print(0, 40, "ocx: ", 1); oled.print(30, 40, ocx, 1);
    oled.show();

    switch (utils.checkPreciseDirection(ocx, 7)) {
      case 0:
      case 1:
        motor.moveToAngle(0, 35, 200);
        break;
      case 2:
      case 3:
      case 4:
        motor.moveToAngle(0, 0, 255, 500);
        motor.moveToAngle(0, 180, 175, 500);
        break;
      case 5:
      case 6:
        motor.moveToAngle(0, 325, 200);
        break;
      case -1:
        motor.hardReset();
        resetVariables();
        break;
    }
    if (qrd.North()) {
      motor.South(maxSpeed, 200);
      motor.rotateToAngle(compass.checkAngle(), 0, 75, true);
      motor.hardReset();
      qrd.setThresholds();
      delay(50);
      resetVariables();
    };
    qrd.setThresholds();
  }

  if (step == 6) {
    ocx = camera.xOrange();
    angle = compass.checkAngle();
    oled.print(95, 0, "S6", 1);
    oled.print(105, 54, i, 1);
    oled.print(0, 20, "ocx: ", 1); oled.print(30, 20, utils.checkPreciseDirection(ocx, 11), 1);
    oled.print(0, 40, "deg: ", 1); oled.print(30, 40, angle, 1);
    oled.show();

    motor.rotateToAngle(angle, 0, rotationSpeed, false);

    switch (utils.checkPreciseDirection(ocx, 11)) {
      case 0:
      case 1:
        motor.moveToAngle(0, -90, 100);
        break;
      case 2:
      case 3:
        motor.moveToAngle(0, -90, 50);
        break;
      case 4:
      case 5:
      case 6:
        motor.hardReset();
        angle = compass.checkAngle();
        step = 5;
        break;
      case 7:
      case 8:
        motor.moveToAngle(0, 90, 50);
        break;
      case 9:
      case 10:
        motor.moveToAngle(0, 90, 100);
        break;
      case -1:
        resetVariables();
        break;
    }
  }
  i++;
}

void resetVariables() {
  j = 0;
  ocx = -1;
  ocy = -1;
  gcx = -1;
  step = -1;
  angle = -1;
  ball_angle = -1;
  whileLoop = true;
}
