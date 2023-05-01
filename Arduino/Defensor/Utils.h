#ifndef UTILS_H
#define UTILS_H

class Utils {
  private:
    byte buzzer;
    int width;
    int height;
  public:
    Utils ();
    void init (byte bzzr, int wdt, int hgt);
    void reset();
    void test ();
    void begin (byte bzzr, int wdt, int hgt);

    int largestNumber (int N1, int N2, int N3, int N4);
    void buzzerDelay (int d);
    int checkDirection (int x);
    int checkPreciseDirection (int x, int splits);
    float to360angle (float angle);
    float circle360 (float angle);
    bool inRange(int minValue, int maxValue, int value, bool orEqual);
    bool inRangeCompass(float _i, int _d, int deviation);
};

#endif
