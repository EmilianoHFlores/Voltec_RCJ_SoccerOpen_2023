#include <Arduino.h>
#include "Qrd.h"

#define wardOffSpeed 255


// En momentos de crisis, cualquier solucion es valida
int N1 = 0;int N2 = 0;int N3 = 0;int N4 = 0;int S1 = 0;int S2 = 0;int S3 = 0;int S4 = 0;int E1 = 0;int E2 = 0;int E3 = 0;int E4 = 0;int W1 = 0;int W2 = 0;int W3 = 0;int W4 = 0;int N1_min = 0;int N2_min = 0;int N3_min = 0;int N4_min = 0;

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

void Qrd::attachCompass() {
  QRDcompass.begin();
}

void Qrd::attachUtils(byte buzzer, int width, int height) {
  QRDutils.begin(buzzer, width, height);
}

int Qrd::wardOff() {
  if (North()) return 0;
  return -1;
}

bool Qrd::North() {
  int n1_ = inN1(); int n2_ = inN2(); int n3_ = inN3(); int n4_ = inN4();
  // Serial.print("N1: "); Serial.print(n1_);
  // Serial.print(" N2: "); Serial.print(n2_);
  // Serial.print(" N3: "); Serial.print(n3_);
  // Serial.print(" N4: "); Serial.print(n4_);
  
  int result = (n1_ + n2_ + n3_ + n4_) > 0;
  // Serial.print("  Result -> ");
  // Serial.println(result);
  return result;
}

// bool Qrd::South() { return ((inS1() + inS2() + inS3() + inS4()) > 1); }
// bool Qrd::East() { return ((inE1() + inE2() + inE3() + inE4()) > 1); }
// bool Qrd::West() { return ((inW1() + inW2() + inW3 ()+ inW4()) > 1); }
// bool Qrd::isInLine() { return ((West() + North() + East() + South()) > 0); }

int lastInN1 = 0;
int n1_difference = 3;
bool Qrd::inN1() {
  bool to_return = false;
  int actual = _N1();
  if (lastInN1 - n1_difference >= actual) to_return = true;
  // Serial.print("Actual: "); Serial.print(actual);
  // Serial.print(" Last: "); Serial.print(lastInN1);
  // Serial.print(" return: "); Serial.println(to_return);
  // if (to_return) delay(1500);

  lastInN1 = actual;
  return to_return; 
}

int lastInN2 = 0;
int n2_difference = 3;
bool Qrd::inN2() {
  bool to_return = false;
  int actual = _N2();
  if (lastInN2 - n2_difference >= actual) to_return = true;
  Serial.print("Actual: "); Serial.print(actual);
  Serial.print(" Last: "); Serial.print(lastInN2);
  Serial.print(" return: "); Serial.println(to_return);
  if (to_return) delay(1500);

  lastInN2 = actual;
  return to_return; 
}

bool Qrd::inN3() {};

bool Qrd::inN4() {};

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

