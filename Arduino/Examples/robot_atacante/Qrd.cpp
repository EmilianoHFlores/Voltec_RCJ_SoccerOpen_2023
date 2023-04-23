#include <Arduino.h>
#include "Qrd.h"
#include "Motor.h"
#include "Compass.h"
#include "Utils.h"

#define wardOffSpeed 255

Motor QRDmotor;
Compass QRDcompass;
Utils QRDutils;


bool northContinuity = false;
int lastN1 = 0;
int preLastN1 = 0;
int lastN2 = 0;
int preLastN2 = 0;
int lastN3 = 0;
int preLastN3 = 0;
int lastN4 = 0;
int preLastN4 = 0;

Qrd::Qrd() {
  this -> N1 = 0;
  this -> N2 = 0;
  this -> N3 = 0;
  this -> N4 = 0;
  this -> S1 = 0;
  this -> S2 = 0;
  this -> S3 = 0;
  this -> S4 = 0;
  this -> E1 = 0;
  this -> E2 = 0;
  this -> E3 = 0;
  this -> E4 = 0;
  this -> W1 = 0;
  this -> W2 = 0;
  this -> W3 = 0;
  this -> W4 = 0;
}

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
  northContinuity = false;
}

void Qrd::test() {
  Serial.println("Qrd.h: I'm Alive");
}

void Qrd::attachMotors(byte NWpwm, byte NWa, byte NWb, byte NEpwm, byte NEa, byte NEb, byte SWpwm, byte SWa, byte SWb, byte SEpwm, byte SEa, byte SEb) {
  QRDmotor.begin(NWpwm, NWa, NWb, NEpwm, NEa, NEb, SWpwm, SWa, SWb, SEpwm, SEa, SEb);
}

void Qrd::attachCompass() {
  QRDcompass.begin();  
}

void Qrd::attachUtils(byte buzzer, int width, int height) {
  QRDutils.begin(buzzer, width, height);
}

void Qrd::wardOff() {
  if (North() == 1) {
    QRDmotor.South(wardOffSpeed, 500);
    QRDmotor.reset();
    reset();
  }
}

bool Qrd::North() { 
  bool n1 = inN1();
  bool n2 = inN2();
  bool n3 = inN3();
  bool n4 = inN4();
  Serial.print("   N1: ");
  Serial.print(n1);
  Serial.print(" N2: ");
  Serial.print(n2);
  Serial.print(" N3: ");
  Serial.print(n3);
  Serial.print(" N4: ");
  Serial.println(n4);
  if((n1 + n2 + n3 + n4) >= 2) {
    if (northContinuity) return true;
    northContinuity = true;
  } else {
    if (northContinuity) northContinuity = false;
    return false;
  }
}
// bool Qrd::South() { return ((inS1() + inS2() + inS3() + inS4()) > 1); }
// bool Qrd::East() { return ((inE1() + inE2() + inE3() + inE4()) > 1); }
// bool Qrd::West() { return ((inW1() + inW2() + inW3 ()+ inW4()) > 1); }
// bool Qrd::isInLine() { return ((West() + North() + East() + South()) > 0); }

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

bool Qrd::inN1() {
  int actualN1 = _N1();
  bool toReturn = false;

  // Serial.print("preLast: "); Serial.print(preLastN1);
  // Serial.print(" Last: "); Serial.print(lastN1);
  // Serial.print(" Actual: "); Serial.print(actualN1);
  // Serial.print(" Resultado: "); Serial.print(((preLastN1 - 3) >= actualN1) || ((lastN1 - 3) >= actualN1));

  if (((preLastN1 - 3) >= actualN1) || ((lastN1 - 3) >= actualN1)) toReturn = true;
  preLastN1 = lastN1;
  lastN1 = actualN1;
  return toReturn;
}

bool Qrd::inN2() {
  int actualN2 = _N2();
  bool toReturn = false;
  // Serial.print("preLast: "); Serial.print(preLastN2);
  // Serial.print(" Last: "); Serial.print(lastN2);
  // Serial.print(" Actual: "); Serial.print(actualN2);
  // Serial.print(" Resultado: "); Serial.print(((preLastN2 - 3) >= actualN2) || ((lastN2 - 3) >= actualN2));

  if (((preLastN2 - 3) >= actualN2) || ((lastN2 - 3) >= actualN2)) toReturn = true;
  preLastN2 = lastN2;
  lastN2 = actualN2;
  return toReturn;
}

bool Qrd::inN3() {
  int actualN3 = _N3();
  bool toReturn = false;
  // Serial.print("preLast: "); Serial.print(preLastN3);
  // Serial.print(" Last: "); Serial.print(lastN3);
  // Serial.print(" Actual: "); Serial.print(actualN3);
  // Serial.print(" Resultado: "); Serial.print(((preLastN3 - 3) >= actualN3) || ((lastN3 - 3) >= actualN3));

  if (((preLastN3 - 3) >= actualN3) || ((lastN3 - 3) >= actualN3)) toReturn = true;
  preLastN3 = lastN3;
  lastN3 = actualN3;
  return toReturn;
}

bool Qrd::inN4() {
  int actualN4 = _N4();
  bool toReturn = false;
  // Serial.print("preLast: "); Serial.print(preLastN4);
  // Serial.print(" Last: "); Serial.print(lastN4);
  // Serial.print(" Actual: "); Serial.print(actualN4);
  // Serial.print(" Resultado: "); Serial.print(((preLastN4 - 3) >= actualN4) || ((lastN4 - 3) >= actualN4));
  if (((preLastN4 - 3) >= actualN4) || ((lastN4 - 3) >= actualN4)) toReturn = true;
  preLastN4 = lastN4;
  lastN4 = actualN4;
  return toReturn;
}

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
