#include <Arduino.h>
#include "Camera.h"

bool ongoing_req = false;
Camera::Camera () {
  this -> baud = 115200;
  this -> timeout = 1;
  this -> width = 320;
  this -> height = 240;
  this -> team = "";
}

void Camera::begin(String TEAM) {
  init (TEAM);
}

void Camera::init(String TEAM) {
  team = TEAM;
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

int Camera::direction(int value, int splits) {
  if (value > 0) {
    if (value <= (width / splits)) return 0;
    for (int i = 1; i < splits; i++) {
      if (value > ((width / splits) * i) && value < ((width / splits) * (i + 1))) return i - 1;
    }
    return splits - 1;
  }
  return -1;
}

int Camera::intercept () {
  if (ongoing_req) return -2;
  ongoing_req = true;
  team == "blue" ? Serial1.write("i") : Serial1.write("I");
  for (int i = 0; i < 50; i++) {
    if (Serial1.available()) {
      ongoing_req = false;
      return Serial1.readString().toInt();
    }
    delay(1);
  }

  ongoing_req = false;
  delay(2);
  return -1;
};

int Camera::xOrange () {
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
  delay(2);
  return -1;
}

int Camera::yOrange () {
  if (ongoing_req) return -2;
  ongoing_req = true;
  Serial1.write("O");
  for (int i = 0; i < 50; i++) {
    if (Serial1.available()) {
      ongoing_req = false;
      return Serial1.readString().toInt();
    }
    delay(1);
  }

  ongoing_req = false;
  delay(2);
  return -1;
}

int Camera::xEnemyGoal() {
  return team == "blue" ? xYellow() : xBlue();
}

int Camera::xBlue () {
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
  delay(2);
  return -1;
};
int Camera::xYellow () {
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
  delay(2);
  return -1;
};