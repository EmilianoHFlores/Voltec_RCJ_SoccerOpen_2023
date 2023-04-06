#include <Arduino.h>
#include "Camera.h"

bool ongoing_req = false;
Camera::Camera () {
  this -> baud = 0;
  this -> timeout = 0;

}

void Camera::begin(long bd, int tmo) {
  init (bd, tmo);
}

void Camera::init (long bd, int tmo) {
  baud = bd;
  timeout = tmo;
  Serial1.begin(baud);
  Serial1.setTimeout(timeout);
}

void Camera::reset () {
  Serial1.end();
  Serial1.begin(baud);
  Serial1.setTimeout(timeout);
}

void Camera::test () {
  Serial.println("Camera.h: I'm Alive");
}

int Camera::callOrange () {
  if (ongoing_req) return -2;
  ongoing_req = true;
  Serial1.write("o");
  for (int i = 0; i < 50; i++) {
    if (Serial1.available()) {
      ongoing_req = false;
      return Serial1.readString().toInt();
    }
    delay(1);
  }

  ongoing_req = false;
  return -1;
};
int Camera::callBlue () {
  if (ongoing_req) return -2;
  ongoing_req = true;
  Serial1.write("b");
  for (int i = 0; i < 50; i++) {
    if (Serial1.available()) {
      ongoing_req = false;
      return Serial1.readString().toInt();
    }
    delay(1);
  }

  ongoing_req = false;
  return -1;
};
int Camera::callYellow () {
  if (ongoing_req) return -2;
  ongoing_req = true;
  Serial1.write("y");
  for (int i = 0; i < 50; i++) {
    if (Serial1.available()) {
      ongoing_req = false;
      return Serial1.readString().toInt();
    }
    delay(1);
  }

  ongoing_req = false;
  return -1;
};
