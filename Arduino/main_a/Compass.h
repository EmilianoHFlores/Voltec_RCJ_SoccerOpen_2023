#include "Oled.h"

#ifndef COMPASS_H
#define COMPASS_H

class Compass {
  private:
    Oled compassOled;

    template <typename T> struct vector { T x, y, z; };

    vector<float> m_max;
    vector<float> m_min;

    template <typename T> float heading(vector<T> from);
    template <typename Ta, typename Tb, typename To> void vector_cross (const vector<Ta> *a, const vector<Tb> *b, vector<To> *out);
    template <typename Ta, typename Tb> float Compass::vector_dot(const vector<Ta> *a, const vector<Tb> *b);
    void vector_normalize(vector<float> *a);

    float getAverageHeading(int n);
    float heading(void);
    float angleCalibrated(float angle, float initial);
    float reverseAngle(float angle);
    float LSMtoNavx(float angle);
    float rawRequestNavx();

    String TYPE;
  public:
    Compass ();
    void init (String type);
    void reset();
    void test ();
    void begin (String type);
    
    float checkAngle ();
    bool range(int init, int destiny, int deviation);
    float angleTo180s(float angle);
    int to360(int angle);
    float add(float angle1, float angle2);
    float getAverageHeadingNavx(int n);

    float initialReading = 0;
};

#endif
