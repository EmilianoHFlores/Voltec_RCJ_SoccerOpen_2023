#include <Arduino.h>
#include <Wire.h>
#include "Compass.h"
#include "Oled.h"
#include "AHRSProtocol.h"

Oled compassOled;

#define ITERATION_DELAY_MS 10
#define NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT 0x32
#define NUM_BYTES_TO_READ 8
int register_address = NAVX_REG_YAW_L;
byte data[512];

Compass::Compass () {}

void Compass::begin() {
  init ();
}

void Compass::init() {
  compassOled.begin();

  Wire.begin();
  for (int i = 0; i < sizeof(data); i++) {
    data[i] = 0;
  }

}

void Compass::reset() {
  Serial.println("Pending...");
}

void Compass::test() {
  Serial.println("Compass.h: I'm alive");
}

float Compass::checkAngle() {
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
