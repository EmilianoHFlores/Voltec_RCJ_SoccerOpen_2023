#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class Ultrasonic {
  private:
    byte EchoE;
    byte TriggerE;
    byte EchoW;
    byte TriggerW;
    byte EchoS;
    byte TriggerS;
  public:
    Ultrasonic ();
    void begin (byte EchoE, byte TriggerE, byte EchoW, byte TriggerW, byte EchoS, byte TriggerS);

    void West();
    void East();
    void South();

    float west;
    float east;
    float south;
};

#endif
