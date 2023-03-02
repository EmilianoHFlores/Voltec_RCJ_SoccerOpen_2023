// Comentarios no hechos

/*
 * -1 = Object not in vision camp
 * 0 = Object at left of the robot vision camp
 * 1 = Object at middle of the robot vision camp
 * 2 = Object at right of the robot vision camp
*/

// Arduino-Native modules
#include <Wire.h>

// Manually installed modules
#include <LiquidCrystal_I2C.h>
#include "AHRSProtocol.h"

// Global constants
#define NWb 2
#define NWa 3
#define SWa 5
#define SWb 4
#define SEa 7
#define SEb 6
#define NEb 8
#define NEa 9
#define buzzer 50
#define buzzerLevel 255
#define defaultDelay 25
#define ITERATION_DELAY_MS 10
#define NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT 0x32
#define NUM_BYTES_TO_READ 8
const int QQVGA[2] = { 160, 120 };
const int QVGA[2] = { 320, 240 };
const int VGA[2] = { 640, 480 };
const int HQQVGA[2] = { 120, 80 };
const int HQVGA[2] = { 240, 160 };
const int HVGA[2] = { 480, 320 };
const int *resolution = QVGA;
const int degreeDeviation = 10;
const bool _directionLog = false;
const bool _log = false;
const String ownTeam = "y";
const String enemyTeam = ownTeam == "y" ? "b" : "y";
// Global variables
int maxSpeed = 255;
int rotationSpeed = 100;
int positionSpeed = 25;
int oX, oY, yX, yY, bX, bY;
double oZ, yZ, bZ;
int lastTimeSeen;
int register_address = NAVX_REG_YAW_L;
String oId, yId, bId;
byte data[512];

// Initialize objects

// LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(9600);
  println("Serial communication started");

  Serial1.begin(115200);
  Serial1.setTimeout(1);
  println("Serial1 communication started");

  Wire.begin();
  for (int i = 0; i < sizeof(data); i++) {
    data[i] = 0;
  }

  // lcd.begin(16, 2);
  // lcd.backlight();
  // println("LCD screen added");

  pinMode(NEa, OUTPUT);
  pinMode(NEb, OUTPUT);
  pinMode(SWa, OUTPUT);
  pinMode(SWb, OUTPUT);
  pinMode(SEa, OUTPUT);
  pinMode(SEb, OUTPUT);
  pinMode(NWa, OUTPUT);
  pinMode(NWb, OUTPUT);
  println("Motors added");
  stop(5000, true);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  println("Buzzer Added");
}

// Local loop function variables
bool endWhileLoop1 = false;
bool endWhileLoop2 = false;
bool endWhileLoop3 = false;
float ballDegrees = 420;

void loop() {
  endWhileLoop1 = false;
  endWhileLoop2 = false;
  endWhileLoop3 = false;
  callOrange();
  if (checkBallDirection() < 0) {
    TurnRight(maxSpeed);
  } else {
    if (ballDegrees == 420) {
      do {
        callOrange();
        int preciseLocation = checkPreciseBallDirection(7);
        switch (preciseLocation) {
          case -1:
            endWhileLoop1 = true;
            break;
          case 0:
          case 1:
            TurnLeft(rotationSpeed);
            break;
          case 2:
          case 3:
          case 4:
            ballDegrees = to360angle(checkDegrees());
            break;
          case 5:
          case 6:
            TurnRight(rotationSpeed);
            break;
        }
      } while (!endWhileLoop1);
    } else {
      rotateToAngle(to360angle(checkDegrees()), 0);
      int diferencia = (ballDegrees <= 180 ? 60 : -60);
      int total = int(ballDegrees) + diferencia;
      oX = -1;
      oY = -1;
      oZ = -1;      
      do {
        moveToAngle(total, maxSpeed);
        callOrange();
        if (oX > 1) {
          endWhileLoop2 = true;
        }
      } while (!endWhileLoop2);
      do {
        Serial.println(ballDegrees);
        callOrange();
        int preciseLocation = checkPreciseBallDirection(7);
        switch (preciseLocation) {
          case -1: 
            endWhileLoop3 = true;
            break;
          case 0:
          case 1:
            TurnLeft(positionSpeed);
            break;
          case 2:
          case 3:,
          case 4:
            North(maxSpeed, 1000);
            break;
          case 5:
          case 6:
            TurnRight(positionSpeed);
            break;
        }
      } while (!endWhileLoop3);

      ballDegrees = 420;
    }
  }
}

void rotateToAngle(float actualAngle, float destineAngle) {
  while (actualAngle <= destineAngle - degreeDeviation) {
    TurnRight(rotationSpeed);
    actualAngle = checkDegrees();
  }
  while (actualAngle >= destineAngle + degreeDeviation) {
    TurnLeft(rotationSpeed);
    actualAngle = checkDegrees();
  }
  stop();
}

float to360angle(float actualAngle) {
  return actualAngle > 0 ? actualAngle : 360 + actualAngle;
}

/*
 * @returns {float} Robot will return the angle the robot is looking to, from -179.99 to +179.99
 * @example
 * float degree = checkDegrees();
 * rotateToAngle(degree + 90)
 * // Robot will move 90 degrees right from its original position
*/
float checkDegrees() {
  int i;
  Wire.beginTransmission(NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT);  // Begin transmitting to navX-Sensor
  Wire.write(register_address);                                 // Sends starting register address
  Wire.write(NUM_BYTES_TO_READ);                                // Send number of bytes to read
  Wire.endTransmission();                                       // Stop transmitting

  /* Receive the echoed value back */
  Wire.beginTransmission(NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT);               // Begin transmitting to navX-Sensor
  Wire.requestFrom(NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT, NUM_BYTES_TO_READ);  // Send number of bytes to read
  delay(1);
  while (Wire.available()) {  // Read data (slave may send less than requested)
    data[i++] = Wire.read();
  }
  Wire.endTransmission();  // Stop transmitting
  return IMURegisters::decodeProtocolSignedHundredthsFloat((char *)&data[0]);
}

/*
 * @param {String} type The prefix of the variable, oX, yX, bX, etc.
 * @returns {void} Prints to the serial monitor all variables
 * @example
 * callOrange();
 * logCoordinatesVariables("o");
 * // This function will call the camera for new data and print the values to the Serial Monitor.
*/
void logCoordinatesVariables(String type) {
  println("Starting Log Coordinates Variables function...");
  if (type == "o") {
    Serial.print("oX: ");
    Serial.print(oX);
    Serial.print(", oY: ");
    Serial.print(oY);
    Serial.print(", oZ: ");
    Serial.print(oZ);
    Serial.print(", oId: ");
    Serial.println(oId);
  } else if (type == "b") {
    Serial.print("bX: ");
    Serial.print(bX);
    Serial.print(", bY: ");
    Serial.print(bY);
    Serial.print(", bZ: ");
    Serial.print(bZ);
    Serial.print(", bId: ");
    Serial.println(bId);
  } else if (type == "y") {
    Serial.print("yX: ");
    Serial.print(yX);
    Serial.print(", yY: ");
    Serial.print(yY);
    Serial.print(", yZ: ");
    Serial.print(yZ);
    Serial.print(", yId: ");
    Serial.println(yId);
  }
  println("Log Coordinates Variables function excecuted successfully...");
}
/*
 * @returns {void} Calls the camera for Orange and then excecutes the substring function
 * @example
 * callOrange();
 * // oX, oY, oZ and oId variables will be updated
*/
void callOrange() {
  println("Starting Call Orange function...");
  Serial1.write("o");
  for (int i = 0; i < 10; i++) {
    println("Esperando...");
    if (Serial1.available()) break;
    delay(1);
  }

  if (Serial1.available()) {
    substringData(Serial1.readString());
  }
  println("Call Orange function excecuted successfully");
}

/*
 * @returns {void} Calls the camera for Yellow and then excecutes the substring function
 * @example
 * callYellowGoal();
 * // yX, yY, yZ and yId variables will be updated
*/
void callYellowGoal() {
  println("Starting Call Yellow Goal function...");
  Serial1.write("y");

  for (int i = 0; i < 10; i++) {
    println("Esperando...");
    if (Serial1.available()) break;
    delay(1);
  }

  if (Serial1.available()) {
    substringData(Serial1.readString());
  }
  println("Call Yellow Goal function excecuted successfully");
}

/*
 * @returns {void} Calls the camera for Orange and then excecutes the substring function
 * @example
 * callBlueGoal();
 * // bX, bY, bZ and bId variables will be updated
*/
void callBlueGoal() {
  println("Starting Call Blue Goal function...");
  Serial1.write("b");
  for (int i = 0; i < 10; i++) {
    println("Esperando...");
    if (Serial1.available()) break;
    delay(1);
  }

  if (Serial1.available()) {
    substringData(Serial1.readString());
  }
  println("Call Blue Goal function excecuted successfully");
}

/*
 * @param {String} type The ID of the goal to follow, ("y" for yellow, "b" for blue)
 * @returns {void} Function will change xX, xY, xZ, and xId variables and move the robot to type goal 
 * @example
 * followGoal("y");
 * // Robot will find the path up to the Y goal
*/
void followGoal(String type) {
  println("Starting Follow Goal function...");
  int typeInt = 0;
  if (type == "b") typeInt = 1;
  switch (typeInt) {
    case 0:
      if (yX >= 0) {
        if (yX <= (resolution[0] / 3)) NorthWest(maxSpeed);
        else if (yX > (resolution[0] / 3) && yX < ((resolution[0] / 3) * 2)) North(maxSpeed);
        else if (yX >= ((resolution[0] / 3) * 2)) NorthEast(maxSpeed);
      } else TurnRight(rotationSpeed);
      break;
    case 1:
      if (bX >= 0) {
        if (bX <= (resolution[0] / 3)) NorthWest(maxSpeed);
        else if (bX > (resolution[0] / 3) && bX < ((resolution[0] / 3) * 2)) North(maxSpeed);
        else if (bX >= ((resolution[0] / 3) * 2)) NorthEast(maxSpeed);
      } else TurnRight(rotationSpeed);
      break;
    default:
      println("Unrecognized goal type");
      break;
  }
  println("Follow Goal function excecuted successfully");
}

/*
 * @returns {void} Function will change oX, oY, oZ, and oId variables and move the robot to the ball
 * @example
 * followBall();
 * // Robot will find the path up to the ball
*/
void followBall() {
  println("Starting Follow Ball function...");
  if (oX >= 0) {
    if (oX <= (resolution[0] / 3)) {
      NorthWest(maxSpeed);
      lastTimeSeen = 0;
    } else if (oX > (resolution[0] / 3) && oX < ((resolution[0] / 3) * 2)) {
      North(maxSpeed);
      lastTimeSeen = 1;
    } else if (oX >= ((resolution[0] / 3) * 2)) {
      NorthEast(maxSpeed);
      lastTimeSeen = 2;
    }
  } else {
    if (lastTimeSeen == 0) TurnLeft(rotationSpeed);
    else if (lastTimeSeen == 2) TurnRight(rotationSpeed);
    else TurnRight(rotationSpeed);
  }
  println("Follow Ball function excecuted successfully");
}

/*
 * Function will work based on oX, oY, oZ and oId values
 * A call to the camera must be perfomemd before executing this function
 * @returns {Int}
 * -1 When the ball is not in vision range of the robot
 * 0 When the ball is at the left of the robot
 * 1 When the ball is in the middle of the robot
 * 2 When the ball is in the right of the robot
 * @example
 * int location = checkBallDirection();
 * // Robot will save in 'location' variable where the ball is
*/
int checkBallDirection() {
  println("Starting Check Ball Direction function...");
  if (oX >= 0) {
    println("Check Ball Direction function excecuted successfully");
    if (oX <= (resolution[0] / 3)) return 0;
    if (oX > (resolution[0] / 3) && oX < (resolution[0] / 3) * 2) return 1;
    if (oX >= ((resolution[0] / 3) * 2)) return 2;
  }
  println("Check Ball Direction function excecuted successfully");
  return -1;
}

/*
 * Function will work based on oX, oY, oZ and oId values
 * A call to the camera must be performed before executing this function
 * @param {Int} splits The amount of equally-sized sections to the ball be found in
 * @returns {Int} The function will return where is the ball located in one section of the amount specified
*/
int checkPreciseBallDirection(int splits) {
  println("Starting Check Ball Direction function...");
  if (oX >= 0) {
    println("Check Ball Direction function excecuted successfully");
    if (oX <= (resolution[0] / splits)) return 0;
    for (int i = 1; i < splits; i++) {
      if (oX > ((resolution[0] / splits) * i) && oX < ((resolution[0] / splits) * (i + 1))) return i - 1;
    }
    return splits - 1;
  }
  println("Check Ball Direction function excecuted successfully");
  return -1;
}


/*
 * Function will work based on xX, xY, xZ and xId values
 * A call to the camera must be perfomemd before executing this function
 * @param {String} type The goal type to detect, "y" or "b"
 * @returns
 * -1 When the goal is not in vision range of the robot
 * 0 When the goal is at the left of the robot
 * 1 When the goal is in the middle of the robot
 * 2 When the goal is in the right of the robot
 * @example
 * int location = checkGoalDirection();
 * // Robot will save in 'location' variable where the ball is
*/
int checkGoalDirection(String type) {
  println("Starting Check Goal Direction function...");
  int typeInt = 0;
  if (type == "b") typeInt = 1;
  switch (typeInt) {
    case 0:
      println("Check Goal Direction function excecuted successfully");
      if (yX >= 0) {
        if (yX <= (resolution[0] / 3)) return 0;
        else if (yX > (resolution[0] / 3) && yX < ((resolution[0] / 3) * 2)) return 1;
        else if (yX >= ((resolution[0] / 3) * 2)) return 2;
      }
      return -1;
      break;
    case 1:
      println("Check Goal Direction function excecuted successfully");
      if (bX >= 0) {
        if (bX <= (resolution[0] / 3)) return 0;
        else if (bX > (resolution[0] / 3) && bX < (resolution[0] / 3) * 2) return 1;
        else if (bX >= ((resolution[0] / 3) * 2)) return 2;
      }
      return -1;
      break;
    default:
      println("Unspecified goal type");
      break;
  }
}

/*
 * @param {String} data Data is raw information received from the camera.
 * @returns {void} Robot will change xX, xY, xZ and xId variables depending on the given data
 * @example
 * substringData("x:10,y:20,z:5,id:o")
*/
void substringData(String data) {
  println("Starting Substring Data function...");
  print("Data: ");
  println(data);
  int initIndex = data.indexOf(",");
  int middleIndex = data.indexOf(",", initIndex + 1);
  int lastIndex = data.lastIndexOf(",");
  int x = data.substring(2, initIndex).toInt();
  int y = data.substring(initIndex + 3, middleIndex).toInt();
  double z = data.substring(middleIndex + 3, lastIndex).toDouble();
  String id = data.substring(lastIndex + 4, data.length());
  if (id == "o") {
    oX = x;
    oY = y;
    oZ = z;
    oId = id;
  } else if (id == "y") {
    yX = x;
    yY = y;
    yZ = z;
    yId = id;
  } else if (id == "b") {
    bX = x;
    bY = y;
    bZ = z;
    bId = id;
  } else {
    print(id);
    print(" - ");
    println("Range Error: ID not recognized");
  }
  println("Substring Data function excecuted successfully");
}

/*
 * @param {Int} d is the amount of milliseconds of delay, from 0 to Infinity
 * @returns {void} Robot will alternate between turning on and off the buzzer but everytime faster
 * @example
 * println("Starting in 5 seconds...");
 * buzzerDelay(5000);
 * println("Started.")
*/
void buzzerDelay(int d) {
  println("Starting Buzzer Delay function...");
  const float max = (d / 3);
  const float mid = (d / 3);
  const float min = (d / 3);
  for (int i = 0; i <= (max / 200); i++) {
    analogWrite(buzzer, buzzerLevel);
    delay(100);
    analogWrite(buzzer, LOW);
    delay(100);
  }
  for (int i = 0; i < (mid / 100); i++) {
    analogWrite(buzzer, buzzerLevel);
    delay(50);
    analogWrite(buzzer, LOW);
    delay(50);
  }
  for (int i = 0; i < (mid / 50); i++) {
    analogWrite(buzzer, buzzerLevel);
    delay(25);
    analogWrite(buzzer, LOW);
    delay(25);
  }
  println("Buzzer Delay function completed successfully");
}

/*
 * @param {int, required} angle
 * angle is the angle the robot will move, from 0 to 359
 * @param {int, required} speed
 * the maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of time the robot will keep moving, from 0 to Infinity
 * @returns;
 * The robot will move to the specified angle at the specified speed
 * @example;
 * moveToAngle(90, 100);
 * // Robot will move to angle 90 at a maximum speed of 100
*/
void moveToAngle(int angle, int speed) {
  println("Starting Move to Angle function");
  int NW_speed = speed * cos((PI / 180) * (angle + 315));
  int SW_speed = speed * cos((PI / 180) * (angle + 225));
  int SE_speed = speed * cos((PI / 180) * (angle + 135));
  int NE_speed = speed * cos((PI / 180) * (angle + 45));
  int _largestNumber = largestNumber(abs(NW_speed), abs(SW_speed), abs(SE_speed), abs(NE_speed));
  float ratio = float(speed) / float(_largestNumber);
  NW_speed *= ratio;
  SW_speed *= -ratio;
  SE_speed *= -ratio;
  NE_speed *= ratio;
  _NW(NW_speed);
  _SW(SW_speed);
  _SE(SE_speed);
  _NE(NE_speed);
  println("Move to Angle function completed successfully");
}
void moveToAngle(int angle, int speed, int d) {
  println("Starting Move to Angle function");
  int NW_speed = speed * cos((PI / 180) * (angle + 315));
  int SW_speed = -speed * cos((PI / 180) * (angle + 225));
  int SE_speed = -speed * cos((PI / 180) * (angle + 135));
  int NE_speed = speed * cos((PI / 180) * (angle + 45));
  int _largestNumber = largestNumber(abs(NW_speed), abs(SW_speed), abs(SE_speed), abs(NE_speed));
  float ratio = float(speed) / float(_largestNumber);
  NW_speed *= ratio;
  SW_speed *= ratio;
  SE_speed *= ratio;
  NE_speed *= ratio;
  _NW(NW_speed);
  _SW(SW_speed);
  _SE(SE_speed);
  _NE(NE_speed);
  delay(d);
  println("Move to Angle function completed successfully");
}

/*
 * @param {int, required} number1
 * Integer number from -Infinity to Infinity
 * @param {int, required} number2
 * Integer number from -Infinity to Infinity
 * @param {int, required} number3 
 * Integer number from -Infinity to Infinity
 * @param {int, required} number4
 * Integer number from -Infinity to Infinity
 * @returns int
 * Robot will return the largest number from four of the specified numbers
 * @example
 * int largest = largestNumber(10, 15, 20, 25);
 * Serial.println(largest)
 * // Output: 25
*/
int largestNumber(int number1, int number2, int number3, int number4) {
  println("Starting LargestNumber function...");
  int numbers[] = { number1, number2, number3, number4 };
  int largest = numbers[0];
  for (int i = 1; i < 4; i++)
    if (numbers[i] > largest) largest = numbers[i];
  println("LargestNumber function completed successfully");
  return largest;
}

/*
 * @warning
 * _log must be true for the function to work
 * @param {String, int, required} action 
 * The action that will be printed to Serial Monitior
 * @param {bool} movement
 * If the action is part of motor-movement
 * @returns;
 * Action will be printed to Serial Monitor without a line jump
 * @example
 * print("Hello, World!", false);
 * // Output: Hello, World!
 * _directionLog = false
 * print("Going North-West", true)
 * // Output: 
 * _directionLog = true
 * print("Going North-West", true)
 * // Output: Going North-West
*/
void print(String action) {
  if (_log == true) {
    Serial.print(action);
  }
}
void print(String action, bool movement) {
  if (movement && _directionLog) {
    Serial.print(action);
  } else if (_log && !_directionLog) {
    Serial.print(action);
  } else if (!movement && !_directionLog) {
    Serial.print(action);
  }
}
void print(int action) {
  if (_log == true) {
    Serial.print(action);
  }
}
void print(int action, bool movement) {
  if (movement && _directionLog) {
    Serial.print(action);
  } else if (_log && !_directionLog) {
    Serial.print(action);
  } else if (!movement && !_directionLog) {
    Serial.print(action);
  }
}

/*
 * @warning
 * _log must be true for the function to work
 * @param {String, int, required} action
 * The action that will be printed to Serial Monitior
 * @param {bool} movement
 * If the action is part of motor-movement
 * @returns;
 * Action will be printed to Serial Monitor with a line jump
 * @example
 * print("Hello, World!", false);
 * // Output: Hello, World!
 * _directionLog = false
 * print("Going North-West", true)
 * // Output: 
 * _directionLog = true
 * print("Going North-West", true)
 * // Output: Going North-West
*/
void println(int action) {
  if (_log == true) {
    Serial.println(action);
    // lcd.clear();
    // lcd.print(action);
  }
}
void println(int action, bool movement) {
  if (movement && _directionLog) {
    Serial.println(action);
  } else if (_log && !_directionLog) {
    Serial.println(action);
  } else if (!movement && !_directionLog) {
    Serial.println(action);
  }
}
void println(String action) {
  if (_log == true) {
    Serial.println(action);
    // lcd.clear();
    // lcd.print(action);
  }
}
void println(String action, bool movement) {
  if (movement && _directionLog) {
    Serial.println(action);
  } else if (_log && !_directionLog) {
    Serial.println(action);
  } else if (!movement && !_directionLog) {
    Serial.println(action);
  }
}

/*
 * @param {int} d
 * the amount of time in milliseconds from 0 to Infinity the robot will remain stopped
 * @param {int} b
 * The amount of time in milliseconds from 0 to Infinity the robot will remain stopped but with buzzer activated
 * @returns;
 * Robot will fully stop motor-movement
 * @example
 * stop(1000, true);
 * // Robot will stop for 1 second with the buzzer active before continue with the code
*/
void stop() {
  println("Robot stopped");
  _NW(0, 0);
  _NE(0, 0);
  _SW(0, 0);
  _SE(0, 0);
}
void stop(int d) {
  println("Robot stopped", true);
  _NW(0, 0);
  _NE(0, 0);
  _SW(0, 0);
  _SE(0, 0);
  delay(d);
}
void stop(int d, bool b) {
  println("Robot stopped", true);
  _NW(0, 0);
  _NE(0, 0);
  _SW(0, 0);
  _SE(0, 0);
  if (b == true) buzzerDelay(d);
  else delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will turn on its own axis to the right
 * @example
 * TurnRight(255, 100);
 * // The robot will be moving to the Right at a speed of 255 during 100 milliseconds
*/
void TurnRight(int speed) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(0, speed);
  _SE(1, speed);
  println("Robot turning right", true);
}
void TurnRight(int speed, int d) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(0, speed);
  _SE(1, speed);
  println("Robot turning right", true);
  delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d 
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will turn on its own axis to the left
 * @example
 * TurnLeft(255, 100);
 * // The robot will be moving to the Left at a speed of 255 during 100 milliseconds
*/
void TurnLeft(int speed) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(1, speed);
  _SE(0, speed);
  println("Robot turning left", true);
}
void TurnLeft(int speed, int d) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(1, speed);
  _SE(0, speed);
  println("Robot turning left", true);
  delay(d);
}

void ownAxisRightRotationLeftDirection(int speed) {
  _NW(1, speed);
  _NE(0, 0);
  _SE(1, speed);
  _SW(1, speed);
}

void ownAxisLeftRotationLeftDirection(int speed) {
  _NW(1, speed);
  _NE(0, 0);
  _SE(1, speed);
  _SW(0, speed);
}

void ownAxisLeftRotationRightDirection(int speed) {
  _NW(0, speed);
  _NE(0, 0);
  _SE(0, speed);
  _SW(0, speed);
}

void ownAxisRightRotationRightDirection(int speed) {
  _NW(0, speed);
  _NE(0, 0);
  _SE(1, speed);
  _SW(0, speed);
}
/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity, optional
 * @returns;
 * The robot will go to the NorthWest-Robot POV direction
 * @example
 * NorthWest(255, 100);
 * // The robot will be moving to NorthWest at a speed of 255 during 100 milliseconds
*/
void NorthWest(int speed) {
  _NE(0, speed);
  _SW(0, speed);
  _NW(0, 0);
  _SE(0, 0);
  println("Robot going North-West", true);
}
void NorthWest(int speed, int d) {
  _NE(0, speed);
  _SW(0, speed);
  _NW(0, 0);
  _SE(0, 0);
  println("Robot going North-West", true);
  delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will go to the NorthEast-Robot POV direction
 * @example
 * NorthEast(255, 100);
 * // The robot will be moving to NorthEast at a speed of 255 during 100 milliseconds
*/
void NorthEast(int speed) {
  _NW(0, speed);
  _SE(0, speed);
  _NE(0, 0);
  _SW(0, 0);
  println("Robot going North-East", true);
}
void NorthEast(int speed, int d) {
  _NW(0, speed);
  _SE(0, speed);
  _NE(0, 0);
  _SW(0, 0);
  println("Robot going North-East", true);
  delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will go to the SouthEast-Robot POV direction
 * @example
 * SouthEast(255, 100);
 * // The robot will be moving to SouthEast at a speed of 255 during 100 milliseconds
*/
void SouthEast(int speed) {
  _NE(1, speed);
  _SW(1, speed);
  _NW(0, 0);
  _SE(0, 0);
  println("Robot going South-East", true);
}
void SouthEast(int speed, int d) {
  _NE(1, speed);
  _SW(1, speed);
  _NW(0, 0);
  _SE(0, 0);
  println("Robot going South-East", true);
  delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will go to the SouthWest-Robot POV direction
 * @example
 * SouthWest(255, 100);
 * // The robot will be moving to SouthWest at a speed of 255 during 100 milliseconds
*/
void SouthWest(int speed) {
  _NW(1, speed);
  _SE(1, speed);
  _NE(0, 0);
  _SW(0, 0);
  println("Robot going South-West", true);
}
void SouthWest(int speed, int d) {
  _NW(1, speed);
  _SE(1, speed);
  _NE(0, 0);
  _SW(0, 0);
  println("Robot going South-West", true);
  delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will go to the North-Robot POV direction
 * @example
 * North(255, 100);
 * // The robot will be moving to North at a speed of 255 during 100 milliseconds
*/
void North(int speed) {
  _NW(0, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(0, speed);
  println("Robot going North", true);
}
void North(int speed, int d) {
  _NW(0, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(0, speed);
  println("Robot going North", true);
  delay(d);
}

/*
 * @param {int, required}
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will go to the South-Robot POV direction
 * @example
 * South(255, 100);
 * // The robot will be moving to South at a speed of 255 during 100 milliseconds
*/
void South(int speed) {
  _NW(1, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(1, speed);
  println("Robot going South", true);
}
void South(int speed, int d) {
  _NW(1, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(1, speed);
  println("Robot going South", true);
  delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d 
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will go to the East-Robot POV direction
 * @example
 * East(255, 100);
 * // The robot will be moving to East at a speed of 255 during 100 milliseconds
*/
void East(int speed) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(0, speed);
  println("Robot going East", true);
}
void East(int speed, int d) {
  _NW(0, speed);
  _NE(1, speed);
  _SW(1, speed);
  _SE(0, speed);
  println("Robot going East", true);
  delay(d);
}

/*
 * @param {int, required} speed
 * The maximum speed the robot will move, from 0 to 255
 * @param {int} d
 * The amount of delay in milliseconds the robot will be moving, from 0 to Infinity
 * @returns;
 * The robot will go to the West-Robot POV direction
 * @example
 * West(255, 100);
 * // The robot will be moving to West at a speed of 255 during 100 milliseconds
*/
void West(int speed) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(1, speed);
  println("Robot going West", true);
}
void West(int speed, int d) {
  _NW(1, speed);
  _NE(0, speed);
  _SW(0, speed);
  _SE(1, speed);
  println("Robot going West", true);
  delay(d);
}

/*
 * @private
 * @warning
 * This function may not be used in the void loop function
 * @param {int} id
 * The direction the North-East wheel will move, if ID is equal to 0, then it will move towards the north, if ID is equal to 0, then it will move towards the south
 * @param {int, required} speed
 * The maximum speed the wheel will be moving.
 * If ID is provided, from 0 to 255
 * If ID is not provided, from -255 to 255
 * When speed is negative, the wheel will move towards south
 * When speed is positive, the wheel will move towards north
 * When speed is 0, the wheel will not move.
 * @returns;
 * The wheel will move to the specified speed
 * @example
 * _NE(0, 255);
 * // The North-East wheel will move towards North at a maximum speed of 255
 * _NE(-100);
 * // The North-East wheel will move towards South at a maximum speed of 100
*/
void _NE(int id, int speed) {
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
  Serial.println("Range Error: ID is expected to be between 0 and 1");
}
void _NE(int speed) {
  if (speed >= 0) {
    analogWrite(NEa, speed);
    analogWrite(NEb, 0);
  } else {
    analogWrite(NEb, abs(speed));
    analogWrite(NEa, 0);
  }
}

/*
 * @private
 * @warning
 * This function may not be used in the void loop function
 * @param {int} id
 * The direction the North-West wheel will move, if ID is equal to 0, then it will move towards the north, if ID is equal to 0, then it will move towards the south, optional
 * @param {int, required} speed
 * The maximum speed the wheel will be moving.
 * If ID is provided, from 0 to 255
 * If ID is not provided, from -255 to 255
 * When speed is negative, the wheel will move towards south
 * When speed is positive, the wheel will move towards north
 * When speed is 0, the wheel will not move.
 * @returns;
 * The wheel will move to the specified speed
 * @example
 * _NW(0, 255);
 * // The North-West wheel will move towards North at a maximum speed of 255
 * _NW(-100);
 * // The North-West wheel will move towards South at a maximum speed of 100
*/
void _NW(int speed) {
  if (speed >= 0) {
    analogWrite(NWa, speed);
    analogWrite(NWb, 0);
  } else {
    analogWrite(NWb, abs(speed));
    analogWrite(NWa, 0);
  }
}
void _NW(int id, int speed) {
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
  Serial.println("Range Error: ID is expected to be between 0 and 1");
}

/*
 * @private
 * @warning
 * This function may not be used in the void loop function
 * @param {int} id
 * The direction the South-East wheel will move, if ID is equal to 0, then it will move towards the north, if ID is equal to 0, then it will move towards the south, optional
 * @param {int, required} speed
 * The maximum speed the wheel will be moving.
 * If ID is provided, from 0 to 255
 * If ID is not provided, from -255 to 255
 * When speed is negative, the wheel will move towards south
 * When speed is positive, the wheel will move towards north
 * When speed is 0, the wheel will not move.
 * @returns;
 * The wheel will move to the specified speed
 * @example
 * _SE(0, 255);
 * // The South-East wheel will move towards North at a maximum speed of 255
 * _NW(-100);
 * // The South-West wheel will move towards South at a maximum speed of 100
*/
void _SE(int speed) {
  if (speed >= 0) {
    analogWrite(SEa, speed);
    analogWrite(SEb, 0);
  } else {
    analogWrite(SEb, abs(speed));
    analogWrite(SEa, 0);
  }
}
void _SE(int id, int speed) {
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
  Serial.println("Range Error: ID is expected to be between 0 and 1");
}

/*
 * @private
 * @warning
 * This function may not be used in the void loop function
 * @param {int} id
 * The direction the South-West wheel will move, if ID is equal to 0, then it will move towards the north, if ID is equal to 0, then it will move towards the south, optional
 * @param {int, required} speed
 * The maximum speed the wheel will be moving.
 * If ID is provided, from 0 to 255
 * If ID is not provided, from -255 to 255
 * When speed is negative, the wheel will move towards south
 * When speed is positive, the wheel will move towards north
 * When speed is 0, the wheel will not move.
 * @returns;
 * The wheel will move to the specified speed
 * @example
 * _SW(0, 255);
 * // The South-West wheel will move towards North at a maximum speed of 255
 * _SW(-100);
 * // The South-West wheel will move towards South at a maximum speed of 100
*/
void _SW(int speed) {
  if (speed >= 0) {
    analogWrite(SWa, speed);
    analogWrite(SWb, 0);
  } else {
    analogWrite(SWb, abs(speed));
    analogWrite(SWa, 0);
  }
}
void _SW(int id, int speed) {
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
  Serial.println("Range Error: ID is expected to be between 0 and 1");
}

/**
GOAL WITH BALL IN FRONT OF THE ROBOT

  callOrange();
  int ballPos = checkBallDirection();
  if (ballPos == -1) {
    float coordenadas = checkDegrees();
    if (coordenadas > 0) {
      TurnLeft(rotationSpeed);
    } else {
      TurnRight(rotationSpeed);
    }
  }
  callBlueGoal();
  int goalPos = checkGoalDirection(enemyTeam);
  if (goalPos != -1 && ballPos != -1) {
    endLoop = false;
    do {
      callOrange();
      int preciseLocation = checkPreciseBallDirection(7);
      switch (preciseLocation) {
        case -1:
          endLoop = true;
          break;
        case 0:
        case 1:
          TurnLeft(positionSpeed);
          break;
        case 2:
        case 3:
        case 4:
          North(maxSpeed, 1000);
          break;
        case 5:
        case 6:
          TurnRight(positionSpeed);
          break;
      }
    } while (!endLoop);
  }
*/
