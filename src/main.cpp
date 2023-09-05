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
#include <OnewireKeypad.h>

#define Rows 4
#define Cols 4
#define Pin A0
#define Row_Res 1000
#define Col_Res 4700
// ExtremePrec, HighPrec, MediumPrec, LowPrec

char KEYS[]= {
  '1','2','3','A',
  '4','5','6','B',
  '7','8','9','C',
  '*','0','#','D',
};

OnewireKeypad <Print, 16> KP(Serial, KEYS, Rows, Cols, Pin, Row_Res, Col_Res);

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


  KP.SetHoldTime(1000);
  
  // This method is set in the constructor with a default value of 5.0
  // You only need to include this if your Arduino is not supplying 5v to
  // the keypad. ie. ~4.7v or even with 3.3v Arduino boards too.
  KP.SetKeypadVoltage(5.0);
  
  KP.ShowRange();
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

  //innerButton();
  if (passwordProcess) {
    checkButtons(passwordProcess);
  } else {
    //checkRFID(passwordProcess);               
  }
  //delay(ms);


  //delay(1000);
  char Key;
  byte KState = KP.Key_State();

  if (KState == PRESSED) {
    if ( Key = KP.Getkey() ) {
      Serial << "Pressed: " << Key << "\n";
      switch (Key) {
        case '*': Serial << "Key:" << KP.Getkey() << "\n"; break;
        case '#': Serial << "Key:" << KP.Getkey() << "\n"; break;
      }
    }
  } else if (KState == HELD) {
    Serial << "Key:" << KP.Getkey() << " being held\n";
  }
}
