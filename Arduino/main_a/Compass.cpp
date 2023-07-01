#include <Arduino.h>
#include <Wire.h>
#include "Compass.h"
#include "AHRSProtocol.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_LSM303_Accel.h>
#include <math.h>

#define ITERATION_DELAY_MS 10
#define NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT 0x32
#define NUM_BYTES_TO_READ 8
int register_address = NAVX_REG_YAW_L;
byte data[512];

Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

template<typename T> float Compass::heading(vector<T> from) {
  sensors_event_t event;
  mag.getEvent(&event);
  vector<float> temp_m = {event.magnetic.x, event.magnetic.y, event.magnetic.z};

  accel.getEvent(&event);
  vector<float> a = {event.acceleration.x, event.acceleration.y, event.acceleration.z};
  
  // Important: subtract average of min and max from magnetometer calibration
  temp_m.x -= (m_min.x + m_max.x) / 2;
  temp_m.y -= (m_min.y + m_max.y) / 2;
  temp_m.z -= (m_min.z + m_max.z) / 2;

  // Compute east and north vectors
  vector<float> east;
  vector<float> north;
  vector_cross(&temp_m, &a, &east);
  vector_normalize(&east);
  vector_cross(&a, &east, &north);
  vector_normalize(&north);

  // compute heading
  float heading = atan2(vector_dot(&east, &from), vector_dot(&north, &from)) * 180 / PI;
  if (heading < 0) {
    heading += 360;
  }
  return heading;
}

template<typename Ta, typename Tb, typename To> void Compass::vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out) {
  out->x = (a->y * b->z) - (a->z * b->y);
  out->y = (a->z * b->x) - (a->x * b->z);
  out->z = (a->x * b->y) - (a->y * b->x);
}

template<typename Ta, typename Tb> float Compass::vector_dot(const vector<Ta> *a, const vector<Tb> *b) {
  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

void Compass::vector_normalize(vector<float> *a) {
  float mag = sqrt(vector_dot(a, a));
  a->x /= mag;
  a->y /= mag;
  a->z /= mag;
}

Compass::Compass() {
  this->TYPE = "";
}

void Compass::begin(String type) {
  init(type);
}

void Compass::init(String type) {
  TYPE = type;
  
  if (TYPE == "adafruit") {
m_min = (vector<float>){0.00, -1.20, -45.15};
m_max = (vector<float>){108.30, 81.45, 32.25};

    //FOR LSM
    if (!mag.begin()) {
      Serial.println("Unable to initialize LSM303 magnetometer");
      Serial.println("Trying again...");
      init (type);
    }

    if (!accel.begin()) {
      Serial.println("Unable to initialize LSM303 accelerometer");
      Serial.println("Trying again...");
      init (type);
    }

    accel.setRange(LSM303_RANGE_4G);
    accel.setMode(LSM303_MODE_NORMAL);

    // get average of 50 readings to get initial reading
    float readQty = 50;
    initialReading = getAverageHeading(readQty);
  } else if (TYPE == "navx") {
    Wire.begin();
    for (int i = 0; i < sizeof(data); i++) {
      data[i] = 0;
    }
    initialReading = getAverageHeadingNavx(50);
  } else {
    Serial.print("Unknown compass type");
    while (1)
      ;
  }
}

void Compass::reset() {
  Serial.println("Pending...");
}

void Compass::test() {
  Serial.println("Compass.h: I'm alive");
}

float Compass::checkAngle() {
  if (TYPE == "adafruit") {
    float lsmAngle = heading();
    if (isnan(lsmAngle)) {
      mag.begin();
    }
    return LSMtoNavx(lsmAngle);
  } else if (TYPE == "navx") {
    float navxLsmLikeAngle = rawRequestNavx();
    return LSMtoNavx(navxLsmLikeAngle);
  }
  return 0.00;
}

float Compass::heading(void) {
  return heading((vector<int>) {1, 0, 0});
}

float Compass::getAverageHeading(int n) {
  float counter = 0;  
  for (int i=0; i<n; i++){
    counter += heading();
  }
  return counter / n  ;
}

float Compass::getAverageHeadingNavx(int n) {
  float counter = 0;
  for (int i = 0; i < n; i++) {
    counter += rawRequestNavx();
  }
  return counter / n;
}

float Compass::angleCalibrated(float angle, float initial) {
  if (initial<=angle){
    return angle - initial;    
  }
  if (initial>=angle){
    return 360 - (initial - angle);
  }
}

float Compass::angleTo180s(float angle) {
  if (angle>180){
    return -(360-angle);
  }    
  else{
    return angle;
  }
}

float Compass::reverseAngle(float angle) {
  return -angle;
}

float Compass::LSMtoNavx(float angle) {
  angle = angleCalibrated(angle, initialReading);
  angle = angleTo180s(angle);
  angle = reverseAngle(angle);
  return angle;
}

bool Compass::range(int _i, int _d, int deviation) {
  int d = 0;
  int d_min = d - deviation;
  int d_max = d + deviation;
  float i = _d - _i;
  if ((d_min < i) && (i < d_max)) return true;
  return false;
}

int Compass::to360(int angle) {
  return angle > 0 ? angle : 360 + angle;
}

float Compass::rawRequestNavx() {
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
