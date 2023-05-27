#include <Arduino.h>
#include "Camera.h"
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

bool ongoing_req = false;
Camera::Camera () {
  this -> baud = 115200;
  this -> timeout = 2;
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

void Camera::call() {
  unsigned long mill = millis();
  Serial1.println(queryString);
  String read = "";
  for (int i = 0; i < 100; i++) {
    if (Serial1.available()) {
      read = Serial1.readString();
      break;
    }
    delay(1);
  }
  save(read);
}

void Camera::call(bool log) {
  unsigned long mill = millis();
  Serial1.println(queryString);
  String read = "";
  for (int i = 0; i < 100; i++) {
    if (Serial1.available()) {
      read = Serial1.readString();
      if (log) Serial.println(read);
      break;
    }
    delay(1);
  }
  save(read);
}

void Camera::del() {
  querySize = 0;
  queryString = "";
}

void Camera::add(String string) {
  String query[queryString.length() / 2];
  for (int i = 0; i < queryString.length(); i += 2) {
    query[i / 2] = queryString.substring(i, i + 2);
  }
  if (includes(query, NUMITEMS(query), string)) return;
  querySize++;
  queryString += string;
  Serial.println(queryString);
}

void Camera::save(String result) {
  String arr[querySize];
  String res = result.substring(1, result.length() - 1);
  int amount_read = 0;
  int last_found = 0;
  for (int i = 0; i < res.length(); i ++) {
    if (res[i] == '#') {
      arr[amount_read] = res.substring(last_found, i);
      amount_read++;
      last_found = i + 1;
    }
  }
  arr[querySize - 1] = res.substring(last_found, res.length());
  for (int i = 0; i < querySize; i++) {
    String object = arr[i];
    String objectArr[2];
    for (int i = 0; i < object.length(); i++) {
      if (object[i] == ':') {
        objectArr[0] = object.substring(0, i);
      }
    }
    objectArr[1] = object.substring(3, object.length());
    if (objectArr[0] == "ox") { _ox = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
    else if (objectArr[0] == "oy") { _oy = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
    else if (objectArr[0] == "bx") { _bx = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
    else if (objectArr[0] == "by") { _by = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
    else if (objectArr[0] == "yx") { _yx = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
    else if (objectArr[0] == "yy") { _yy = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
    else if (objectArr[0] == "bi") { _bi = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
    else if (objectArr[0] == "yi") { _yi = objectArr[1] == "-1" ? -1 : objectArr[1].toInt(); }
  }
  querySize = 0;
  queryString = "";
}

bool Camera::includes(String arr[], int len, String target) {
  for (int i = 0; i < len; i++) if (arr[i] == target) return true;
  return false;
}