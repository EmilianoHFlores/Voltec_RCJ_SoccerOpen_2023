#include <Servo.h>
#define brushless 8
#define potenciometer A8
#define led 3

Servo motor;
void setup () {
  Serial.begin(9600);
  pinMode(potenciometer, INPUT);
  pinMode(led, OUTPUT);
  motor.attach(brushless);
  motor.writeMicroseconds(1000);
  Serial.println("Setup finished");
}

void loop () {
  int val = map(analogRead(potenciometer), 102, 1023, 0, 255);
  int val_b = map(analogRead(potenciometer), 102, 1023, 1000, 2000);
  analogWrite(led, val);
  motor.writeMicroseconds(val_b);
}