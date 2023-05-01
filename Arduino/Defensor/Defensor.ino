#include <Wire.h>
#include "Motor.h"
#include "Utils.h"
#include "Camera.h"
#include "Compass.h"
#include "Ultrasonic.h"
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
#define COMPASS_DEVIATION 30

#define distanceThresholdMin 40
#define limitThresholdMax 200
#define limitThresholdMin 3

#define fieldWidth 106
#define fieldThreshold 10

#define Button 7

Motor motor;
Utils utils;
Camera camera;
Compass compass;
Ultrasonic ultrasonic;
Oled oled;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  // oled.begin();
  // oled.print(10, 10, "Starting...");
  // oled.show();

  motor.begin(2, 30, 31, 3, 32, 33, 5, 36, 37, 4, 34, 35);
  motor.attachCompass("adafruit");
  motor.attachUtils(53, 320, 240);

  camera.begin(115200, 1, TEAM);
  utils.begin(53, 320, 240);
  compass.begin("adafruit");

  ultrasonic.begin(29, 28, 25, 24, 27, 26);

  // oled.print(0, 0, "Setup finished, waiting for the button to be pushed", 1); oled.show();
  while(true) {
    if (digitalRead(Button) == 1) break;
  }
  // oled.print(0, 0, "Setup finished", 1);
  // oled.show();
};

int ocx = -1;  // X central orange
int angle = 0;
int uls = -1;  // Ultrasónico Sur
int step = 0;
int i = 0;

void loopa () {
  motor.rotateToAngle(compass.checkAngle(), 0, 75, true);
  Serial.print("Compass: "); Serial.print(compass.checkAngle());
  Serial.print(" - Ultrasonic: "); Serial.print(ultrasonic.South());
  Serial.print(" - Camera: "); Serial.println(camera.xOrange());
}

void loop () {
  Serial.print("reading... camera");
  ocx = camera.xOrange();
  Serial.print("  reading... ultrasonic");
  uls = ultrasonic.South();
  Serial.print("  reading... compass");
  angle = compass.checkAngle();
  Serial.print("  Angle: ");
  Serial.print(angle);
  Serial.print("  i: ");
  Serial.println(i);
  if (step == 0) {
    Serial.println("Entered step 0");
    // printToOled(i, step, ocx, uls, angle);
    if (limitThresholdMin > uls || limitThresholdMax < uls) step = 1;
    else if (distanceThresholdMin > uls) step = 2; // Robot is down the threshold 
    else if (ocx >= 0) step = 3; // Robot is between thresholds and ball is visible 
    else if (ocx == -1) step = 4; // Robot is between thresholds but ball is not visible
    else step = 0;
    Serial.println("Finished step 0");
  }

  if (step == 1) {
    // printToOled(i, step, ocx, uls, angle);
    Serial.println("Entered step 1");
    motor.North(lowestSpeed);
    Serial.println("Finished step 1");
  }

  if (step == 2) {
    // printToOled(i, step, ocx, uls, angle);
    Serial.println("Entered step 2");
    motor.rotateToAngle(compass.checkAngle(), 0, 100, true);
    Serial.println("Rotate to angle");
    motor.North(rotationSpeed);
    Serial.println("Finished step 2");
  }
  if (step == 3) {
    // printToOled(i, step, ocx, uls, angle);
    Serial.println("Entered step 3");
    switch (utils.checkPreciseDirection(ocx, 12)) {
      case 0:
        motor.moveToAngle(0, 270, 100);
        break;
      case 1:
        motor.moveToAngle(0, 270, 75);
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        motor.hardReset();
        motor.rotateToAngle(compass.checkAngle(), 0, 100, false);
        motor.moveToAngle(0, 0, 100);
        break;
      case 10:
        motor.moveToAngle(0, 90, 75);        
        break;
      case 11:
        motor.moveToAngle(0, 90, 100);
        break;
      case -1:
        step = 4;
    }
    Serial.println("Finished step 3");
  }
  if (step == 4) {
    // printToOled(i, step, ocx, uls, angle);
    Serial.println("Entered step 4");
    motor.rotateToAngle(compass.checkAngle(), 0, 100, true);
    Serial.println("Rotate to angle");
    motor.South(150);
    Serial.println("Finished step 4");
  }

  resetVariables();
  i++;
}

void resetVariables() {
  ocx = -1;  // X central orange
  angle = 0;
  uls = -1; // Posición en eje X del robot
  step = 0;
}

// void printToOled(int _print_i, int _print_step, int _print_ocx, int _print_uls, int _print_ulx) {
//   oled.print(100, 50, _print_i, 1); 
//   oled.print(0, 0, "S", 1); oled.print(10, 0, _print_step, 1);
//   oled.print(0, 16, "ocx:", 1); oled.print(30, 16, utils.checkPreciseDirection(_print_ocx, 11), 1);
//   oled.print(0, 32, "uls:", 1); oled.print(30, 32, _print_uls, 1);
//   oled.print(0, 48, "ang:", 1); oled.print(30, 48, _print_ulx, 1);
//    oled.show();
// }