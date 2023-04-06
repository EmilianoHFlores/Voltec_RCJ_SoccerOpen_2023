#include <Arduino.h>
#include <Wire.h>
#include "Compass.h"
#include "AHRSProtocol.h"

#define ITERATION_DELAY_MS 10
#define NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT 0x32
#define NUM_BYTES_TO_READ 8
int register_address = NAVX_REG_YAW_L;
byte data[512];

Compass::Compass () {
  this -> type = type;
}

void Compass::begin(String typ) {
  init (typ);
}

void Compass::init(String typ) {
  type = typ;

  if (type == "navx") initNavx();
  else if (type == "adafruit") initAdaf();
  else { Serial.println("Unknown compass type"); }
}

void Compass::reset() {
  Serial.println("Pending...");
}

void Compass::test() {
  Serial.println("Compass.h: I'm alive");
}

void Compass::initNavx () {
  Wire.begin();
  for (int i = 0; i < sizeof(data); i++) {
    data[i] = 0;
  }
}

void Compass::initAdaf () {
  Serial.println("Not ready...");
}

double Compass::checkAngle() {
  if (type == "navx") return checkAngleNavx();
  if (type == "adafruit") return checkAngleAdaf();
  Serial.println("Unknwon compass type");
  return 0.00;
}

double Compass::checkAngleNavx() {
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

double Compass::checkAngleAdaf() {
  Serial.println("Adafruit compass not ready");
  return 0.00;
}