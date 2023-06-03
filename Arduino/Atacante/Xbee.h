#include <Arduino.h>

#ifndef XBEE_H
#define XBEE_H

class Xbee {
  private:
    long baud = 9600;
    long timeout = 10;
    int _selfAction = 0;
    int _robot2Action = 0;
  public:
    Xbee();
    void begin();
    void Send(int action);
    void Receive();
    int selfAction() { return _selfAction; };
    int robot2Action() { return _robot2Action; };
};

#endif