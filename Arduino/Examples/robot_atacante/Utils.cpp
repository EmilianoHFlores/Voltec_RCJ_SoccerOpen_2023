#include <Arduino.h>
#include "Utils.h"

Utils::Utils () {
  this -> buzzer = 0;
  this -> width = 0;
  this -> height = 0;
}

void Utils::begin (byte bzzr, int wdt, int hgt) {
  init (bzzr, wdt, hgt);
}

void Utils::init (byte bzzr, int wdt, int hgt) {
  buzzer = bzzr;
  width = wdt;
  height = hgt;
  pinMode(buzzer, OUTPUT);
}

void Utils::reset () {
  digitalWrite(buzzer, LOW);
}

void Utils::test () {
  Serial.println("Utils.h: I'm Alive");
}

int Utils::largestNumber (int N1, int N2, int N3, int N4) {
  int numbers[] = { N1, N2, N3, N4 };
  int largest = numbers[0];
  for (int i = 1; i < 4; i++)
    if (numbers[i] > largest) largest = numbers[i];
  return largest;
}

void Utils::buzzerDelay(int d) {
  const float max = (d / 3);
  const float mid = (d / 3);
  const float min = (d / 3);
  for (int i = 0; i <= (max / 200); i++) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  for (int i = 0; i < (mid / 100); i++) {
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
  }
  for (int i = 0; i < (mid / 50); i++) {
    digitalWrite(buzzer, HIGH);
    delay(25);
    digitalWrite(buzzer, LOW);
    delay(25);
  }
}

int Utils::checkDirection (int x) {
  return checkPreciseDirection(x, 3);
}

int Utils::checkPreciseDirection(int x, int splits) {
  if (x > 0) {
    if (x <= (width / splits)) return 0;
    for (int i = 1; i < splits; i++) {
      if (x > ((width / splits) * i) && x < ((width / splits) * (i + 1))) return i - 1;
    }
    return splits - 1;
  }
  return -1;
}

float Utils::to360angle (float angle) {
  return angle > 0 ? angle : 360 + angle;
}

float Utils::circle360(float angle) {
  if (angle >= 0) {
    if (angle > 360) {
      return angle - 360;
    }
  }

  if (abs(angle) > 360) {
    return angle + 360;
  }
  return angle > 0 ? 360 - angle : 360 + angle;  
}

bool Utils::inRange(int minValue, int maxValue, int value, bool orEqual) {
  if (orEqual) {
    return ((minValue <= value) && (value <= maxValue)); 
  }
  return ((minValue < value) && (value < maxValue));
}