#include <Servo.h>

#ifndef DRIBBLER_H
#define DRIBBLER_H

class Dribbler {
  private:
    byte pin;
    byte led;
    Servo Brushless;
  public:
    Dribbler () {};
    void begin(byte brushlessPin, byte ledPin) {
        pin = brushlessPin;
        led = ledPin;
        Brushless.attach(brushlessPin);
        Brushless.writeMicroseconds(1000);
    };

    void run(uint8_t speed) {
        int output = map(speed, 0, 255, 1000, 2000);
        Brushless.writeMicroseconds(output);
        analogWrite(led, speed);
    };
    void stop() {
        run(0);
    };
};

#endif
