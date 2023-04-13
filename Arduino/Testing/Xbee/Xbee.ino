#define button A0
#define buttonClear A1
#define ROBOT_NAME "Flor"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(buttonClear, INPUT);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("INPUT: ");
  lcd.setCursor(0, 1);
  lcd.print("OUTPUT: ");
}

int lastPulse = 0;
void loop() {
  if (Serial1.available()) {
    String str = Serial1.readString();
    str = str.substring(0, str.length() - 2); // Remove the 2 white spaces at the end
    lcd.setCursor(7, 0);
    lcd.print(str);
    lcd.print(repeat(" ", 16 - (7 + str.length())));
    if (isCommand(str)) {
      if (commandName(str) == "") {
        output("OK");
      } else if (commandName(str) == "NAME") {
        output(ROBOT_NAME);
      }
    }

  }

  if (digitalRead(button) == HIGH && lastPulse == LOW) {
    output("XB+NAME");
  }
  lastPulse = digitalRead(button);
}

void output(String data) {
  Serial1.println(data);
  lcd.setCursor(8, 1);
  lcd.print(data);
  lcd.print(repeat(" ", 16 - (7 + data.length())));
}

bool isCommand (String command) {
  return command.substring(0, 3) == "XB+" ? true : false;
}

String commandName (String command) {
  return command.substring(3, command.length());
}

String repeat(String text, int times) {
  String repeated = text;
  for (int i = 1; i < times; i++) {
    repeated += text;
  }
  return repeated;
}