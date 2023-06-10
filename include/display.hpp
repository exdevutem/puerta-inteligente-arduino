void startDisplay() {
    lcd.init();
    lcd.setBacklight(HIGH);
    lcd.clear();
    lcd.print("Club ExDev");
}

void writeFirstLine(String text) {
    lcd.setCursor(0, 0);
    lcd.print(text);

}

void writeSecondLine(String text) {
    lcd.setCursor(0, 1);
    lcd.print(text);
}

void writeDisplay(String firstLine, String secondLine) {
    lcd.clear();
    writeFirstLine(firstLine);
    writeSecondLine(secondLine);
}

void writeIn(String text, int col, int row) {
    lcd.setCursor(col, row);
    lcd.print(text);
}