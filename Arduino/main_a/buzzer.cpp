#include "buzzer.h"

Buzzer::Buzzer() {};

void Buzzer::begin(byte buzzer) {
  this -> buzzer = buzzer;
  this -> timer = timer;
  pinMode(buzzer, OUTPUT);
}

void Buzzer::beep(int volume) {
    analogWrite(buzzer, volume);
}

void Buzzer::stop() {
    analogWrite(buzzer, 0);
}

void Buzzer::startTimer() {
    timer = millis();
}

void Buzzer::beeps(int times, int volume) {
    unsigned long now = millis() - timer;
    int interval = 200;
    if (now < interval) { beep (volume); return; }
    if (now < (interval * 2)) { stop (); return; }
    for (int i = 1; i < times; i++) {
        if (now < (interval + (interval * 2) * i)) { beep(volume); return; }
        if (now < ((interval * 2) + (interval * 2) * i)) { stop(); return; }
    }
}

void Buzzer::morse(String message, int volume) {
    unsigned long now = millis() - timer;
    int interval = 50;
    for (int i = 0; i < message.length();) {
        switch (message[i]) {
        case '.':
            beep();
            if (lastPulse >= interval) {
                stop();
                lastPulse = 0;
            }
            break;
        case '-':
            beep();
            if (lastPulse >= (interval * 3)) {
                stop();
                lastPulse = 0;
            }
            break;
        case ' ':
            stop();
            if (lastPulse >= interval) lastPulse = 0;
            break;
        case '/':
            stop();
            if (lastPulse >= (interval * 3)) lastPulse = 0;
            break;
        default:
            stop();
        }
        lastPulse = millis() - timer;
    }
    

}