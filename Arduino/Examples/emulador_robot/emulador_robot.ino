#include <LiquidCrystal.h>

#define attackButton 2
#define stopButton 3
#define defendButton 4

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600);
  lcd.begin(20, 4);
  pinMode(attackButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(defendButton, INPUT);
  printMessage("");
  printButton("");
  printSelfAction(0);
  printRobot2Action(0);
}

int attackButtonLast = 0;
int stopButtonLast = 0;
int defendButtonLast = 0;

// 0 -> Stop
// 1 -> Attacking
// 2 -> Deffending
int currentAction = 0;
int robot2Action = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available()) {
    String readString = Serial1.readString();
    readString = readString.substring(0, readString.length() -2);

    if (readString == "STP") robot2Action = 0;
    if (readString == "ATT") robot2Action = 1;
    if (readString == "DEF") robot2Action = 2;
    printMessage("CONTENT RECEIVED");
  }
  if (!attackButtonLast && digitalRead(attackButton)) {
    printButton("ATT");
    if (robot2Action != 1) {
      send("ATT");
      currentAction = 1;
    } else printMessage("CAN'T MAKE ACTION");
  }
  if (!stopButtonLast && digitalRead(stopButton)) {
    printButton("STP");
    send("STP");
    currentAction = 0;
  }
  if (!defendButtonLast && digitalRead(defendButton)) {
    printButton("DEF");
    if (robot2Action != 2) {
      send("DEF");
      currentAction = 2;
    } else printMessage("CAN'T MAKE ACTION");
  }
  printSelfAction(currentAction);
  printRobot2Action(robot2Action);

  attackButtonLast = digitalRead(attackButton);
  stopButtonLast = digitalRead(stopButton);
  defendButtonLast = digitalRead(defendButton);
  delay(10);
}

void send(String query) {
  Serial1.println(query);
}

void printMessage(String input) {
  lcd.setCursor(0, 0); lcd.print(input + repeat(" ", 20 - input.length()));
}

void printButton(String output) {
  lcd.setCursor(0, 1); lcd.print("BUTTON: ");
  lcd.setCursor(8, 1); lcd.print(output + repeat(" ", 20 - 8 - output.length()));
}

void printSelfAction(int action) {
  lcd.setCursor(0, 2); lcd.print("SELF: ");
  lcd.setCursor(6, 2);
  if (action == 0) lcd.print("STP");
  if (action == 1) lcd.print("ATT");
  if (action == 2) lcd.print("DEF"); 
}

void printRobot2Action(int action) {
  lcd.setCursor(0, 3); lcd.print("ROBOT2: ");
  lcd.setCursor(8, 3);
  if (action == 0) lcd.print("STP");
  if (action == 1) lcd.print("ATT");
  if (action == 2) lcd.print("DEF"); 
}

String repeat(String text, int times) {
  String repeated = text;
  for (int i = 1; i < times; i++) {
    repeated += text;
  }
  return repeated;
}