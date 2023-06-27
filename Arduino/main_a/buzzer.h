#include <Arduino.h>
#include <Wire.h>
#include <Arduino.h>

#ifndef BUZZER_H
#define BUZZER_H


class Buzzer {
  private:
    byte buzzer;

    unsigned long timer = 0;
    unsigned long lastPulse = 0;
  public:
    Buzzer();
    void begin(byte buzzer);
    void beep(int volume);
    void stop();
    
    void startTimer();
    void clear();

    void beeps(int times, int volume);
    void morse(String message, int volume);
}
;

#endif
