#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_LSM303_Accel.h>
#include <math.h>

float MagMinX, MagMaxX;
float MagMinY, MagMaxY;
float MagMinZ, MagMaxZ;

long lastDisplayTime;

/*
 * LSM303AGR Compass Example
 * 
 * From what I can gather, this shows the proper way to get a compas heading from the LSM303.
 * It solves two problems which aren't addressed in the Adafruit example code:
 * 1.) Show how to incorporate min and max values from calibration. Makes a HUGE difference in my testing.
 * 2.) Take the z-axis into account.
 * 
 * Based heavily off Adafruit example code and https://github.com/pololu/lsm303-arduino.
 */


Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

template <typename T> struct vector
{
  T x, y, z;
};

// Stores min and max magnetometer values from calibration
vector<float> m_max;
vector<float> m_min;

/*
 * Returns the angular difference in the horizontal plane between the "from" vector and north, in degrees.
 * Description of heading algorithm:
 * Shift and scale the magnetic reading based on calibration data to find
 * the North vector. Use the acceleration readings to determine the Up
 * vector (gravity is measured as an upward acceleration). The cross
 * product of North and Up vectors is East. The vectors East and North
 * form a basis for the horizontal plane. The From vector is projected
 * into the horizontal plane and the angle between the projected vector
 * and horizontal north is returned.
 */
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

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Compass Test\n");

  // Values determined with calibration example and lots of LSM303 wiggling
  /*COMPASS 1
  -75.30  -82.95  -66.15
Mag Maximums: 12.45  14.25  36.30

COMPASS 2

Mag Minimums: -85.80  -36.00  -3.60
Mag Maximums: 25.65  65.25  108.75
*/
  //COMPASS 1
  /*m_min = (vector<float>){-75.30, -82.95, -66.15};
  m_max = (vector<float>){12.45, 14.25, 36.30};*/
  //COMPASS 2
  m_min = (vector<float>){-75.90, -40.95, -4.50};
  m_max = (vector<float>){24.30, 69.00, 109.95};
  //COMPASS 3 
  /*m_min = (vector<float>){-75.30, -82.95, -66.15};
  m_max = (vector<float>){12.45, 14.25, 36.30};*/
  
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
}


void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t magEvent;

  mag.getEvent(&magEvent);

  if (magEvent.magnetic.x < MagMinX) MagMinX = magEvent.magnetic.x;
  if (magEvent.magnetic.x > MagMaxX) MagMaxX = magEvent.magnetic.x;

  if (magEvent.magnetic.y < MagMinY) MagMinY = magEvent.magnetic.y;
  if (magEvent.magnetic.y > MagMaxY) MagMaxY = magEvent.magnetic.y;

  if (magEvent.magnetic.z < MagMinZ) MagMinZ = magEvent.magnetic.z;
  if (magEvent.magnetic.z > MagMaxZ) MagMaxZ = magEvent.magnetic.z;

  if ((millis() - lastDisplayTime) > 1000)  // display once/second
  {
    m_min = (vector<float>){-68.25,  -81.90,  -35.40};
  m_max = (vector<float>){21.30,  29.10,  52.95};
    Serial.print("m_min = (vector<float>){"); Serial.print(MagMinX); Serial.print(", ");Serial.print(MagMinY); Serial.print(", "); Serial.print(MagMinZ); Serial.println("};");
    Serial.print("m_max = (vector<float>){"); Serial.print(MagMaxX); Serial.print(", ");Serial.print(MagMaxY); Serial.print(", "); Serial.print(MagMaxZ); Serial.println("};"); Serial.println();
    lastDisplayTime = millis();
  }
}
