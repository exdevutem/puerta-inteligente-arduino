#include <SD.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

#include "./settings.hpp"
#include "../include/wire.hpp"
#include "../include/display.hpp"
#include "../include/password.hpp"
#include "./buttons.hpp"
#include "../include/rfid.hpp"
#include "../include/sd.hpp"

// -- Unordered functions --
// Must clean up these later.

void menu(int option) {
  switch (option) {
    case 1:
      Serial.print("Opción 1");
      break;
    case 2:
      Serial.print("Opción 2");
      break;
    default:
      Serial.print("Opción default");
      break;
  }
}

void setup() {
  //startWireConnection();
  Serial.begin(9600); /* start serial for debug */
  SPI.begin();     // Init SPI bus
  startDisplay();
  startRFID();
  startButtons();
  startSD();
  passwordProcess = false;
  loadMembersToList();
}

void loop() {
  /*
  int seconds = millis() / 1000;
  Serial.println(String(seconds) + " Segundos"); 
  MG995_Servo.write(180);
  delay(2000);
  MG995_Servo.write(0);
  delay(2000);
  */
 innerButton();
  if (passwordProcess) {
    checkButtons(passwordProcess);
  } else {
    checkRFID(passwordProcess);               
  }
  delay(ms);
}
