/*
  Este es un código de ejemplo de un robot atacante con baja precisión
  Esta versión del código fue utilizada durante el Torneo Regional de Robótica XII
*/

#include <LiquidCrystal_I2C.h>

#include "Motor.h"
#include "Utils.h"
#include "Camera.h"
#include "Compass.h"
#include "Ultrasonic.h"

#define maxSpeed 255
#define aproachSpeed 150
#define angleSpeed 75
#define rotationSpeed 50
#define positionSpeed 25

#define COMPASS_DEVIATION 15

Motor motor;
Utils utils;
Camera camera;
Compass compass;
Ultrasonic ultrasonic;
LiquidCrystal_I2C lcd (0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Martillo jala :)");
  
  motor.begin(7, 8, 10, 9, 12, 11, 3, 2);
  camera.begin(115200, 1);
  compass.begin("navx");
  ultrasonic.begin(49, 48, 51, 50, 53, 52);
  utils.begin(53, 320, 240);

  motor.attachUtils(53, 320, 240);
  motor.attachCompass("navx");

  motor.reset(5000);
};

int angle_route;
int ball_x;
int ball_angle;
int ball_location;
int ball_preciseLocation;

bool endLoop = false;
bool next = false;
float ballDegrees = -1;

void loop () {
  lcd.setCursor(0, 1);
  lcd.print("DIOS PLAN            ");


  ball_x = camera.callOrange();
  if (ball_x > 0) motor.North(200, 100);
  else motor.TurnRight(100);

  endLoop = false;
  ball_x = camera.callOrange();
    if (ball_x > 0) {
    while (!endLoop) {
      lcd.setCursor(0, 1);
      lcd.print("Locate        ");
      ball_x = camera.callOrange();
      Serial.println(utils.checkPreciseDirection(ball_x, 7));
      switch (utils.checkPreciseDirection(ball_x, 7)) {
        case 0:
          motor.TurnRight(rotationSpeed);
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
          next = true;
          endLoop = true;
          ball_angle = compass.checkAngle();
          if (ball_angle > 120 && ball_angle < -120) {
            motor.rotateToAngle0(ball_angle, angleSpeed, true);
            delay(250);
            motor.South(maxSpeed, 500);
          }
          break;
        case 6:
          motor.TurnLeft(rotationSpeed);
          break;
        case -1:
          next = false;
          endLoop = true;
          break;
      }
    }

    if (next) {
      Serial.println("Colocar el robot en frente de la pelota");
      lcd.setCursor(0, 1);
      lcd.print("Place           ");
      next = false;
      endLoop = false;
      motor.moveToAngle(ball_angle > 0 ? 60 : -60, aproachSpeed);
      while (!endLoop) {
        ball_x = camera.callOrange();
        switch (utils.checkPreciseDirection(ball_x, 7)) {
          case 2:
          case 3:
          case 4:
              next = true;
              endLoop = true;
              motor.reset();
            break;
            break;
          case -1:
            endLoop = true;
            break;
        }
      }
    }

    if (next) { // Girar el robot y meter gol
      lcd.setCursor(0, 1);
      lcd.print("rot and score   ");
      Serial.println("Girar el robot y meter gol");
      next = false;
      endLoop = false;
      motor.rotateToAngle0(compass.checkAngle(), angleSpeed, true);
      motor.North(maxSpeed);
      while (!endLoop) {
        ball_x = camera.callOrange();
        if (ball_x == -1) {
          endLoop = true;
          next = true;
          delay(500);
          motor.reset();
        }
      }
    }

    if (next) {
      next = false;
      endLoop = false;
      motor.rotateToAngle0(compass.checkAngle(), angleSpeed, true);
      motor.South(maxSpeed, 500);
    }

  angle_route = -1;
  ball_x = -1;
  ball_angle = -1;
  ball_location = -1;
  ball_preciseLocation = -1;

  } else {
    motor.TurnRight(rotationSpeed);
  }
}
