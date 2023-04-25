#include <Arduino.h>
#include "Camera.h"

bool ongoing_req = false;
Camera::Camera () {
  this -> baud = 0;
  this -> timeout = 0;
  this -> team = "";
}

void Camera::begin(long bd, int tmo, String TEAM) {
  init (bd, tmo, TEAM);
}

void Camera::init (long bd, int tmo, String TEAM) {
  baud = bd;
  timeout = tmo;
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

int Camera::xOwnGoal() {
  return team == "blue" ? xBlue() : xYellow();
}

int Camera::xEnemyGoal() {
  return team == "blue" ? xYellow() : xBlue();
}



// int Camera::yOwnGoal() {
//   return team == "blue" ? yBlue() : yYellow();
// }

// int Camera::yEnemyGoal() {
//   return team == "blue" ? yYellow() : yBlue();
// }

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

// int Camera::yYellow () {
//   if (ongoing_req) return -2;
//   ongoing_req = true;
//   Serial1.write("Y");
//   for (int i = 0; i < 50; i++) {
//     if (Serial1.available()) {
//       ongoing_req = false;
//       return Serial1.readString().toInt();
//     }
//     delay(1);
//   }

//   ongoing_req = false;
//   delay(2);
//   return -1;
// };
// int Camera::yBlue () {
//   if (ongoing_req) return -2;
//   ongoing_req = true;
//   Serial1.write("B");
//   for (int i = 0; i < 50; i++) {
//     if (Serial1.available()) {
//       ongoing_req = false;
//       return Serial1.readString().toInt();
//     }
//     delay(1);
//   }

//   ongoing_req = false;
//   delay(2);
//   return -1;
// };


