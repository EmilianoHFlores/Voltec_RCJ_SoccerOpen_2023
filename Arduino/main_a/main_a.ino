#include "Strategy.h"
#include "Qrd.h"

#define VOLUME 10
#define TOGGLE_SWITCH 40
#define START_SWITCH 41
#define COMPASS_TYPE "navx"
#define HELPER_COMPASS "adafruit"
#define START_TIME 5000
#define PREFERRED_ACTION 2
#define TEAM "yellow"
#define REQUIRE_BUTTON true

unsigned long previousMillis = millis();
unsigned long currentMillis = millis();
unsigned long qrdMillis = millis();
unsigned long loopId;

Strategy strategy;
Qrd qrd;

void setup () {
  Serial.begin(115200);
  Serial.setTimeout(50);
  pinMode(START_SWITCH, INPUT);
  pinMode(TOGGLE_SWITCH, INPUT);

  strategy.begin(TEAM);
  strategy.dribbler.begin(6, 7);
  strategy.buzzer.begin(10);
  
  strategy.buzzer.beep(VOLUME);
  delay(250);
  strategy.buzzer.stop();

  strategy.camera.begin();
  strategy.pixy.begin();

  strategy.xbee.begin();
  strategy.motor.begin(2, 33, 32, 5, 38, 39, 3, 34, 35, 4, 36, 37);
  strategy.compass.begin(COMPASS_TYPE);
  strategy.motor.attachCompass(&strategy.compass);
  strategy.motor.helper.begin(HELPER_COMPASS);
  strategy.ultrasonic.begin(25, 24, 23, 22, 27, 26);
  qrd.begin(99, A12, A14, A13,  99, 99, 99, 99,  A7, A4, A6, A5,  A3, A0, A2, A1);

  unsigned long start_millis = millis();
  strategy.buzzer.startTimer();
  for (;;) {
    if (millis() - start_millis >= START_TIME) break;
    strategy.buzzer.beeps(2, VOLUME);
  }
  strategy.buzzer.stop();
  strategy.buzzer.startTimer();
  for (;;) {
    if (digitalRead(START_SWITCH) || !REQUIRE_BUTTON) break;
    strategy.buzzer.beeps(3, VOLUME);
    Serial.println("Waiting for human...");
  }
  strategy.buzzer.stop();
}

bool dribbling = false;
bool firstDribbling = true;
unsigned long start;

unsigned long long previousQrd = 0;
void loopa () {
  strategy.motor.North(200);
  if (millis() - previousQrd >= 25) {
    Serial.println();
    bool north = qrd.North(true);
    if (north) {
      strategy.motor.Stop();
      strategy.motor.South(200);
      delay(400);
      strategy.motor.Stop();
      for (int i = 0; i < 25; i++) qrd.North(false);
    }
    previousQrd = millis();
  }
}

// void loop () {
//   Serial.print("  Navx: "); Serial.print(strategy.compass.checkAngle());
//   Serial.print("  Helper: "); Serial.print(strategy.motor.helper.checkAngle());
//   Serial.print("  Navx initial: "); Serial.print(strategy.compass.initialReading);
//   Serial.print("  Helper initial: "); Serial.println(strategy.motor.helper.initialReading);
//   strategy.motor.rotateToAngle(strategy.compass.checkAngle(), 0, true);
// }

void loop () {
  if (!digitalRead(TOGGLE_SWITCH)) strategy.motor.active(false);
  else strategy.motor.active(true);

  strategy.camera.add("ox");
  strategy.camera.add("oy");
  strategy.camera.add("yi");
  strategy.camera.add("yx");
  strategy.camera.add("yw");
  strategy.camera.call();
  strategy.pixy.getBlocks();

  if (strategy.camera.ox() == -1 && !dribbling) {
    strategy.dribbler.stop();
    if (strategy.pixy.ox() != -1) {
      float angle = strategy.compass.checkAngle();

      strategy.motor.rotateToAngle(
        angle,
        180,
        true
      );
      dribbling = true;
      start = millis();
      return;
    }

    strategy.endAction();
    strategy.passiveDeffense();
    return;
  }

  if (dribbling) {
    if (strategy.camera.ox() == -1) {
      if (millis() - start <= 100) return;
      strategy.buzzer.beep(255);
      delay(500);
      strategy.buzzer.stop();
      strategy.motor.rotateToAngle(strategy.compass.checkAngle(), 0, true);
      dribbling = false;
      return;
    }
    if (strategy.camera.oy() > 10) {
      strategy.goto_ball(100);
    } else if (strategy.camera.oy() >= 3) {
      strategy.goto_ball(60);
      strategy.dribbler.run(50);
    } else if (strategy.camera.oy() == 0) {
      if (firstDribbling) {
        start = millis();
        firstDribbling = false;
      }
      if (millis() - start <= 1000) {
        strategy.goto_ball(50);
        return;
      }
      strategy.dribbler.run(100);
      if (strategy.compass.checkAngle() < 0) strategy.motor.TurnLeft(90);
      else strategy.motor.TurnRight(90);
    }

    if (strategy.compass.range(strategy.compass.checkAngle(), 0, 10)) {
      strategy.dribbler.stop();
      strategy.motor.Stop();
      dribbling = false;
      return;
    }
  } else {
    strategy.dribbler.stop();
    strategy.deffend();
  }
}