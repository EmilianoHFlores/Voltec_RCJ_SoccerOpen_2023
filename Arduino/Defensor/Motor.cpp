#include <Arduino.h>
#include "Motor.h"

#define COMPASS_DEVIATION 15

Motor::Motor() {
  this -> NWa = 0;
  this -> NWb = 0;
  this -> NEa = 0;
  this -> NEb = 0;
  this -> SWa = 0;
  this -> SWb = 0;
  this -> SEa = 0;
  this -> SEb = 0;
  this -> NWpwm = 0;
  this -> NEpwm = 0;
  this -> SWpwm = 0;
  this -> SEpwm = 0;
}

void Motor::begin(byte nwpwm, byte nwa, byte nwb, byte nepwm, byte nea, byte neb, byte swpwm, byte swa, byte swb, byte sepwm, byte sea, byte seb) {
  init (nwpwm, nwa, nwb, nepwm, nea, neb, swpwm, swa, swb, sepwm, sea, seb);
}

void Motor::attachUtils (byte buzzer, int width, int height) {
  utilsMotor.begin(buzzer, width, height);
}

void Motor::attachCompass (String type) {
  compassMotor.begin(type);
}


void Motor::init (byte nwpwm, byte nwa, byte nwb, byte nepwm, byte nea, byte neb, byte swpwm, byte swa, byte swb, byte sepwm, byte sea, byte seb) {
  NWpwm = nwpwm;
  NWa = nwa;
  NWb = nwb;
  NEpwm = nepwm;
  NEa = nea;
  NEb = neb;
  SWpwm = swpwm;
  SWa = swa;
  SWb = swb;
  SEpwm = sepwm;
  SEa = sea;
  SEb = seb;

  pinMode(SEpwm, OUTPUT);
  pinMode(SWpwm, OUTPUT);
  pinMode(NEpwm, OUTPUT);
  pinMode(NWpwm, OUTPUT);
  pinMode(NEa, OUTPUT);
  pinMode(NEb, OUTPUT);
  pinMode(NWa, OUTPUT);
  pinMode(NWb, OUTPUT);
  pinMode(SEa, OUTPUT);
  pinMode(SEb, OUTPUT);
  pinMode(SWa, OUTPUT);
  pinMode(SWb, OUTPUT);
}

void Motor::reset() {
  digitalWrite(SWpwm, LOW);
  digitalWrite(NWpwm, LOW);
  digitalWrite(SEpwm, LOW);
  digitalWrite(NEpwm, LOW);
  digitalWrite(NEa, LOW);
  digitalWrite(NEb, LOW);
  digitalWrite(NWa, LOW);
  digitalWrite(NWb, LOW);
  digitalWrite(SEa, LOW);
  digitalWrite(SEb, LOW);
  digitalWrite(SWa, LOW);
  digitalWrite(SWb, LOW);
}

void Motor::reset(int d) {
  reset();
  delay(d);
}

void Motor::test() {
  Serial.println("Motor.h: Hi, I'm alive");
}

void Motor::moveToAngle(int initAngle, int angle, int speed) {
  int NW_speed = speed * cos((PI / 180) * (angle + 315));
  int SW_speed = speed * cos((PI / 180) * (angle + 225));
  int SE_speed = speed * cos((PI / 180) * (angle + 135));
  int NE_speed = speed * cos((PI / 180) * (angle + 45));

  int numbers[] = { abs(NW_speed), abs(SW_speed), abs(SE_speed), abs(NE_speed) };
  int _largestNumber = numbers[0];
  for (int i = 1; i < 4; i++)
    if (numbers[i] > _largestNumber) _largestNumber = numbers[i];
  float ratio = float(speed) / float(_largestNumber);
  NW_speed *= ratio;
  SW_speed *= -ratio;
  SE_speed *= -ratio;
  NE_speed *= ratio;
  _NW(NW_speed);
  _SW(SW_speed);
  _SE(SE_speed);
  _NE(NE_speed);
  rotateToAngle(compassMotor.checkAngle(), initAngle, 75, false);
}

void Motor::moveToAngle(int initAngle, int angle, int speed, int d) {
  long mill = millis();
  while (mill + d >= millis()) {
    moveToAngle(initAngle, angle, speed);
  }
}

void Motor::_NE(int id, int speed) {
  analogWrite(NEpwm, speed);
  if (id == 0) {
    digitalWrite(NEa, 1);
    digitalWrite(NEb, 0);
    return;
  } else if (id == 1) {
    digitalWrite(NEa, 0);
    digitalWrite(NEb, 1);
    return;
  }
  analogWrite(NEpwm, LOW);
  digitalWrite(NEa, LOW);
  digitalWrite(NEb, LOW);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_NE(int speed) {
  analogWrite(NEpwm, abs(speed));
  if (speed >= 0) {
    digitalWrite(NEa, 1);
    digitalWrite(NEb, 0);
  } else {
    digitalWrite(NEa, 0);
    digitalWrite(NEb, 1);
  }
}


void Motor::_NW(int id, int speed) {
  analogWrite(NWpwm, speed);
  if (id == 0) {
    digitalWrite(NWa, 1);
    digitalWrite(NWb, 0);
    return;
  } else if (id == 1) {
    digitalWrite(NWa, 0);
    digitalWrite(NWb, 1);
    return;
  }
  analogWrite(NWpwm, LOW);
  digitalWrite(NWa, LOW);
  digitalWrite(NWb, LOW);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_NW(int speed) {
  analogWrite(NWpwm, abs(speed));
  if (speed >= 0) {
    digitalWrite(NWa, 1);
    digitalWrite(NWb, 0);
  } else {
    digitalWrite(NWa, 0);
    digitalWrite(NWb, 1);
  }
}

void Motor::_SE(int id, int speed) {
  analogWrite(SEpwm, speed);
  if (id == 0) {
    digitalWrite(SEa, 1);
    digitalWrite(SEb, 0);
    return;
  } else if (id == 1) {
    digitalWrite(SEa, 0);
    digitalWrite(SEb, 1);
    return;
  }
  analogWrite(SEpwm, LOW);
  digitalWrite(SEa, LOW);
  digitalWrite(SEb, LOW);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_SE(int speed) {
  analogWrite(SEpwm, abs(speed));
  if (speed >= 0) {
    digitalWrite(SEa, 1);
    digitalWrite(SEb, 0);
  } else {
    digitalWrite(SEa, 0);
    digitalWrite(SEb, 1);
  }
}

void Motor::_SW(int id, int speed) {
  analogWrite(SWpwm, speed);
  if (id == 0) {
    digitalWrite(SWa, 1);
    digitalWrite(SWb, 0);
    return;
  } else if (id == 1) {
    digitalWrite(SWa, 0);
    digitalWrite(SWb, 1);
    return;
  }
  analogWrite(SWpwm, LOW);
  digitalWrite(SWa, LOW);
  digitalWrite(SWb, LOW);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_SW(int speed) {
  analogWrite(SWpwm, abs(speed));
  if (speed >= 0) {
    digitalWrite(SWa, 1);
    digitalWrite(SWb, 0);
  } else {
    digitalWrite(SWa, 0);
    digitalWrite(SWb, 1);
  }
}

void Motor::NorthWest(int speed) {
  _NE(0, speed);
  _SW(0, speed);
  _NW(0, 0);
  _SE(0, 0);
}
void Motor::NorthWest(int speed, int d) {
  _NE(0, speed);
  _SW(0, speed);
  _NW(0, 0);
  _SE(0, 0);
  delay(d);
}

void Motor::NorthEast(int speed) {
  _NW(0, speed);
  _SE(0, speed);
  _NE(0, 0);
  _SW(0, 0);
}
void Motor::NorthEast(int speed, int d) {
  _NW(0, speed);
  _SE(0, speed);
  _NE(0, 0);
  _SW(0, 0);
  delay(d);
}

void Motor::SouthEast(int speed) {
  _NE(1, speed);
  _SW(1, speed);
  _NW(0, 0);
  _SE(0, 0);
}
void Motor::SouthEast(int speed, int d) {
  _NE(1, speed);
  _SW(1, speed);
  _NW(0, 0);
  _SE(0, 0);
  delay(d);
}

void Motor::SouthWest(int speed) {
  _NW(1, speed);
  _SE(1, speed);
  _NE(0, 0);
  _SW(0, 0);
}
void Motor::SouthWest(int speed, int d) {
  _NW(1, speed);
  _SE(1, speed);
  _NE(0, 0);
  _SW(0, 0);
  delay(d);
}

void Motor::North(int speed) {
  _NW(0, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(0, speed);
}
void Motor::North(int speed, int d) {
  long mill = millis();
  while (mill + d >= millis()) {
    North(speed);   // Robot must break while when line is detected
  }
}

void Motor::South(int speed) {
  _NW(1, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(1, speed);
}
void Motor::South(int speed, int d) {
  _NW(1, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(1, speed);
  delay(d);
}

void Motor::East(int speed) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(0, speed);
}
void Motor::East(int speed, int d) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(0, speed);
  delay(d);
}

void Motor::West(int speed) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(1, speed);
}
void Motor::West(int speed, int d) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(1, speed);
  delay(d);
}

void Motor::TurnLeft(int speed) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(1, speed);
  _SE(0, speed);
};

void Motor::TurnLeft(int speed, int d) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(1, speed);
  _SE(0, speed);
  delay(d);
};

void Motor::TurnRight(int speed) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(0, speed);
  _SE(1, speed);
};

void Motor::TurnRight(int speed, int d) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(0, speed);
  _SE(1, speed);
  delay(d);
};

void Motor::rotateToAngle(float _i, int _d, int speed, bool stop) {
  int d = 0;
  int d_min = d - COMPASS_DEVIATION;
  int d_max = d + COMPASS_DEVIATION;
  float i = _d - _i;

  if ((d_min < i) && (i < d_max)) {
    if (stop) hardReset();
    return;
  }
  if (i < 0 || i > 180) TurnLeft(speed);
  else TurnRight(speed);
  rotateToAngle(compassMotor.checkAngle(), _d, speed, stop);
}

void Motor::hardReset() {
  digitalWrite(NEa, 1);
  digitalWrite(NEb, 1);
  digitalWrite(NEpwm, 255);

  digitalWrite(NWa, 1);
  digitalWrite(NWb, 1);
  digitalWrite(NWpwm, 255);

  digitalWrite(SWa, 1);
  digitalWrite(SWb, 1);
  digitalWrite(SWpwm, 255);


  digitalWrite(SEa, 1);
  digitalWrite(SEb, 1);
  digitalWrite(SEpwm, 255);
}

void Motor::forceStop() {
  for (int i = 0; i < 400; i++) {
    if (i % 2 == 0) North(50, 1);
    else South(50, 1);
  }
  for (int i = 0; i < 100; i++) {
    if (i % 2 == 0) North(1, 1);
    else South(1, 1);
  }
}

// bool Motor::goalOutRange(int gcy) {
//   if (gcy == -1) return false;
//   else if (gcy <= 30) {
//     rotateToAngle(compassMotor.checkAngle(), 0, 75, true);
//     moveToAngle(compassMotor.checkAngle(), 180, 255, 1000);
//     return true;
//   }
//   return false;
// }