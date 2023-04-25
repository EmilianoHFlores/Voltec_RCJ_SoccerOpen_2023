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
#define COMPASS_DEVIATION 15

Motor motor;
Utils utils;
Camera camera;
Compass compass;
Ultrasonic ultrasonic;
Oled oled;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  oled.begin();
  oled.print(10, 10, "Starting...");
  oled.show();

  motor.begin(4, 34, 35, 3, 32, 33, 2, 30, 31, 5, 37, 36);
  motor.attachCompass();
  motor.attachUtils(53, 320, 240);

  camera.begin(115200, 1, TEAM);
  utils.begin(53, 320, 240);
  compass.begin();

  motor.reset(5000);  // Wait for NAVX to turn on TODO: Rewrite this function
  oled.print(0, 0, "Setup finished", 1); oled.show();
};

void loop() {}
