#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_LSM303_Accel.h>
#include <math.h>

//FOR LSM
Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

template <typename T> struct vector
{
  T x, y, z;
};

// Stores min and max magnetometer values from calibration
vector<float> m_max;
vector<float> m_min;


 //Returns the angular difference in the horizontal plane between the "from" vector and north, in degrees.
template <typename T> float heading(vector<T> from)
{
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

template <typename Ta, typename Tb, typename To> void vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out)
{
  out->x = (a->y * b->z) - (a->z * b->y);
  out->y = (a->z * b->x) - (a->x * b->z);
  out->z = (a->x * b->y) - (a->y * b->x);
}

template <typename Ta, typename Tb> float vector_dot(const vector<Ta> *a, const vector<Tb> *b)
{
  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

void vector_normalize(vector<float> *a)
{
  float mag = sqrt(vector_dot(a, a));
  a->x /= mag;
  a->y /= mag;
  a->z /= mag;
}

//FOR LSM
float initialReading = 0;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Compass Test\n");
  //Obtained from calibration
  m_min = (vector<float>){-36.30, -17.85, 0.00};
  m_max = (vector<float>){31.20, 43.95, 75.15};
  
  //FOR LSM
  if (!mag.begin())
  {
    Serial.println("Unable to initialize LSM303 magnetometer");
    while (1);
  }

  if (!accel.begin()) {
    Serial.println("Unable to initialize LSM303 accelerometer");
    while (1);
  }

  accel.setRange(LSM303_RANGE_4G);
  accel.setMode(LSM303_MODE_NORMAL);
  
  // get average of 50 readings to get initial reading
  float readQty = 50;
  Serial.println("Getting initial reading...");
  initialReading = getAverageHeading(readQty);
  Serial.print("Initial reading is "); Serial.println(initialReading);
  
}

void loop(void)
{
  //-----LSM ANGLE
  float lsmAngle = heading();
  if (isnan(lsmAngle)){
    mag.begin();
  }
  Serial.print("Raw Angle: ");
  Serial.print(lsmAngle);
  lsmAngle = LSMtoNavx(lsmAngle);  
  Serial.print("    NAVX-like Angle: ");
  Serial.println(lsmAngle);
}

/*
 * Returns the angular difference in the horizontal plane between a default vector and north, in degrees.
 * The default vector here is the +X axis as indicated by the silkscreen.
 */
float heading(void)
{
  return heading((vector<int>) {1, 0, 0});
}

float getAverageHeading(int n){
  float counter = 0;  
  for (int i=0; i<n; i++){
    counter += heading();
  }
  return counter / n  ;
}

float angleCalibrated(float angle, float initial){
  // gets the angle from 0-360, transforming so that if you are facing the same direction as when the program started, you get 0
  if (initial<=angle){
    return angle - initial;    
  }
  if (initial>=angle){
    return 360 - (initial - angle);
  }
}

float angleTo180s(float angle){
  if (angle>180){
    return -(360-angle);
  }    
  else{
    return angle;
  }
}

float reverseAngle(float angle){
  return -angle;
}

float LSMtoNavx(float angle){
  angle = angleCalibrated(angle, initialReading);
  angle = angleTo180s(angle);
  angle = reverseAngle(angle);
  return angle;
}