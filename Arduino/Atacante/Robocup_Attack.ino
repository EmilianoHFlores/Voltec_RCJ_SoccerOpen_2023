#include "Camera.h"
#include "Compass.h"
#include "Motor.h"
#include "Oled.h"
#include "Xbee.h"

#define COMPASS_TYPE "navx"

#define defaultSpeed 200
#define scoreSpeed 125
#define fastRotationSpeed 70
#define rotationSpeed 60
#define positionSpeed 50
#define horizontalSpeed 90
#define lowestSpeed 40

#define horizontal_lowest 30
#define horizontal_1 35
#define horizontal_2 40
#define horizontal_3 50
#define horizontal_4 65
#define horizontal_5 80
#define horizontal_6 100
#define horizontal_7 120
#define horizontal_highest 160
#define horizontal_aproach 80

#define southThreshold 40

long loopIndex = 0;
int ballAngle = 0;
int step = -1;
int lastTimeSeen = 0;

Camera camera;
Oled oled;
Motor motor;
Compass compass;
Xbee xbee;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  oled.begin();
  oled.print<String>(0, 0, "Starting...", 3); oled.show();

  camera.begin();
  compass.begin(COMPASS_TYPE);

  xbee.begin();

  motor.begin(2, 30, 31, 3, 32, 33, 5, 36, 37, 4, 35, 34);
  motor.attachCompass(COMPASS_TYPE);

  unsigned long mill = millis();
  for (;;) {
    if (millis() - mill >= 5000) break;
    oled.print<String>(0, 0, "Ready in:", 2);
    oled.print<long>(0, 32, 5000 - (millis() - mill), 2);
    oled.print<String>(50, 32, "ms", 2);
    oled.show();
  }
  oled.print<String>(0, 0, "Started", 3); oled.show();
};

void loop() {
  xbee.Receive();
  if (step == -1) {
    camera.add("ox");
    camera.call();
    oled_log();
    switch (camera.direction(camera.ox(), 7)) {
    case 0: motor.TurnLeft(fastRotationSpeed); lastTimeSeen = 0; break;
    case 1: motor.TurnLeft(positionSpeed); lastTimeSeen = 0; break;
    case 2: motor.TurnLeft(lowestSpeed); lastTimeSeen = 0; break;
    case 3:
      motor.Stop();
      step = 0; 
      xbee.Send(1);
      if (xbee.selfAction() != 1) { xbee.Send(2); };
      break;
    case 4: motor.TurnRight(lowestSpeed);  lastTimeSeen = 1; break;
    case 5: motor.TurnRight(positionSpeed); lastTimeSeen = 1; break;
    case 6: motor.TurnRight(fastRotationSpeed); lastTimeSeen = 1; break;
    default: lastTimeSeen ? motor.TurnRight(fastRotationSpeed) : motor.TurnLeft(fastRotationSpeed); break;
    }
  }

  if (xbee.selfAction() == 1) {
    if (step == 0) {
      ballAngle = compass.checkAngle();
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
      reset();
    }

    if (step == 2) {
      camera.add("yi");
      camera.add("ox");
      camera.call();
      oled_log();
      if (camera.yi()) { step = 3; return; };
      switch (camera.direction(camera.ox(), 11)) {
        case 0: motor.TurnLeft(rotationSpeed); break;
        case 1: case 2: motor.TurnLeft(positionSpeed); break;
        case 3: motor.TurnLeft(lowestSpeed); break;
        case 4: motor.moveToAngle(compass.checkAngle(), ballAngle > 0 ? 45 : -45, defaultSpeed); break;
        case 5: motor.moveToAngle(compass.checkAngle(), ballAngle > 0 ? 90 : -90, defaultSpeed); break;
        case 6: motor.moveToAngle(compass.checkAngle(), ballAngle > 0 ? 45 : -45, defaultSpeed); break;
        case 7: motor.TurnRight(lowestSpeed); break;
        case 8: case 9: motor.TurnRight(positionSpeed); break;
        case 10: motor.TurnRight(rotationSpeed); break;
        default: reset(); break;
      }
    }

    if (step == 3) {
      camera.add("ox");
      camera.call();
      oled_log();
      switch (camera.direction(camera.ox(), 7)) {
      case 0: motor.TurnLeft(fastRotationSpeed); lastTimeSeen = 0; break;
      case 1: motor.TurnLeft(positionSpeed); lastTimeSeen = 0; break;
      case 2: motor.TurnLeft(lowestSpeed); lastTimeSeen = 0; break;
      case 3: motor.Stop(); step = 0; break;
      case 4: motor.TurnRight(lowestSpeed);  lastTimeSeen = 1; break;
      case 5: motor.TurnRight(positionSpeed); lastTimeSeen = 1; break;
      case 6: motor.TurnRight(fastRotationSpeed); lastTimeSeen = 1; break;
      default: reset(); break;
      }
    }
  }

  if (xbee.selfAction() == 2) {
    if (step == 0) {
      camera.add("ox");
      camera.call();
      oled_log();
      if (camera.ox() == -1) return reset();
      if (!compass.range(compass.checkAngle(), 0, 15)) step = 1;
      step = 2;
      // next
    }
    if (step == 1) {
      camera.add("ox");
      camera.call();
      oled_log();
      switch (camera.direction(camera.ox(), 9)) {
        case 0: motor.TurnLeft(rotationSpeed); break;
        case 1: motor.TurnLeft(positionSpeed); break;
        case 2: motor.TurnLeft(lowestSpeed); break;
        case 3: motor.moveToAngle(compass.checkAngle(), compass.checkAngle() > 0 ? 45 : -45, defaultSpeed); break;
        case 4: motor.moveToAngle(compass.checkAngle(), compass.checkAngle() > 0 ? 90 : -90, defaultSpeed); break;
        case 5: motor.moveToAngle(compass.checkAngle(), compass.checkAngle() > 0 ? 45 : -45, defaultSpeed); break;
        case 6: motor.TurnRight(lowestSpeed); break;
        case 7: motor.TurnRight(positionSpeed); break;
        case 8: motor.TurnRight(rotationSpeed); break;
      }
      if (compass.range(compass.checkAngle(), 0, 15)) step = 2;
    }
    if (step == 2) {
      camera.add("ox");
      camera.add("oy");
      camera.call();
      if (camera.ox() == -1) return reset();
      if (camera.oy() <= 0) step = 3;
      if (camera.oy() <= 5) {
        switch (camera.direction(camera.ox(), 7)) {
          case 0: motor.moveToAngle(0, -90, horizontal_2); break;
          case 1: motor.moveToAngle(0, -90, horizontal_1); break;
          case 2: motor.moveToAngle(0, -90, horizontal_lowest); break;
          case 3: motor.moveToAngle(0, 0, defaultSpeed); break;
          case 4: motor.moveToAngle(0, 90, horizontal_lowest); break;
          case 5: motor.moveToAngle(0, 90, horizontal_1); break;
          case 6: motor.moveToAngle(0, 90, horizontal_2); break;
        }
      } else if (camera.oy() <= 10) {
        switch (camera.direction(camera.ox(), 9)) {
          case 0: motor.moveToAngle(0, -90, horizontal_4); break;
          case 1: motor.moveToAngle(0, -90, horizontal_3); break;
          case 2: motor.moveToAngle(0, -90, horizontal_2); break;
          case 3: motor.moveToAngle(0, -90, horizontal_1); break;
          case 4: motor.moveToAngle(0, 0, horizontal_aproach); break;
          case 5: motor.moveToAngle(0, 90, horizontal_1); break;
          case 6: motor.moveToAngle(0, 90, horizontal_2); break;
          case 7: motor.moveToAngle(0, 90, horizontal_3); break;
          case 8:  motor.moveToAngle(0, 90, horizontal_4); break;
        }
      } else if (camera.oy() <= 20) {
        switch (camera.direction(camera.ox(), 9)) {
          case 0: motor.moveToAngle(0, -90, horizontal_6); break;
          case 1: motor.moveToAngle(0 ,-90, horizontal_5); break;
          case 2: motor.moveToAngle(0, -90, horizontal_4); break;
          case 3: motor.moveToAngle(0, -90, horizontal_3); break;
          case 4: motor.moveToAngle(0, 0, horizontal_aproach); break;
          case 5: motor.moveToAngle(0, 90, horizontal_3); break;
          case 6: motor.moveToAngle(0, 90, horizontal_4); break;
          case 7: motor.moveToAngle(0, 90, horizontal_5); break;
          case 8: motor.moveToAngle(0, 90, horizontal_6); break;
        }
      } else {
        switch (camera.direction(camera.ox(), 9)) {
          case 0: motor.moveToAngle(0, -90, horizontal_highest); break;
          case 1: motor.moveToAngle(0, -90, horizontal_7); break;
          case 2: motor.moveToAngle(0, -90, horizontal_5); break;
          case 3: motor.moveToAngle(0, -45, horizontal_4); break;
          case 4: motor.moveToAngle(0, 0, horizontal_aproach); break;
          case 5: motor.moveToAngle(0, 45, horizontal_4); break;
          case 6: motor.moveToAngle(0, 90, horizontal_5); break;
          case 7: motor.moveToAngle(0, 90, horizontal_7); break;
          case 8: motor.moveToAngle(0, 90, horizontal_highest); break;
        }
      }
    }
    if (step == 3) {
      camera.del();
      oled_log();
      follow_ball();
      reset();
    }
  }
  loopIndex++;
}

void reset() {
  ballAngle = -1;
  step = -1;
  xbee.Send(0);
}

void oled_log() {
  oled.print<String>(0, 0, "step:", 1); oled.print<int>(30, 0, step, 1); oled.print<long>(90, 0, loopIndex, 1);
  oled.print<String>(45, 0, "self", 1); oled.print<int>(75, 0, xbee.selfAction(), 1);
  oled.print<String>(0, 15, "OCX: ", 1); oled.print<int>(60, 15, camera.ox(), 1);
  oled.print<String>(0, 30, "DEG: ", 1); oled.print<int>(60, 30, compass.checkAngle(), 1);
  oled.print<String>(0, 45, "GXI: ", 1); oled.print<int>(60, 45, camera.yi(), 1);
  oled.show();
}

void follow_ball() {
  camera.del();
  camera.add("yi");
  camera.add("ox");
  camera.add("oy");
  camera.call();
  oled.print<String>(0, 0, "ox: ", 3); oled.print<int>(64, 0, camera.ox(), 3); oled.show();
  if (!camera.yi()) return;
  if (camera.ox() == -1) return;
  if (camera.oy() <= 0) {
    switch (camera.direction(camera.ox(), 5)) {
      case 0: motor.TurnLeft(lowestSpeed); break;
      case 1: motor.North(scoreSpeed); break;
      case 2: motor.North(scoreSpeed); break;
      case 3: motor.North(scoreSpeed); break;
      case 4: motor.TurnRight(lowestSpeed); break;
    }
  } else if (camera.oy() <= 5) {
    switch (camera.direction(camera.ox(), 5)) {
      case 0: motor.TurnLeft(lowestSpeed); break;
      case 1: motor.TurnLeft(lowestSpeed); break;
      case 2: motor.North(scoreSpeed); break;
      case 3: motor.TurnLeft(lowestSpeed); break;
      case 4: motor.TurnLeft(lowestSpeed); break;
    }
  } else if (camera.oy() <= 15) {
    switch (camera.direction(camera.ox(), 11)) {
      case 0: motor.TurnLeft(rotationSpeed); break;
      case 1: motor.TurnLeft(positionSpeed); break;
      case 2: motor.TurnLeft(lowestSpeed); break;
      case 3: motor.TurnLeft(lowestSpeed); break;
      case 4: motor.North(scoreSpeed); break;
      case 5: motor.North(scoreSpeed); break;
      case 6: motor.North(scoreSpeed); break;
      case 7: motor.TurnRight(lowestSpeed); break;
      case 8: motor.TurnRight(lowestSpeed); break;
      case 9: motor.TurnRight(positionSpeed); break;
      case 10: motor.TurnRight(rotationSpeed); break;
    }
  } else {
    switch (camera.direction(camera.ox(), 5)) {
      case 0: motor.TurnLeft(positionSpeed);  break;
      case 1: motor.TurnLeft(lowestSpeed); break;
      case 2: motor.North(scoreSpeed); break;
      case 3: motor.TurnRight(lowestSpeed); break;
      case 4: motor.TurnLeft(positionSpeed); break;
    }
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
