#include <Arduino.h>
#include "Oled.h"
#include "Qrd.h"
#define wardOffSpeed 255


// En momentos de crisis, cualquier solucion es valida
Qrd::Qrd() {}
void Qrd::begin (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4) {
  sensors[0].pin = n1;
  sensors[1].pin = n2;
  sensors[2].pin = n3;
  sensors[3].pin = n4;

  sensors[4].pin = s1;
  sensors[5].pin = s2;
  sensors[6].pin = s3;
  sensors[7].pin = s4;

  sensors[8].pin = w1;
  sensors[9].pin = w2;
  sensors[10].pin = w3;
  sensors[11].pin = w4;

  sensors[12].pin = e1;
  sensors[13].pin = e2;
  sensors[14].pin = e3;
  sensors[15].pin = e4;

  for (int i = 0; i < 16; i++) {
    pinMode(sensors[i].pin, INPUT);
  }

  sensors[0].limit = -65;
  sensors[1].limit = -80;
  sensors[2].limit = -80;
  sensors[3].limit = -80;
}

void Qrd::test() { Serial.println("Qrd.h: I'm Alive"); }

void Qrd::reset() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 10; j++) sensors[i].vArray[j] = 0;
  } 
}

bool Qrd::North(bool log, Oled *oled) {
  int n1 = _N1(log);
  int n2 = 0;
  int n3 = 0;
  int n4 = 0;
  // int n2 = _N2(log);
  // int n3 = _N3(log);
  // int n4 = _N4(log);
  countIndex++;
  if (n1 || n2 || n3 || n4) {
    if (n1) { oled -> print<String>(0, 0, "N1", 4); oled->print<int>(0, 32, sensors[0].value, 4); oled -> show(); Serial.print("    LINE DETECTED BY N1");};
    if (n2) { oled -> print<String>(0, 0, "N2", 4); oled->print<int>(0, 32, sensors[1].value, 4); oled -> show(); Serial.print("    LINE DETECTED BY N2");};
    if (n3) { oled -> print<String>(0, 0, "N3", 4); oled->print<int>(0, 32, sensors[2].value, 4); oled -> show(); Serial.print("    LINE DETECTED BY N3");};
    if (n4) { oled -> print<String>(0, 0, "N4", 4); oled->print<int>(0, 32, sensors[3].value, 4); oled -> show(); Serial.print("    LINE DETECTED BY N4");};
    reset();
    return true;
  }
  Serial.print("                        ");
  return false;
}

bool Qrd::_N1(bool log) {
  int sensor = 0;
  int val = analogRead(sensors[sensor].pin);
  for (int i = sensors[sensor].vArraySize; i > 0; i--) {
    sensors[sensor].vArray[i] = sensors[sensor].vArray[i - 1];
  }
  sensors[sensor].vArray[0] = val;

  int yA[5], xA[5];
  for (int i = 0; i < 5; i++) xA[i] = sensors[sensor].vArray[i];
  for (int i = 5; i < 10; i++) yA[i - 5] = sensors[sensor].vArray[i];

  int y, x;
  for (int i = 0; i < 5; i++) x += xA[i];
  for (int i = 0; i < 5; i++) y += yA[i];
  y /= 5;
  x /= 5;
  sensors[sensor].value = x - y;

  if (!log) return (x - y) < sensors[sensor].limit;
  Serial.print("(");
  Serial.print(countIndex);
  format<int>(countIndex, 6);
  Serial.print(", ");
  Serial.print(x);
  format<int>(x, 4);
  Serial.print(")  ");
  Serial.print("x: ");
  Serial.print(x);
  format<int>(x, 5);
  Serial.print(" y: ");
  Serial.print(y);
  format<int>(y, 5);
  Serial.print(" diff: ");
  Serial.print(x - y);
  format<int>(x - y, 5);


  return (x - y) < sensors[sensor].limit;
}

bool Qrd::_N2(bool log) {
  int sensor = 1;
  int val = analogRead(sensors[sensor].pin);
  for (int i = sensors[sensor].vArraySize; i > 0; i--) {
    sensors[sensor].vArray[i] = sensors[sensor].vArray[i - 1];
  }
  sensors[sensor].vArray[0] = val;

  int yA[5], xA[5];
  for (int i = 0; i < 5; i++) xA[i] = sensors[sensor].vArray[i];
  for (int i = 5; i < 10; i++) yA[i - 5] = sensors[sensor].vArray[i];

  int y, x;
  for (int i = 0; i < 5; i++) x += xA[i];
  for (int i = 0; i < 5; i++) y += yA[i];
  y /= 5;
  x /= 5;
  sensors[sensor].value = x - y;

  if (!log) return (x - y) < sensors[sensor].limit;
  Serial.print("(");
  Serial.print(countIndex);
  format<int>(countIndex, 6);
  Serial.print(", ");
  Serial.print(x - y);
  format<int>(x - y, 4);
  Serial.print(")    ");
  return (x - y) < sensors[sensor].limit;
}

bool Qrd::_N3(bool log) {
  int sensor = 2;
  int val = analogRead(sensors[sensor].pin);
  for (int i = sensors[sensor].vArraySize; i > 0; i--) {
    sensors[sensor].vArray[i] = sensors[sensor].vArray[i - 1];
  }
  sensors[sensor].vArray[0] = val;

  int yA[5], xA[5];
  for (int i = 0; i < 5; i++) xA[i] = sensors[sensor].vArray[i];
  for (int i = 5; i < 10; i++) yA[i - 5] = sensors[sensor].vArray[i];

  int y, x;
  for (int i = 0; i < 5; i++) x += xA[i];
  for (int i = 0; i < 5; i++) y += yA[i];
  y /= 5;
  x /= 5;
  sensors[sensor].value = x - y;

  if (!log) return (x - y) < sensors[sensor].limit;
  Serial.print("(");
  Serial.print(countIndex);
  format<int>(countIndex, 6);
  Serial.print(", ");
  Serial.print(x - y);
  format<int>(x - y, 4);
  Serial.print(")    ");
  return (x - y) < sensors[sensor].limit;
}

bool Qrd::_N4(bool log) {
  int sensor = 3;
  int val = analogRead(sensors[sensor].pin);
  for (int i = sensors[sensor].vArraySize; i > 0; i--) {
    sensors[sensor].vArray[i] = sensors[sensor].vArray[i - 1];
  }
  sensors[sensor].vArray[0] = val;

  int yA[5], xA[5];
  for (int i = 0; i < 5; i++) xA[i] = sensors[sensor].vArray[i];
  for (int i = 5; i < 10; i++) yA[i - 5] = sensors[sensor].vArray[i];

  int y, x;
  for (int i = 0; i < 5; i++) x += xA[i];
  for (int i = 0; i < 5; i++) y += yA[i];
  y /= 5;
  x /= 5;
  sensors[sensor].value = x - y;

  if (!log) return (x - y) < sensors[sensor].limit;
  Serial.print("(");
  Serial.print(countIndex);
  format<int>(countIndex, 6);
  Serial.print(", ");
  Serial.print(x - y);
  format<int>(x - y, 4);
  Serial.print(")    ");
  return (x - y) < sensors[sensor].limit;
}
