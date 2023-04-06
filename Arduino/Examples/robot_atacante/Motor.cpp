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
}

void Motor::begin(byte nwa, byte nwb, byte nea, byte neb, byte swa, byte swb, byte sea, byte seb) {
  init (nwa, nwb, nea, neb, swa, swb, sea, seb);
}

void Motor::attachUtils (byte buzzer, int width, int height) {
  utilsMotor.begin(buzzer, width, height);
}

void Motor::attachCompass (String type) {
  compassMotor.begin(type);
}

void Motor::init (byte nwa, byte nwb, byte nea, byte neb, byte swa, byte swb, byte sea, byte seb) {
  NWa = nwa;
  NWb = nwb;
  NEa = nea;
  NEb = neb;
  SWa = swa;
  SWb = swb;
  SEa = sea;
  SEb = seb;

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
  digitalWrite(NEa, LOW);
  digitalWrite(NEb, LOW);
  digitalWrite(NWa, LOW);
  digitalWrite(NWb, LOW);
  digitalWrite(SEa, LOW);
  digitalWrite(SEb, LOW);
  digitalWrite(SWa, LOW);
  digitalWrite(SWb, LOW);
  delay(d);
}

void Motor::test() {
  Serial.println("Motor.h: I'm alive");
}

void Motor::moveToAngle(int angle, int speed) {
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
}

void Motor::moveToAngle(int angle, int speed, int d) {
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
  delay(d);
}

void Motor::_NE(int id, int speed) {
  if (id == 0) {
    analogWrite(NEa, speed);
    analogWrite(NEb, 0);
    return;
  } else if (id == 1) {
    analogWrite(NEb, speed);
    analogWrite(NEa, 0);
    return;
  }
  analogWrite(NEa, 0);
  analogWrite(NEb, 0);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_NE(int speed) {
    if (speed >= 0) {
    analogWrite(NEa, speed);
    analogWrite(NEb, 0);
  } else {
    analogWrite(NEb, abs(speed));
    analogWrite(NEa, 0);
  }
}

void Motor::_NW(int id, int speed) {
  if (id == 0) {
    analogWrite(NWa, speed);
    analogWrite(NWb, 0);
    return;
  } else if (id == 1) {
    analogWrite(NWb, speed);
    analogWrite(NWa, 0);
    return;
  }
  analogWrite(NWa, 0);
  analogWrite(NWb, 0);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_NW(int speed) {
  if (speed >= 0) {
    analogWrite(NWa, speed);
    analogWrite(NWb, 0);
  } else {
    analogWrite(NWb, abs(speed));
    analogWrite(NWa, 0);
  }
}

void Motor::_SE(int id, int speed) {
  if (id == 0) {
    analogWrite(SEa, speed);
    analogWrite(SEb, 0);
    return;
  } else if (id == 1) {
    analogWrite(SEb, speed);
    analogWrite(SEa, 0);
    return;
  }
  analogWrite(SEa, 0);
  analogWrite(SEb, 0);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_SE(int speed) {
  if (speed >= 0) {
    analogWrite(SEa, speed);
    analogWrite(SEb, 0);
  } else {
    analogWrite(SEb, abs(speed));
    analogWrite(SEa, 0);
  }
}

void Motor::_SW(int id, int speed) {
  if (id == 0) {
    analogWrite(SWa, speed);
    analogWrite(SWb, 0);
    return;
  } else if (id == 1) {
    analogWrite(SWb, speed);
    analogWrite(SWa, 0);
    return;
  }
  analogWrite(SWa, 0);
  analogWrite(SWb, 0);
  Serial.println("Range Error: ID is expected to be or 0 or 1");
}
void Motor::_SW(int speed) {
    if (speed >= 0) {
    analogWrite(SWa, speed);
    analogWrite(SWb, 0);
  } else {
    analogWrite(SWb, abs(speed));
    analogWrite(SWa, 0);
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
  _NW(0, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(0, speed);
  delay(d);
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

void Motor::rotateToAngle(float initAngle, int destinyAngle, int speed, bool stop) {
  Serial.println("Incomplete");
}

void Motor::rotateToAngle0(float initAngle, int speed, bool stop) {
  while (initAngle > COMPASS_DEVIATION) {
    TurnLeft(speed);
    initAngle = compassMotor.checkAngle();
  }
  while (initAngle < -COMPASS_DEVIATION) {
    TurnRight(speed);
    initAngle = compassMotor.checkAngle();
  }

  if (stop) reset();
}
