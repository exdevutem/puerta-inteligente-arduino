void startDisplay()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Club ExDev");
}

void writeDisplay(String firstLine, String secondLine)
{
    lcd.clear();
    lcd.print(firstLine);
    lcd.setCursor(0, 1);
    lcd.print(secondLine);
}