#include <Arduino.h>
#include "Qrd.h"

#define wardOffSpeed 255


// En momentos de crisis, cualquier solucion es valida
Qrd::Qrd() {}
void Qrd::begin (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4) { init (n1, n2, n3, n4, s1, s2, s3, s4, e1, e2, e3, e4, w1, w2, w3, w4); }
void Qrd::init (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4) {
  sensors[0] = {n1, 1023, 0, 0};
  sensors[1] = {n2, 1023, 0, 0};
  sensors[2] = {n3, 1023, 0, 0};
  sensors[3] = {n4, 1023, 0, 0};

  sensors[4] = {s1, 1023, 0, 0};
  sensors[5] = {s2, 1023, 0, 0};
  sensors[6] = {s3, 1023, 0, 0};
  sensors[7] = {s4, 1023, 0, 0};

  sensors[8] = {w1, 1023, 0, 0};
  sensors[9] = {w2, 1023, 0, 0};
  sensors[10] = {w3, 1023, 0, 0};
  sensors[11] = {w4, 1023, 0, 0};

  sensors[12] = {e1, 1023, 0, 0};
  sensors[13] = {e2, 1023, 0, 0};
  sensors[14] = {e3, 1023, 0, 0};
  sensors[15] = {e4, 1023, 0, 0};

  for (int i = 0; i < 16; i++) {
    pinMode(sensors[i].pin, INPUT);
  }

  QRDoled.begin();
}

void Qrd::reset() { Serial.println("Unavailable"); }
void Qrd::test() { Serial.println("Qrd.h: I'm Alive");}

int Qrd::wardOff() {
  // if (North()) return 0;
  // if (South()) return 1;
  // if (East()) return 2;
  // if (West()) return 3;
  // return -1;
}

void Qrd::getThresholds () {
  for (int j = 0; j < 1; j++) {
    SensorData* p = &sensors[j];
    QRDoled.print(0, 0, "Sensor: ", 2); QRDoled.print(80, 0, j, 2); QRDoled.show();
    for (int i = 0; i < 75; i++) {
      int val = analogRead(p -> pin);
      if (p -> min > val) p -> min = val;
      if (p -> max < val) p -> max = val;
      p -> threshold = ((p -> min + p -> max) / 3);
      QRDoled.print(0, 0, "Sensor: ", 2); QRDoled.print(80, 0, j, 2);
      QRDoled.print(0, 50, "Threshold:", 1); QRDoled.print(60,50,p -> threshold, 1);
      QRDoled.print(90, 32, i, 1); QRDoled.show();
      // Serial.print(i);
      // Serial.print(". ");
      // Serial.print("act: ");
      // Serial.print(val);
      // Serial.print(" min: ");
      // Serial.print(p -> min);
      // Serial.print(" max: ");
      // Serial.print(p -> max);
      // Serial.print(" threshold: ");
      // Serial.println(p -> threshold);
    }
    delay(1000);
  }
}

int Qrd::_N1() {
  SensorData* p = &sensors[0];
  int val = analogRead(p -> pin);
  QRDoled.print(0, 0, val, 4);
  QRDoled.print(0, 32, p -> threshold, 4); QRDoled.show();
  Serial.print("N1: ");
  Serial.print(val);
  Serial.print("/");
  Serial.print(p -> threshold);
  Serial.print(":");
  Serial.print(val < p -> threshold);
  return val < p -> threshold;
}

int Qrd::_N2() {
  SensorData* p = &sensors[1];
  int val = analogRead(p -> pin);
  Serial.print("  ~~~  N2: ");
  Serial.print(val);
  Serial.print("/");
  Serial.print(p -> threshold);
  Serial.print(":");
  Serial.print(val < p -> threshold);
  return val < p -> threshold;
}

int Qrd::_N3() {
  SensorData* p = &sensors[2];
  int val = analogRead(p -> pin);
  Serial.print("  ~~~  N3: ");
  Serial.print(val);
  Serial.print("/");
  Serial.print(p -> threshold);
  Serial.print(":");
  Serial.print(val < p -> threshold);
  return val < p -> threshold;
}

int Qrd::_N4() {
  SensorData* p = &sensors[3];
  int val = analogRead(p -> pin);
  Serial.print("  ~~~  N4: ");
  Serial.print(val);
  Serial.print("/");
  Serial.print(p -> threshold);
  Serial.print(":");
  Serial.print(val < p -> threshold);
  return val < p -> threshold;
}