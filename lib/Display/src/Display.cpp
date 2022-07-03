#include "./Display.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

Display::Display()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Display iniciado");
}

void Display::write(String firstLine, String secondLine)
{
    lcd.clear();
    lcd.print(firstLine);
    lcd.setCursor(0, 1);
    lcd.print(secondLine);
}