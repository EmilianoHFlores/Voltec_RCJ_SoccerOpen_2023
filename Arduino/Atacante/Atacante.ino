#include "Camera.h"
#include "Compass.h"
#include "Motor.h"
#include "Oled.h"
#define TEAM "blue"
#define defaultSpeed 100
#define scoreSpeed 50
#define fastRotationSpeed 60
#define rotationSpeed 50
#define positionSpeed 35
#define horizontalSpeed 60
#define lowestSpeed 30
const int Button = 7;
int i = 0;
int seen = 0;
int ball_angle = -1;
Camera camera;
Oled oled;
Motor motor;
Compass compass;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  oled.begin();
  oled.print(0, 0, "Starting...", 2); oled.show();

  pinMode(Button, INPUT);

  camera.begin(TEAM);
  compass.begin("navx");
  delay(1500);
  motor.begin(2, 30, 31, 3, 32, 33, 5, 36, 37, 4, 35, 34);
  // Subclasses
  motor.attachCompass("navx");
  // motor.attachQrd(A9, A8, A10, A11,   A3, A0, A2, A1,    A7, A4, A6, A5,    A15, A12, A14, A13);
  unsigned long mill = millis();
  for (;;) {
    if (millis() - mill >= 5000) break;
    oled.print(0, 0, "Ready in:", 2);
    oled.print(0, 32, 5000 - (millis() - mill), 2);
    oled.print(50, 32, "ms", 2);
    oled.show();
  }

  oled.print(0, 0, "Ready", 3); oled.show();
  // while (!digitalRead(Button));
  oled.print(0, 0, "Started", 3); oled.show();
};

int step = -1;

void loop() {
  if (step == -1) {
    camera.add("ox");
    camera.call();
    oled_log();
    switch (camera.direction(camera.ox(), 7)) {
    case 0: motor.TurnLeft(fastRotationSpeed); seen = 0; break;
    case 1: motor.TurnLeft(positionSpeed); seen = 0; break;
    case 2: motor.TurnLeft(lowestSpeed); seen = 0; break;
    case 3: motor.hardReset(); step = 0; break;
    case 4: motor.TurnRight(lowestSpeed);  seen = 1; break;
    case 5: motor.TurnRight(positionSpeed); seen = 1; break;
    case 6: motor.TurnRight(fastRotationSpeed); seen = 1; break;
    default: seen ? motor.TurnRight(fastRotationSpeed) : motor.TurnLeft(fastRotationSpeed); break;
    }
  }
  if (step == 0) {
    ball_angle = compass.checkAngle();
    camera.add("yi");
    camera.call();
    oled_log();
    if (camera.yi()) step = 1;
    else step = 2;
  }

  if (step == 1) {
    camera.del();
    oled_log();
    follow_ball();
    step = -1;
  }

  if (step == 2) {
    camera.add("yi");
    camera.add("ox");
    camera.call();
    oled_log();
    if (camera.yi()) { step = 1; return; };
    if (compass.range(compass.checkAngle(), 0, 15)) { step = 3; return; };
    switch (camera.direction(camera.ox(), 9)) {
    case 0: motor.TurnLeft(rotationSpeed); break;
    case 1: motor.TurnLeft(positionSpeed); break;
    case 2: motor.TurnLeft(lowestSpeed); break;
    case 3: motor.moveToAngle(compass.checkAngle(), ball_angle > 0 ? 45 : -45, defaultSpeed); break;
    case 4: motor.moveToAngle(compass.checkAngle(), ball_angle > 0 ? 90 : -90, defaultSpeed); break;
    case 5: motor.moveToAngle(compass.checkAngle(), ball_angle > 0 ? 45 : -45, defaultSpeed); break;
    case 6: motor.TurnRight(lowestSpeed); break;
    case 7: motor.TurnRight(positionSpeed); break;
    case 8: motor.TurnRight(rotationSpeed); break;
    default: step = -1;
    }
  }

  if (step == 3) {
    camera.add("ox");
    camera.add("yi");
    camera.call();
    oled_log();
    if (camera.yi()) { step = 1; return; };
    switch (camera.direction(camera.ox(), 13)) {
    case 0:
    case 1:
      motor.moveToAngle(0, -90, defaultSpeed);
      break;
    case 2:
    case 3:
      motor.moveToAngle(0, -90, horizontalSpeed);
      break;
    case 4:
    case 5:
      motor.moveToAngle(0, -90, positionSpeed);
      break;
    case 6:
      motor.hardReset(); step = 4; break;
    case 7:
    case 8:
      motor.moveToAngle(0, 90, positionSpeed);
      break;
    case 9:
    case 10:
      motor.moveToAngle(0, 90, horizontalSpeed);
      break;
    case 11:
    case 12:
      motor.moveToAngle(0, 90, defaultSpeed);
      break;
    default: 
      motor.hardReset();
      step = -1;
      break;
    }
  }

  if (step == 4) {
    camera.add("yx");
    camera.add("yi");
    camera.add("ox");
    camera.call();
    if (camera.ox() == -1) { step = -1; return; }
    if (camera.yi()) { step = -1; return; }
    if (camera.direction(camera.yx(), 3) == 0) motor.moveToAngle(0, 90, positionSpeed);
    else if (camera.direction(camera.yx(), 3) == 2) motor.moveToAngle(0, -90, positionSpeed);
    else step = 1; // If goal is not visible, then throw the ball outside of the field (this may happen because a robot is blocking the goal)
  }

  i++;
}

void reset() {
  ball_angle = -1;
  step = -1;
}

void oled_log() {
  oled.print(0, 0, "step", 1); oled.print(40, 0, step, 1); oled.print(90, 0, i, 1);
  oled.print(0, 15, "OCX: ", 1); oled.print(60, 15, camera.ox(), 1);
  oled.print(0, 30, "DEG: ", 1); oled.print(60, 30, compass.checkAngle(), 1);
  oled.print(0, 45, "GXI: ", 1); oled.print(60, 45, camera.yi(), 1);
  oled.show();
}

void follow_ball() {
  camera.del();
  camera.add("yi");
  camera.add("ox");
  camera.call(true);
  oled.print(0, 0, "ox: ", 3); oled.print(64, 0, camera.ox(), 3);
  if (!camera.yi()) return;
  switch (camera.direction(camera.ox(), 9)) {
    case 0: case 1: motor.TurnLeft(positionSpeed); seen = 0; break;
    case 2: case 3: motor.TurnLeft(lowestSpeed); seen = 0; break;
    case 4: motor.North(scoreSpeed); break;
    case 5: case 6: motor.TurnRight(lowestSpeed);  seen = 1; break;
    case 7: case 8: motor.TurnRight(positionSpeed); seen = 1; break;
    default: return;
  }
  follow_ball();
}



/* QRD SOUTH VALUES

  Serial.print("South:");
  Serial.println();

  Serial.println();
  Serial.println();
  Serial.println();

  Serial.print("inS1: ");
  Serial.print(qrd.inS1());
  Serial.print(" - S1: ");
  Serial.println(qrd._S1());

  Serial.print("inS2: ");
  Serial.print(qrd.inS2());
  Serial.print(" - S2: ");
  Serial.println(qrd._S2());

  Serial.print("inS3: ");
  Serial.print(qrd.inS3());
  Serial.print(" - S3: ");
  Serial.println(qrd._S3());

  Serial.print("inS4: ");
  Serial.print(qrd.inS4());
  Serial.print(" - S4: ");
  Serial.println(qrd._S4());

  delay(50);

*/