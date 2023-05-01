#include <Arduino.h>
#include "Ultrasonic.h"

#define DEVIATION 10
#define WIDTH 106
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

void Ultrasonic::begin(byte eche, byte trige, byte echw, byte trigw, byte echs, byte trigs) {
  init (eche, trige, echw, trigw, echs, trigs);
}

void Ultrasonic::init (byte eche, byte trige, byte echw, byte trigw, byte echs, byte trigs) {
  this -> EchoE = eche;
  this -> TriggerE = trige;
  this -> EchoW = echw;
  this -> TriggerW = trigw;
  this -> EchoS = echs;
  this -> TriggerS = trigs;

  pinMode(EchoE, INPUT);
  pinMode(TriggerE, OUTPUT);
  pinMode(EchoW, INPUT);
  pinMode(TriggerW, OUTPUT);
  pinMode(EchoS, INPUT);
  pinMode(TriggerS, OUTPUT);
  reset();
}

void Ultrasonic::reset() {
  digitalWrite(TriggerE, LOW);
  digitalWrite(TriggerW, LOW);
  digitalWrite(TriggerS, LOW);
}

void Ultrasonic::test() {
  Serial.println("Ultrasonic.h: I'm Alive");
}

float Ultrasonic::East() {
  digitalWrite(TriggerE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerE, LOW);
  return pulseIn(EchoE, HIGH) * 0.01723;
}

float Ultrasonic::West() {
  digitalWrite(TriggerW, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerW, LOW);
  return pulseIn(EchoW, HIGH) * 0.01723;
}

float Ultrasonic::South() {
  digitalWrite(TriggerS, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerS, LOW);
  return pulseIn(EchoS, HIGH) * 0.01723;
}

float Ultrasonic::x_location () {
  float east = East();
  
  // float east = East();
  // float west = West();
  // if ((east + west) > (WIDTH - ROBOT_DIAMETER - DEVIATION) && (east + west) < (WIDTH - ROBOT_DIAMETER + DEVIATION)) {
  //   return east;
  // }

  return WIDTH - east - (ROBOT_DIAMETER / 2);
}

float Ultrasonic::y_location () {
  return South();
}

float Ultrasonic::angleToGoal() {
  float a = x_location();
  float b = y_location();
  if (a == -1) return -1;
  return atan(a / b) * (180 / PI);
}