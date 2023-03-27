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
  startRFID();
  startDisplay();
  startButtons();
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

  checkRFID(passwordProcess);
  if (passwordProcess) {
    //Serial.println("checkButtons");
    checkButtons(ms);
  }

  delay(ms);
}
