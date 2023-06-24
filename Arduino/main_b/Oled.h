#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef OLED_H
#define OLED_H


class Oled {
  private:
    Adafruit_SSD1306 oledDisplay;
  public:
    Oled () : oledDisplay(128, 64, &Wire, 4) {};
    void begin () {
      if(!oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Old Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
      }
      oledDisplay.fillScreen(SSD1306_BLACK);
      oledDisplay.clearDisplay();
    };
    
    template <typename T>
    void print(int x, int y, T data, int size) {
      oledDisplay.setTextColor(SSD1306_WHITE);
      oledDisplay.setCursor(x, y);
      oledDisplay.setTextSize(size);
      oledDisplay.print(data);
    };

    void show() {
      oledDisplay.display();
      oledDisplay.fillScreen(SSD1306_BLACK);
    };

    Adafruit_SSD1306 display() { return oledDisplay; };
};

#endif
