#include <SD.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#include "./settings.hpp"
#include "../include/wire.hpp"
#include "../include/display.hpp"
#include "../include/password.hpp"
#include "./buttons.hpp"
#include "../include/rfid.hpp"
#include "../include/sd.hpp"

// -- Unordered functions --
// Must clean up these later.

void menu(int option)
{
  switch (option)
  {
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

void setup()
{
  startWireConnection();
  Serial.begin(9600); /* start serial for debug */
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  startRFID();
  startDisplay();
  startButtons();
  startSD();
  passwordProcess = false;
  ms = 1000;
  //int aux = 0;
}

void loop()
{
int seconds;
    seconds = millis() / 1000;
    String text = String(seconds) + " Segundos";
    Serial.println(text);

  if (passwordProcess) {
    //Serial.println("checkButtons");
    checkButtons(ms);
  } else {
    checkRFID(passwordProcess);
    findSD("1234");
  
  }

  delay(ms);
}
