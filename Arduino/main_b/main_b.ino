#include "Strategy.h"
#include "Qrd.h"

#define VOLUME 255
#define TOGGLE_SWITCH 40
#define START_SWITCH 41
#define COMPASS_TYPE "navx"
#define START_TIME 5000
#define PREFERRED_ACTION 2
#define TEAM "blue"
#define REQUIRE_BUTTON true

unsigned long previousMillis = millis();
unsigned long currentMillis = millis();
unsigned long qrdMillis = millis();
unsigned long loopId;

Strategy strategy;
Qrd qrd;

void setup () {
  Serial.begin(115200);
  pinMode(START_SWITCH, INPUT);
  pinMode(TOGGLE_SWITCH, INPUT);

  strategy.begin(TEAM);
  strategy.buzzer.begin(10);
  
  strategy.buzzer.beep(VOLUME);
  delay(250);
  strategy.buzzer.stop();

  strategy.camera.begin();
  Serial.println("camera");
  strategy.xbee.begin();
  Serial.println("xbee");
  strategy.motor.begin(2, 33, 32, 5, 38, 39, 3, 34, 35, 4, 36, 37);
  Serial.println("motor");
  strategy.compass.begin(COMPASS_TYPE);
  Serial.println("compass");
  strategy.motor.attachCompass(&strategy.compass);
  Serial.println("attachCompass");
  strategy.ultrasonic.begin(25, 24, 23, 22, 27, 26);
  Serial.println("ultrasonic");

  qrd.begin(A9, A8, A10, A11,   A3, A0, A2, A1,    A7, A4, A6, A5,    A15, A12, A14, A13);
  Serial.println("qrd");

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

void loop () {
  Serial.println();
  if (!digitalRead(TOGGLE_SWITCH)) strategy.motor.active(false);
  else strategy.motor.active(true);

  strategy.camera.add("ox");
  strategy.camera.add("oy");
  strategy.camera.add("yi");
  strategy.camera.add("yx");
  strategy.camera.add("yw");
  strategy.camera.call();

  if (strategy.camera.ox() == -1) {
    strategy.endAction();
    strategy.passiveDeffense();
    return;
  }
  strategy.deffend();
}
