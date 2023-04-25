#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Oled.h"

#define WIDTH 128
#define HEIGHT 64
#define RESET 4

const int OLEDa = SSD1306_WHITE;
const int OLEDb = SSD1306_BLACK;

Adafruit_SSD1306 dspy(WIDTH, HEIGHT, &Wire, RESET);

Oled::Oled() {}

void Oled::init() {
  if(!dspy.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Old Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }
  dspy.fillScreen(OLEDb);
  dspy.clearDisplay(); 
}

void Oled::begin() {
  init();
}

void Oled::test() {
  print(10, 10, "I'm, alive");
  show();
}

void Oled::reset() {
  dspy.fillScreen(OLEDb);
  dspy.clearDisplay();
  show();
}

void Oled::print(int x, int y, int data) {
  dspy.setTextColor(OLEDa);
  dspy.setCursor(x, y);
  dspy.setTextSize(2);
  dspy.print(data);
}
void Oled::print(int x, int y, int data, int size) {
  dspy.setTextColor(OLEDa);
  dspy.setCursor(x, y);
  dspy.setTextSize(size);
  dspy.print(data);
}

void Oled::print(int x, int y, float data) {
  dspy.setTextColor(OLEDa);
  dspy.setCursor(x, y);
  dspy.setTextSize(2);
  dspy.print(data);
}
void Oled::print(int x, int y, float data, int size) {
  dspy.setTextColor(OLEDa);
  dspy.setCursor(x, y);
  dspy.setTextSize(size);
  dspy.print(data);
}

void Oled::print(int x, int y, String data) {
  dspy.setTextColor(OLEDa);
  dspy.setCursor(x, y);
  dspy.setTextSize(1);
  dspy.print(data);
}
void Oled::print(int x, int y, String data, int size) {
  dspy.setTextColor(OLEDa);
  dspy.setCursor(x, y);
  dspy.setTextSize(size);
  dspy.print(data);
}

void Oled::show() {
  dspy.display();
  dspy.fillScreen(OLEDb);
}

Adafruit_SSD1306 Oled::display() {
  return dspy;
}
