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
      int min;
      int max;
      int threshold;
    };

    SensorData sensors[16];
    // Sensors 0 to 3 -> North
    // Sensors 4 to 7 -> South
    // Sensors 8 to 11 -> East
    // Sensors 12 to 15 -> West

    Oled QRDoled;
  public:
    Qrd();
    
    void begin (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4);
    void init (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4);
    void reset ();
    void test ();
    void getThresholds ();
    SensorData getStruct(int index) { return sensors[index]; };

    int wardOff();

    int North() { return _N1(); }; //  || _N2() || _N3() || _N4(); 
    int South() { return 0; };
    int East() { return 0; };
    int West() { return 0; };

    int _N1();
    int _N2();
    int _N3();
    int _N4();
};

#endif
