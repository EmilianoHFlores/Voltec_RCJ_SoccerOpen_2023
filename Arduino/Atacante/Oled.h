#include <Adafruit_SSD1306.h>

#ifndef OLED_H
#define OLED_H

class Oled {
  public:
    Oled ();
    void init  ();
    void reset ();
    void test  ();
    void begin ();
    
    void print(int x, int y, int data);
    void print(int x, int y, float data);
    void print(int x, int y, String data);
    void print(int x, int y, int data, int size);
    void print(int x, int y, float data, int size);
    void print(int x, int y, String data, int size);
    void print(int x, int y, long unsigned data, int size);
    void show();
    Adafruit_SSD1306 display();
};

#endif
