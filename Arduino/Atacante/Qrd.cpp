#include <Arduino.h>
#include "Qrd.h"

#define wardOffSpeed 255


// En momentos de crisis, cualquier solucion es valida
int N1 = 0;int N2 = 0;int N3 = 0;int N4 = 0;int S1 = 0;int S2 = 0;int S3 = 0;int S4 = 0;int E1 = 0;int E2 = 0;int E3 = 0;int E4 = 0;int W1 = 0;int W2 = 0;int W3 = 0;int W4 = 0;
int N1_min = 1023;int N2_min = 1023;int N3_min = 1023;int N4_min = 1023;
Qrd::Qrd() {}

void Qrd::begin (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4) {
  init (n1, n2, n3, n4, s1, s2, s3, s4, e1, e2, e3, e4, w1, w2, w3, w4);
}

void Qrd::init (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4) {
  N1 = n1;
  N2 = n2;
  N3 = n3;
  N4 = n4;
  S1 = s1;
  S2 = s2;
  S3 = s3;
  S4 = s4;
  W1 = w1;
  W2 = w2;
  W3 = w3;
  W4 = w4;
  E1 = e1;
  E2 = e2;
  E3 = e3;
  E4 = e4;
  pinMode(N1, INPUT);
  pinMode(N2, INPUT);
  pinMode(N3, INPUT),
  pinMode(N4, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(E1, INPUT);
  pinMode(E2, INPUT);
  pinMode(E3, INPUT);
  pinMode(E4, INPUT);
  pinMode(W1, INPUT);
  pinMode(W2, INPUT);
  pinMode(W3, INPUT);
  pinMode(W4, INPUT);
}

void Qrd::reset() {
  Serial.println("Unavailable");
}

void Qrd::test() {
  Serial.println("Qrd.h: I'm Alive");
}

void Qrd::attachCompass(String type) {
  QRDcompass.begin(type);
}

void Qrd::setThresholds() {
  N1_min = N1_min_avg(2) - 1;
  N2_min = N2_min_avg(2) - 1;
  N3_min = N3_min_avg(2) - 1;
  N4_min = N4_min_avg(2) - 1;
}

int Qrd::wardOff() {
  if (North()) return 0;
  return -1;
}

bool Qrd::North() {
  int n1_ = inN1(); int n2_ = inN2(); int n3_ = inN3(); int n4_ = inN4();
  int result = (n1_ + n2_ + n3_ + n4_) > 0;
  return result;
}

bool Qrd::inN1() {
  return N1_min > _N1();
}

bool Qrd::inN2() {
  return N2_min > _N2();
}

bool Qrd::inN3() {
  return N3_min > _N3();
};

bool Qrd::inN4() {
  return N4_min > _N4();
};

int Qrd::_N1() { return analogRead(N1); }
int Qrd::_N2() { return analogRead(N2); }
int Qrd::_N3() { return analogRead(N3); }
int Qrd::_N4() { return analogRead(N4); }
int Qrd::_S1() { return analogRead(S1); }
int Qrd::_S2() { return analogRead(S2); }
int Qrd::_S3() { return analogRead(S3); }
int Qrd::_S4() { return analogRead(S4); }
int Qrd::_E1() { return analogRead(E1); }
int Qrd::_E2() { return analogRead(E2); }
int Qrd::_E3() { return analogRead(E3); }
int Qrd::_E4() { return analogRead(E4); }
int Qrd::_W1() { return analogRead(W1); }
int Qrd::_W2() { return analogRead(W2); }
int Qrd::_W3() { return analogRead(W3); }
int Qrd::_W4() { return analogRead(W4); }


void Qrd::logNorthValues() {
  Serial.print("N1: ");
  Serial.print(_N1());
  Serial.print(" N2: ");
  Serial.print(_N2());
  Serial.print(" N3: ");
  Serial.print(_N3());
  Serial.print(" N4: ");
  Serial.println(_N4());
}


void Qrd::logSouthValues() {
  Serial.print("S1: ");
  Serial.print(_S1());
  Serial.print(" S2: ");
  Serial.print(_S2());
  Serial.print(" S3: ");
  Serial.print(_S3());
  Serial.print(" S4: ");
  Serial.println(_S4());
}


void Qrd::logEastValues() {
  Serial.print("E1: ");
  Serial.print(_E1());
  Serial.print(" E2: ");
  Serial.print(_E2());
  Serial.print(" E3: ");
  Serial.print(_E3());
  Serial.print(" E4: ");
  Serial.println(_E4());
}


void Qrd::logWestValues() {
  Serial.print("W1: ");
  Serial.print(_W1());
  Serial.print(" W2: ");
  Serial.print(_W2());
  Serial.print(" W3: ");
  Serial.print(_W3());
  Serial.print(" W4: ");
  Serial.println(_W4());
}

int Qrd::get_N1_min() { return N1_min; };
int Qrd::get_N2_min() { return N2_min; };
int Qrd::get_N3_min() { return N3_min; };
int Qrd::get_N4_min() { return N4_min; };

int Qrd::N1_min_avg (int qty) {
  int _n_val;
  int _min_val = 1023;
  for (int i = 0; i < qty; i++) {
    _n_val = _N1();
    if (_min_val > _n_val) _min_val = _n_val;
  }
  return _min_val;
}
int Qrd::N2_min_avg (int qty) {
  int _n_val;
  int _min_val = 1023;
  for (int i = 0; i < qty; i++) {
    _n_val = _N2();
    if (_min_val > _n_val) _min_val = _n_val;
  }
  return _min_val;
}
int Qrd::N3_min_avg (int qty) {
  int _n_val;
  int _min_val = 1023;
  for (int i = 0; i < qty; i++) {
    _n_val = _N3();
    if (_min_val > _n_val) _min_val = _n_val;
  }
  return _min_val;
}
int Qrd::N4_min_avg (int qty) {
  int _n_val;
  int _min_val = 1023;
  for (int i = 0; i < qty; i++) {
    _n_val = _N4();
    if (_min_val > _n_val) _min_val = _n_val;
  }
  return _min_val;
}
