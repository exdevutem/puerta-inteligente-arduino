#include <Arduino.h>
#include <Wire.h>
#include "../lib/Display/src/Display.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // Set for Display

#define I2C_BUS_ADDRESS 8

// Display functions
// Constructor
void startDisplay()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Display iniciado");
}
// Writer
void writeDisplay(String firstLine, String secondLine)
{
  lcd.clear();
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
}

// Unordered functions
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

// function that executes whenever data is received from master
void receiveEvent(int howMany)
{
  while (0 < Wire.available())
  {
    int c = Wire.read();
    menu(c);
  }
  Serial.println(); /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent()
{
  Wire.write("Hello NodeMCU"); /*send string on request */
}

void setup()
{
  Wire.begin(I2C_BUS_ADDRESS);
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  Serial.begin(9600);           /* start serial for debug */
  startDisplay();
}

void loop()
{
  int seconds = millis() / 1000;
  String text = String(seconds) + " Segundos";
  Serial.println(seconds);
  writeDisplay("Prueba", text);
  delay(1000);
}