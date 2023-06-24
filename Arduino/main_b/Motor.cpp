#include <Arduino.h>
#include "Motor.h"
#include "Pid.h"

#define COMPASS_DEVIATION 15

Motor::Motor() {
  compassP.kp = 1.8;
  compassP.ki = .1;
  compassP.kd = 1.3;
  compassP.minOutput = 30;
  compassP.maxOutput = 255;
  compassP.delay = 20;
  compassP.errorThreshold = 5;
  compassP.maxErrorSum = 180;

  stability.kp = 1.5;
  stability.ki = .1;
  stability.kd = 1;
  stability.minOutput = 0;
  stability.maxOutput = 30;
  stability.delay = 20;
  stability.errorThreshold = 3;
  stability.maxErrorSum = 180;
}

void Motor::attachCompass (Compass * c) {
  compass = c;
};

void Motor::begin (byte nwpwm, byte nwa, byte nwb, byte nepwm, byte nea, byte neb, byte swpwm, byte swa, byte swb, byte sepwm, byte sea, byte seb) {
  _NE = {nea, neb, nepwm, 0, nea, neb};
  _NW = {nwa, nwb, nwpwm, 0, nwa, nwb};
  _SE = {sea, seb, sepwm, 0, sea, seb};
  _SW = {swa, swb, swpwm, 0, swa, swb};

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
}

void Motor::active(bool state) {
  if (state) {
    if (_NE.a != 50) return;
    _NE.a = _NE.originalA;
    _NE.b = _NE.originalB;
    _NW.a = _NW.originalA;
    _NW.b = _NW.originalB;
    _SE.a = _SE.originalA;
    _SE.b = _SE.originalB;
    _SW.a = _SW.originalA;
    _SW.b = _SW.originalB;
    return;
  }
  if (_NE.a == 50) return;
  Stop();
  _NE.a = 50;
  _NE.b = 50;
  _NW.a = 50;
  _NW.b = 50;
  _SE.a = 50;
  _SE.b = 50;
  _SW.a = 50;
  _SW.b = 50;
}

void Motor::pidNorth(int initAngle, int speed) {
  float input = compass -> checkAngle();
  float error = initAngle + input;
  float output = pid.computePID(input, initAngle, error, &stability);
  if (output == 9999) return;
  Serial.print(" output: ");
  Serial.print(output);
  fNE(speed + output);
  fSE(speed + output);
  fNW(speed - output);
  fSW(speed - output);
}

void Motor::pidSouth(int initAngle, int speed) {
  speed *= -1;
  float input = compass -> checkAngle();
  float error = initAngle + input;
  float output = pid.computePID(input, initAngle, error, &stability);
  if (output == 9999) return;
  fNW(speed - output);
  fSW(speed - output);
  fNE(speed + output);
  fSE(speed + output);
}

void Motor::pidEast(int initAngle, int speed) {
  float input = compass -> checkAngle();
  float error = initAngle + input;
  float output = pid.computePID(input, initAngle, error, &stability);
  if (output == 9999) return;
  fNW(speed - output);
  fNE(-speed + output);
  fSW(-speed - output);
  fSE(speed + output);
}

void Motor::pidWest(int initAngle, int speed) {
  float input = compass -> checkAngle();
  float error = initAngle + input;
  float output = pid.computePID(input, initAngle, error, &stability);
  if (output == 9999) return;
  fNW(-speed - output);
  fNE(speed + output);
  fSW(speed - output);
  fSE(-speed + output);
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
  fNW(NW_speed);
  fSW(SW_speed);
  fSE(SE_speed);
  fNE(NE_speed);
}

void Motor::NorthWest(int speed) {
  fNE(0, speed);
  fSW(0, speed);
  fNW(0, 0);
  fSE(0, 0);
}

void Motor::NorthEast(int speed) {
  fNW(0, speed);
  fSE(0, speed);
  fNE(0, 0);
  fSW(0, 0);
}

void Motor::SouthEast(int speed) {
  fNE(1, speed);
  fSW(1, speed);
  fNW(0, 0);
  fSE(0, 0);
}

void Motor::SouthWest(int speed) {
  fNW(1, speed);
  fSE(1, speed);
  fNE(0, 0);
  fSW(0, 0);
}

void Motor::North(int speed) {
  fNW(0, speed);
  fNE(0, speed);
  fSW(0, speed);
  fSE(0, speed);
}

void Motor::South(int speed) {
  fNW(1, speed);
  fNE(1, speed);
  fSW(1, speed);
  fSE(1, speed);
}

void Motor::East(int speed) {
  fNW(0, speed);
  fNE(1, speed);
  fSW(1, speed);
  fSE(0, speed);
}

void Motor::West(int speed) {
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
  rotation = 0;
};

void Motor::TurnRight(int speed) {
  fNW(0, speed);
  fNE(1, speed);
  fSW(0, speed);
  fSE(1, speed);
  rotation = 1;
};

void Motor::rotateToAngle(float _initAngle, int _destiny, bool stop) {
  pid.clearPIDdata(&compassP);
  for (;;) {
    float error = (_destiny - _initAngle) * -1;
    int output = pid.computePID(_initAngle, _destiny, error, &compassP);
    if (output != 9999) {
      if (output == 0) {
        if (stop) Stop();
        return;
      };
      if (output < 0) TurnRight(abs(output));
      else TurnLeft(abs(output));
      _initAngle = compass -> checkAngle();
    }
  }
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
