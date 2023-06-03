#include "Xbee.h"

Xbee::Xbee () {};

void Xbee::begin() {
  Serial2.begin(baud);
  Serial2.setTimeout(timeout);
}

void Xbee::Send(int action) {
  if (robot2Action() == action && action != 0) return;
  _selfAction = action;
  
  String toPrint;
  if (action == 0) toPrint = "STP";
  else if (action == 1) toPrint = "ATT";
  else toPrint = "DEF";
  Serial2.println(toPrint);
}

void Xbee::Receive() {
  if (Serial2.available()) {
    String readString = Serial2.readString();
    readString = readString.substring(0, readString.length() - 2);
    if (readString == "STP") _robot2Action = 0;
    else if (readString == "ATT") _robot2Action = 1;
    else if (readString == "DEF") _robot2Action = 2;
  }
}