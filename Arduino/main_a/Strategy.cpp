#include "Strategy.h"
#include "Pid.h"

Strategy::Strategy () {
  center.kp = .6;
  center.ki = .12;
  center.kd = .8;
  center.minOutput = 10;
  center.maxOutput = 255;
  center.delay = 10;
  center.errorThreshold = 15;
  center.maxErrorSum = 160;

  deffense.kp = 2.1;
  deffense.ki = .2;
  deffense.kd = 1.6;
  deffense.minOutput = 30;
  deffense.maxOutput = 255;
  deffense.delay = 10;
  deffense.errorThreshold = 15;
  deffense.maxErrorSum = 160;

  score.kp = .5;
  score.ki = 0;
  score.kd = 0;
  score.minOutput = 0;
  score.maxOutput = 35;
  score.delay = 50;
  score.errorThreshold = 0;
  score.maxErrorSum = 160;

  _errorThreshold = deffense.errorThreshold;
};

void Strategy::begin(String team) {
  TEAM = team;
  goalX = team == "blue" ? "yx" : "bx";
  goalY = team == "blue" ? "yy" : "by";
  goalIntercept = team == "blue" ? "yi" : "bi";
}

void Strategy::reset() {}

void Strategy::attack() {
  if (attackAction == 2) { score_goal(); return; }
  scoregoal_first = true;
  int direction = 0; // -1 = left, 0 = undefined, 1 = right
  int toAngle = 0;
  if (checkIntercept()) { attackAction = 2; return; }

  if (checkWeight() == 1) direction = -1;
  else if (checkWeight() == 0) direction = 1;
  else if (compass.checkAngle() > 0) direction = 1;
  else direction = -1;
  if (camera.ox() <= (160 - 50) || camera.ox() >= (160 + 50)) {
    center_ball();
    return;
  };

  if (camera.oy() <= 3) {
    motor.pidSouth(0, 130);
    return;
  }

  if (camera.oy() <= 5) toAngle = 90;
  else if (camera.oy() <= 15) toAngle = 45;
  else toAngle = 30;

  motor.moveToAngle(compass.checkAngle(), toAngle * direction, horizontalSpeed * floatMap(camera.oy(), 0, 30, .15, 1));
}

void Strategy::score_goal() {
  int error = (camera.width / 2) - camera.ox();
  int output = pid.computePID(camera.ox(), camera.width / 2, error, &score);
  if (output == 9999) return;
  if (output == 0) return motor.pidNorth(0, scoreSpeed);
  output *= -1;
  Serial.print("+"); Serial.print(scoreSpeed + output); format<int>(scoreSpeed + output, 6);
  Serial.print("-"); Serial.print(scoreSpeed - output); format<int>(scoreSpeed - output, 6);

  motor.fNE(scoreSpeed + output);
  motor.fSE(scoreSpeed + output);
  motor.fNW(scoreSpeed - output);
  motor.fSW(scoreSpeed - output);
}

void Strategy::goto_ball(int speed) {
  int error = (camera.width / 2) - camera.ox();
  int output = pid.computePID(camera.ox(), camera.width / 2, error, &score);
  if (output == 9999) return;
  if (output == 0) return motor.pidNorth(0, speed);
  output *= -1;
  Serial.print("+"); Serial.print(speed + output); format<int>(speed + output, 6);
  Serial.print("-"); Serial.print(speed - output); format<int>(speed - output, 6);

  motor.fNE(speed + output);
  motor.fSE(speed + output);
  motor.fNW(speed - output);
  motor.fSW(speed - output);
}

bool Strategy::center_ball() {
  int error = (camera.width / 2) - camera.ox();

  int output = pid.computePID(camera.ox(), camera.width / 2, error, &center);
  if (output == 9999) return false;
  if (output == 0) return true;

  if (output < 0) motor.TurnRight(abs(output));
  else motor.TurnLeft(abs(output));
  return false;
}

bool Strategy::checkIntercept() {
  return TEAM == "blue" ? camera.yi() : camera.bi();
}

int Strategy::checkWeight() {
  return TEAM == "blue" ? camera.yw() : camera.bw();
}

void Strategy::endAction () {
  xbee.Send(0);
  pid.clearPIDdata(&center);
  pid.clearPIDdata(&deffense);
}

void Strategy::deffend() {
  if (!compass.range(compass.checkAngle(), 0, 10)) motor.rotateToAngle(compass.checkAngle(), 0, true);
  if (locate_behind()) {
    goto_ball(scoreSpeed * floatMap(camera.oy(), 0, 30, 1, .65));
  }
}

bool Strategy::locate_behind() {
  float angle = compass.checkAngle();
  int error = (camera.width / 2) - camera.ox();

  if (camera.oy() <= 3) deffense.errorThreshold = 160;
  else deffense.errorThreshold = _errorThreshold;
  int output = pid.computePID(camera.ox(), camera.width / 2, error, &deffense);
  if (output == 9999) return false;
  if (output == 0) return true;
  if (output > 0) motor.pidEast(0, abs(output));
  else motor.pidWest(0, abs(output));
  return false;
}

bool Strategy::passiveDeffense() {
  if (!compass.range(compass.checkAngle(), 0, 8)) {
    motor.rotateToAngle(compass.checkAngle(), 0, true);
  }

  ultrasonic.South();
  ultrasonic.West();
  ultrasonic.East();

  Serial.print("South: "); Serial.print(ultrasonic.south); format<float>(ultrasonic.south, 8);
  Serial.print("West: "); Serial.print(ultrasonic.west); format<float>(ultrasonic.west, 8);
  Serial.print("East: "); Serial.print(ultrasonic.east); format<float>(ultrasonic.east, 8);
  Serial.print("Yx: "); Serial.print(pixyGoalX()); format<int>(pixyGoalX(), 8);
  Serial.print("Angle"); Serial.print(compass.checkAngle()); format<float>(compass.checkAngle(), 8);
  Serial.println();

  if (ultrasonic.west > 100) motor.pidWest(0, 130);
  else if (ultrasonic.west < 60) motor.pidEast(0, 130);
  else if (ultrasonic.south > 35) motor.pidSouth(0, 130);
  else if (ultrasonic.south < 25) motor.pidNorth(0, 130);
  else if (ultrasonic.west > 80) motor.pidWest(0, 130);
  else if (ultrasonic.west < 80) motor.pidEast(0, 130);
}