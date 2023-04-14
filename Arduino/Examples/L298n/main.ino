/*
  Este programa iniciará un robot sin instrucciones, para ejemplos de funciones del robot referir a Arduino/Examples
*/

// Para información de las librerías instaladas, referir a Arduino/lib_requirements.txt
#include <LiquidCrystal_I2C.h>

#include "Motor.h"
#include "Utils.h"
#include "Camera.h"
#include "Compass.h"
#include "Ultrasonic.h"

Motor motor;
Utils utils;
Camera camera;
Compass compass;
Ultrasonic ultrasonic;
LiquidCrystal_I2C lcd (0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando       ");
  
  motor.begin(NWa, NWb, NEa, NEb, SWa, SWb, SEa, SEb);
  camera.begin(camera_baud_rate, camera_timeout);
  compass.begin(compass_type);
  ultrasonic.begin(EastEcho, EastTrigger, WestEcho, WestTrigger, SouthEcho, SouthTrigger);
  utils.begin(buzzer, camera_width, camera_height);

  motor.attachUtils(buzzer, camera_width, camera_height);
  motor.attachCompass(compass_type);

  // Delay de 5 segundos antes de iniciar el programa
  motor.reset(5000);
}

void loop () {
  // Concatenar funciones
  // Para ejemplos utilizando las librerías, referir a Arduino/Examples
}
