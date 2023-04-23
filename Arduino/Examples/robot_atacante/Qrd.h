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
    byte N1;
    byte N2;
    byte N3;
    byte N4;

    byte S1;
    byte S2;
    byte S3;
    byte S4;

    byte E1;
    byte E2;
    byte E3;
    byte E4;

    byte W1;
    byte W2;
    byte W3;
    byte W4;
  public:
    Qrd();
    void begin (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4);
    void init (byte n1, byte n2, byte n3, byte n4, byte s1, byte s2, byte s3, byte s4, byte e1, byte e2, byte e3, byte e4, byte w1, byte w2, byte w3, byte w4);
    void reset ();
    void test ();

    void attachMotors(byte NWpwm, byte NWa, byte NWb, byte NEpwm, byte NEa, byte NEb, byte SWpwm, byte SWa, byte SWb, byte SEpwm, byte SEa, byte SEb);
    void attachUtils(byte buzzer, int width, int height);
    void attachCompass();

    bool North();
    // bool South();
    // bool East();
    // bool West();

    void wardOff();

    void logNorthValues();
    void logSouthValues();
    void logEastValues();
    void logWestValues();

    bool inN1();
    bool inN2();
    bool inN3();
    bool inN4();

    bool inS1();
    bool inS2();
    bool inS3();
    bool inS4();

    bool inE1();
    bool inE2();
    bool inE3();
    bool inE4();

    bool inW1();
    bool inW2();
    bool inW3();
    bool inW4();

    int _N1 ();
    int _N2 ();
    int _N3 ();
    int _N4 ();
    int _S1 ();
    int _S2 ();
    int _S3 ();
    int _S4 ();
    int _E1 ();
    int _E2 ();
    int _E3 ();
    int _E4 ();
    int _W1 ();
    int _W2 ();
    int _W3 ();
    int _W4 ();
};

#endif
