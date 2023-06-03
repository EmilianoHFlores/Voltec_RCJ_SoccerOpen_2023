#include <Arduino.h>
#include "Motor.h"

#define COMPASS_DEVIATION 15

Motor::Motor() {}

void Motor::attachCompass (String type) {
  compassMotor.begin(type);
}

void Motor::attachQrd (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4) {
  qrdMotor.begin(n1, n2, n3, n4, s1, s2, s3, s4, e1, e2, e3, e4, w1, w2, w3, w4);
  qrdMotor.getThresholds();
}

void Motor::begin (byte nwpwm, byte nwa, byte nwb, byte nepwm, byte nea, byte neb, byte swpwm, byte swa, byte swb, byte sepwm, byte sea, byte seb) {
  _NE = {nea, neb, nepwm, 0};
  _NW = {nwa, nwb, nwpwm, 0};
  _SE = {sea, seb, sepwm, 0};
  _SW = {swa, swb, swpwm, 0};

  pinMode(_SE.pwm, OUTPUT);
  pinMode(_SW.pwm, OUTPUT);
  pinMode(_NE.pwm, OUTPUT);
  pinMode(_NW.pwm, OUTPUT);

  pinMode(_SE.b, OUTPUT);
  pinMode(_SW.b, OUTPUT);
  pinMode(_NE.b, OUTPUT);
  pinMode(_NW.b, OUTPUT);

  pinMode(_SE.a, OUTPUT);
  pinMode(_SW.a, OUTPUT);
  pinMode(_NE.a, OUTPUT);
  pinMode(_NW.a, OUTPUT);

  oledMotor.begin();
}

void Motor::moveToAngle(int initAngle, int angle, int speed) {
  wardOff();
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
  fNW(NW_speed);
  fSW(SW_speed);
  fSE(SE_speed);
  fNE(NE_speed);
  rotateToAngle(compassMotor.checkAngle(), initAngle, 75, false);
}

void Motor::moveToAngle(int initAngle, int angle, int speed, int d) {
  long mill = millis();
  while (mill + d >= millis()) {
    moveToAngle(initAngle, angle, speed);
  }
}


void Motor::NorthWest(int speed) {
  wardOff();
  fNE(0, speed);
  fSW(0, speed);
  fNW(0, 0);
  fSE(0, 0);
}

void Motor::NorthEast(int speed) {
  wardOff();
  fNW(0, speed);
  fSE(0, speed);
  fNE(0, 0);
  fSW(0, 0);
}

void Motor::SouthEast(int speed) {
  wardOff();
  fNE(1, speed);
  fSW(1, speed);
  fNW(0, 0);
  fSE(0, 0);
}

void Motor::SouthWest(int speed) {
  wardOff();
  fNW(1, speed);
  fSE(1, speed);
  fNE(0, 0);
  fSW(0, 0);
}

void Motor::North(int speed) {
  wardOff();
  fNW(0, speed);
  fNE(0, speed);
  fSW(0, speed);
  fSE(0, speed);
}

void Motor::South(int speed) {
  wardOff();
  fNW(1, speed);
  fNE(1, speed);
  fSW(1, speed);
  fSE(1, speed);
}

void Motor::East(int speed) {
  wardOff();
  fNW(0, speed);
  fNE(1, speed);
  fSW(1, speed);
  fSE(0, speed);
}

void Motor::West(int speed) {
  wardOff();
  fNW(1, speed);
  fNE(0, speed);
  fSW(0, speed);
  fSE(1, speed);
}

void Motor::TurnLeft(int speed) {
  fNW(1, speed);
  fNE(0, speed);
  fSW(1, speed);
  fSE(0, speed);
};

void Motor::TurnRight(int speed) {
  fNW(0, speed);
  fNE(1, speed);
  fSW(0, speed);
  fSE(1, speed);
};

void Motor::rotateToAngle(float _i, int _d, int speed, bool stop) {
  int d = 0;
  int d_min = d - COMPASS_DEVIATION;
  int d_max = d + COMPASS_DEVIATION;
  float i = _d - _i;
  if ((d_min < i) && (i < d_max)) {
    if (stop) Stop();
    return;
  }
  if (i < 0 || i > 180) TurnLeft(speed);
  else TurnRight(speed);
  rotateToAngle(compassMotor.checkAngle(), _d, speed, stop);
}

void Motor::Stop() {
  fNE(2, 255);
  fNW(2, 255);
  fSE(2, 255);
  fSW(2, 255);
}

void Motor::wardOff() {
  // int delayTime = 350;
  // switch (qrdMotor.wardOff()) {
  // case 0:
  //   oledMotor.print(0, 0, "LINE INTERRUPT", 3); oledMotor.show();
  //   South(maxSpeed);
  //   delay(delayTime);
  //   break;
  // case 1:
  //   oledMotor.print(0, 0, "LINE INTERRUPT", 3); oledMotor.show();
  //   hardReset();
  //   North(maxSpeed);
  //   delay(delayTime);
  //   break;
  // case 2:
  //   oledMotor.print(0, 0, "LINE INTERRUPT", 3); oledMotor.show();
  //   hardReset();
  //   West(maxSpeed);
  //   delay(delayTime);
  //   break;
  // case 3:
  //   oledMotor.print(0, 0, "LINE INTERRUPT", 3); oledMotor.show();
  //   hardReset();
  //   East(maxSpeed);
  //   delay(delayTime);
  //   break;
  // }
}

void Motor::fNE(int id, int speed) {
  analogWrite(_NE.pwm, speed - _NE.speedDeviation);
  if (id == 0) {
    digitalWrite(_NE.a, 1);
    digitalWrite(_NE.b, 0);
    return;
  } else if (id == 1) {
    digitalWrite(_NE.a, 0);
    digitalWrite(_NE.b, 1);
    return;
  }
  analogWrite(_NE.pwm, LOW);
  digitalWrite(_NE.a, HIGH);
  digitalWrite(_NE.b, HIGH);
}
void Motor::fNE(int speed) {
  analogWrite(_NE.pwm, abs(speed) - _NE.speedDeviation);
  if (speed >= 0) {
    digitalWrite(_NE.a, 1);
    digitalWrite(_NE.b, 0);
  } else {
    digitalWrite(_NE.a, 0);
    digitalWrite(_NE.b, 1);
  }
}


void Motor::fNW(int id, int speed) {
  analogWrite(_NW.pwm, speed - _NW.speedDeviation);
  if (id == 0) {
    digitalWrite(_NW.a, 1);
    digitalWrite(_NW.b, 0);
    return;
  } else if (id == 1) {
    digitalWrite(_NW.a, 0);
    digitalWrite(_NW.b, 1);
    return;
  }
  analogWrite(_NW.pwm, LOW);
  digitalWrite(_NW.a, HIGH);
  digitalWrite(_NW.b, HIGH);
}
void Motor::fNW(int speed) {
  analogWrite(_NW.pwm, abs(speed) - _NW.speedDeviation);
  if (speed >= 0) {
    digitalWrite(_NW.a, 1);
    digitalWrite(_NW.b, 0);
  } else {
    digitalWrite(_NW.a, 0);
    digitalWrite(_NW.b, 1);
  }
}

void Motor::fSE(int id, int speed) {
  analogWrite(_SE.pwm, speed - _SE.speedDeviation);
  if (id == 0) {
    digitalWrite(_SE.a, 1);
    digitalWrite(_SE.b, 0);
    return;
  } else if (id == 1) {
    digitalWrite(_SE.a, 0);
    digitalWrite(_SE.b, 1);
    return;
  }
  analogWrite(_SE.pwm, LOW);
  digitalWrite(_SE.a, HIGH);
  digitalWrite(_SE.b, HIGH);
}
void Motor::fSE(int speed) {
  analogWrite(_SE.pwm, abs(speed) - _SE.speedDeviation);
  if (speed >= 0) {
    digitalWrite(_SE.a, 1);
    digitalWrite(_SE.b, 0);
  } else {
    digitalWrite(_SE.a, 0);
    digitalWrite(_SE.b, 1);
  }
}

void Motor::fSW(int id, int speed) {
  analogWrite(_SW.pwm, speed - _SW.speedDeviation);
  if (id == 0) {
    digitalWrite(_SW.a, 1);
    digitalWrite(_SW.b, 0);
    return;
  } else if (id == 1) {
    digitalWrite(_SW.a, 0);
    digitalWrite(_SW.b, 1);
    return;
  }
  analogWrite(_SW.pwm, LOW);
  digitalWrite(_SW.a, HIGH);
  digitalWrite(_SW.b, HIGH);
}
void Motor::fSW(int speed) {
  analogWrite(_SW.pwm, abs(speed) - _SW.speedDeviation);
  if (speed >= 0) {
    digitalWrite(_SW.a, 1);
    digitalWrite(_SW.b, 0);
  } else {
    digitalWrite(_SW.a, 0);
    digitalWrite(_SW.b, 1);
  }
}
