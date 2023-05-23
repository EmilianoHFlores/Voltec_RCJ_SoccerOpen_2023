#include "Camera.h"
#include "Compass.h"
#include "Motor.h"
#include "Oled.h"
#include "Ultrasonic.h"
#define TEAM "blue"
#define defaultSpeed 125
#define fastRotationSpeed 75
#define rotationSpeed 60
#define positionSpeed 30
const int Button = 7;
int i = 0;
int ocx = 0;
int ocy = 0;
int gxi = 0;
int gcy = 0;
int seen = 0;
int ball_angle = -1;
Camera camera;
Oled oled;
Motor motor;
Compass compass;
Ultrasonic ultrasonic;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  oled.begin();
  oled.print(0, 0, "Starting...", 2); oled.show();

  pinMode(Button, INPUT);

  camera.begin(TEAM);
  compass.begin("navx");
  motor.begin(2, 30, 31, 3, 32, 33, 5, 36, 37, 4, 35, 34);
  ultrasonic.begin(53, 53, 53, 53, 53, 53);

  // Subclasses
  motor.attachCompass("navx");

  unsigned long mill = millis();
  for (;;) {
    if (millis() - mill >= 5000) break;
    oled.print(0, 0, "Ready in:", 2);
    oled.print(0, 32, 5000 - (millis() - mill), 2);
    oled.print(50, 32, "ms", 2);
    oled.show();
  }

  oled.print(0, 0, "Ready", 3); oled.show();
  while (!digitalRead(Button));
  oled.print(0, 0, "Started", 3); oled.show();
};

int step = -1;

void loop() {
  if (step == -1) {
    ocx = camera.xOrange();
    oled_log("centering ball");
    switch (camera.direction(ocx, 7)) {
    case 0: motor.TurnLeft(fastRotationSpeed); seen = 0; break;
    case 1: motor.TurnLeft(rotationSpeed); seen = 0; break;
    case 2: motor.TurnLeft(positionSpeed); seen = 0; break;
    case 3: motor.hardReset(); step = 0; ball_angle = compass.checkAngle(); break;
    case 4: motor.TurnRight(positionSpeed);  seen = 1; break;
    case 5: motor.TurnRight(rotationSpeed); seen = 1; break;
    case 6: motor.TurnRight(fastRotationSpeed); seen = 1; break;
    default: seen ? motor.TurnRight(fastRotationSpeed) : motor.TurnLeft(fastRotationSpeed); break;
    }
  }

  if (step == 0) {
    gxi = camera.intercept();
    oled_log("Checking intercept");
    if (gxi) step = 1;
    else step = -1;
  }

  if (step == 1) {
    oled.print(0, 0, "Follow ball"); oled.show();
    follow_ball();
    step = -1;
  }

  i++;
}

void oled_log(String name) {
  oled.print(0, 0, name, 1); oled.print(90, 0, i, 1);
  oled.print(0, 15, "OCX: ", 1); oled.print(60, 15, ocx, 1);
  oled.print(0, 30, "DEG: ", 1); oled.print(60, 30, compass.checkAngle(), 1);
  oled.print(0, 45, "GXI: ", 1); oled.print(60, 45, gxi, 1);
  oled.show();
}

void follow_ball() {
  ocx = camera.xOrange();
  int to_move_angle = -1;
  switch (camera.direction(ocx, 5)) {
  case 0: to_move_angle = 45; break;
  case 1: to_move_angle = 15; break;
  case 2: to_move_angle = 0; break;
  case 3: to_move_angle = -15; break;
  case 4: to_move_angle = -45; break;
  case -1: break;
  }
  oled.print(0, 0, "to: ", 3); oled.print(0, 32, to_move_angle, 3);
  oled.show();
  if (to_move_angle == -1) {
    motor.hardReset();
    return;
  }
  motor.moveToAngle(0, to_move_angle, defaultSpeed);
  ocy = camera.yOrange();
  if (ocy == 0) {
    motor.moveToAngle(0, 0, defaultSpeed, 500);
    motor.moveToAngle(0, 180, defaultSpeed, 500);
    return;
  }
  follow_ball();
}