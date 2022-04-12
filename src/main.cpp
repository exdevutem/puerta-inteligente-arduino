#include <Arduino.h>
#include <Wire.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ButtonVal = analogRead(0);
  Serial.print("BUTTON: ");
  Serial.println(ButtonVal);
}

