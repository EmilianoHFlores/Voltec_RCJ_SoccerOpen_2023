#include "Strategy.h"
#include "Pid.h"

Strategy::Strategy () {
  center.kp = .8;
  center.ki = 0;
  center.kd = .4;
  center.minOutput = 10;
  center.maxOutput = 255;
  center.delay = 50;
  center.errorThreshold = 15;
  center.maxErrorSum = 160;

  deffense.kp = 1.8;
  deffense.ki = .12;
  deffense.kd = 1;
  deffense.minOutput = 20;
  deffense.maxOutput = 255;
  deffense.delay = 10;
  deffense.errorThreshold = 20;
  deffense.maxErrorSum = 160;

  score.kp = .5;
  score.ki = 0;
  score.kd = 0;
  score.minOutput = 0;
  score.maxOutput = 35;
  score.delay = 50;
  score.errorThreshold = 0;
  score.maxErrorSum = 160;
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

  if ( checkIntercept()
   || (abs(camera.ox() - camera.width / 2) <= 20 && compass.range(compass.checkAngle(), 0, 10))
   || attackAction == 1
  ) {
    buzzer.stop();
    attackAction = 1;
    if (!center_ball()) return;
    attackAction = 2;
    return;
  }
  digitalWrite(9, LOW);

  attackAction = 0;
  int direction = 0; // -1 = left, 0 = undefined, 1 = right
  int toAngle = 0;

  if (checkWeight() == 1) direction = -1;
  else if (checkWeight() == 0) direction = 1;
  else if (compass.checkAngle() > 0) direction = 1;
  else direction = -1;

  if (camera.ox() <= (160 - 70) || camera.ox() >= (160 + 50)) {
    buzzer.stop();
    center_ball();
    return;
  };

  if (camera.oy() <= 5) {
    motor.pidSouth(0, 130);
    buzzer.beep(255);
    return;
  }

  if (camera.oy() <= 5) toAngle = 90;
  else if (camera.oy() <= 15) toAngle = 45;
  else toAngle = 30;
  buzzer.beep(100);
  motor.moveToAngle(compass.checkAngle(), toAngle * direction, horizontalSpeed * floatMap(camera.oy(), 0, 30, .6, 1));
}

void Strategy::score_goal() {
  if (camera.oy() <= 0) {
    digitalWrite(9, HIGH);
  } else {
    digitalWrite(9, LOW);
  }

  int error = (camera.width / 2) - camera.ox();
  int output = pid.computePID(camera.ox(), camera.width / 2, error, &score);
  if (output == 9999) return;
  if (output == 0) return motor.pidNorth(0, scoreSpeed);
  output *= 4;
  Serial.print("+"); Serial.print(scoreSpeed + output); format<int>(scoreSpeed + output, 6);
  Serial.print("-"); Serial.print(scoreSpeed - output); format<int>(scoreSpeed - output, 6);
  motor.fNE(scoreSpeed + output);
  motor.fSE(scoreSpeed - output);
  motor.fNW(scoreSpeed + output);
  motor.fSW(scoreSpeed - output);

}

bool Strategy::center_ball() {
  int error = (camera.width / 2) - camera.ox();

  center.errorThreshold = 15 * floatMap(camera.oy(), 0, 30, 4, 1);
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
  attackAction = 0;
  pid.clearPIDdata(&center);
  pid.clearPIDdata(&deffense);
  motor.Stop();
}

// void Strategy::deffend() {
//   if (locate_behind()) motor.pidNorth(0, 130);
// }
// bool Strategy::locate_behind() {
//   float angle = compass.checkAngle();
//   int error = (camera.width / 2) - camera.ox();

//   int output = pid.computePID(camera.ox(), camera.width / 2, error, &deffense);

//   if (output < 0) motor.pidEast(0, abs(output));
//   else motor.pidWest(0, abs(output));
//   Serial.println("PID successfully executed.");
// }
