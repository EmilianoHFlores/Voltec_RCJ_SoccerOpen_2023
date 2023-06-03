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

    float East();
    float West();
    float South();
    
    float x_location();
    float y_location();
    float angleToGoal();
};

#endif
