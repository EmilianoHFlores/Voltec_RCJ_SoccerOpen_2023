#include <Arduino.h>
#include "Ultrasonic.h"

#define DEVIATION 5
#define WIDTH 98
#define HEIGHT 207
#define ROBOT_DIAMETER 17.5
Ultrasonic::Ultrasonic () {
  this -> EchoE = 0;
  this -> TriggerE = 0;
  this -> EchoW = 0;
  this -> TriggerW = 0;
  this -> EchoS = 0;
  this -> TriggerS = 0;

}

void Ultrasonic::begin (byte eche, byte trige, byte echw, byte trigw, byte echs, byte trigs) {
  this -> EchoE = eche;
  this -> TriggerE = trige;
  this -> EchoW = echw;
  this -> TriggerW = trigw;
  this -> EchoS = echs;
  this -> TriggerS = trigs;
  this -> west = -1;
  this -> east = -1;
  this -> south = -1;

  pinMode(EchoE, INPUT);
  pinMode(TriggerE, OUTPUT);
  pinMode(EchoW, INPUT);
  pinMode(TriggerW, OUTPUT);
  pinMode(EchoS, INPUT);
  pinMode(TriggerS, OUTPUT);

  digitalWrite(TriggerE, LOW);
  digitalWrite(TriggerW, LOW);
  digitalWrite(TriggerS, LOW);
}

void Ultrasonic::East() {
  digitalWrite(TriggerE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerE, LOW);
  float val = pulseIn(EchoE, HIGH) * 0.01723;
  if (val > 300) return;
  east = val;
}

void Ultrasonic::West() {
  digitalWrite(TriggerW, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerW, LOW);
  float val = pulseIn(EchoW, HIGH) * 0.01723;
  if (val > 300) return;
  west = val;
}

void Ultrasonic::South() {
  digitalWrite(TriggerS, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerS, LOW);
  float val = pulseIn(EchoS, HIGH) * 0.01723;
  if (val > 300) return;
  south = val;
}
