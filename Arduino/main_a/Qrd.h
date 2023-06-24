#include "Oled.h"

#ifndef QRD_H
#define QRD_H


/*
             ^ Frente del robot
      _________________
     /  ▥          ▥   \
    /   1           2    \
    \                    /
    / ▥3             4▥ \
    \____________________/
    
*/
class Qrd {
  private:
    struct SensorData {
      byte pin;
      int limit;
      int value;
      int vArraySize = 10;
      int vArray[10];
      int _1, _2, _3, _4, _5, _6, _7, _8, _9, _10; // Avoid interference with next structure
    };
    SensorData sensors[16];

    template <typename T>
    void format(T data, int spaces) {
      if(String(data).length() < spaces) printSpaces(spaces - String(data).length());
    };
    void printSpaces(int spaces) {
      for (int i = 0; i < spaces; i++) {
        Serial.print(" ");
      }
    }
  public:
    Qrd();
    
    void begin (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4);
    void reset ();
    void test ();

    SensorData getStruct(int index) { return sensors[index]; }

    bool North(bool log, Oled *oled);

    bool _N1(bool log);
    bool _N2(bool log);
    bool _N3(bool log);
    bool _N4(bool log);

    long countIndex = 0;
};

#endif
