#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "../lib/Display/src/Display.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);         // Set for Display. Uses pin A4, A5
#define button1Pin 2                        // Set for Button 1. Uses pin 2.
#define button2Pin 3                        // Set for Button 2. Uses pin 3.
#define button3Pin 4                        // Set for Button 3. Uses pin 4.
#define button4Pin 5                        // Set for Button 4. Uses pin 5.
#define innerButtonPin 6                    // Set for inner Button. Uses pin 6.
int password[4];                            // Array for password.
const int dummyPassword[4] = {1, 2, 3, 4};  // Array with an example password.

#define I2C_BUS_ADDRESS 8

// Display functions
// Constructor
void startDisplay() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Display iniciado");
}
// Writer
void writeDisplay(String firstLine, String secondLine) {
    lcd.clear();
    lcd.print(firstLine);
    lcd.setCursor(0, 1);
    lcd.print(secondLine);
}

// Buttons functions
void startButtons() {
    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
    pinMode(button3Pin, INPUT);
    pinMode(button4Pin, INPUT);
    pinMode(innerButtonPin, INPUT);
}
bool passwordIsFillable() {
    bool fillable = false;
    for (int i = 0; i < sizeof(password); i++) {
        if (password[i] == 0) {
            fillable = true;
        }
    }
    return fillable;
}
void cleanPassword() {
    for (int i = 0; i < sizeof(password); i++) {
        password[i] = 0;
    }
}
void setPasswordValue(int value) {
    for (int i = 0; i < sizeof(password); i++) {
        bool done = false;
        if (password[i] == 0 && done == false) {
            password[i] = value;
            done = true;
        }
    }
}
bool comparePasswords() {
    for (int i = 0; i < sizeof(password); i++) {
        int inputPasswordValue = password[i];
        int storedPasswordValue = dummyPassword[i];
        if (inputPasswordValue != storedPasswordValue) {
            return false;
        }
    }
    return true;
}
void checkButtons() {
    int button1State = digitalRead(button1Pin);
    int button2State = digitalRead(button2Pin);
    int button3State = digitalRead(button3Pin);
    int button4State = digitalRead(button4Pin);
    int innerButtonState = digitalRead(innerButtonPin);

    if (button1State == HIGH || button2State == HIGH || button3State == HIGH ||
        button4State == HIGH) {
        if (passwordIsFillable()) {
            // When the password array has at least one 0.
            if (button1State == HIGH) {
                setPasswordValue(1);
            } else if (button2State == HIGH) {
                setPasswordValue(2);
            } else if (button3State == HIGH) {
                setPasswordValue(3);
            } else if (button4State == HIGH) {
                setPasswordValue(4);
            }

            if (passwordIsFillable() == false) {
                if (comparePasswords() == true) {
                    // TO-DO: Turn on the Servo
                    writeDisplay("Ingresar contraseña:", "Exitoso");
                } else {
                    // If the comparison is wrong, this won't let in.
                    writeDisplay("Ingresar contraseña:", "Incorrecta");
                    cleanPassword();
                }
            }
        }
    }
}

// Unordered functions
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

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
    while (0 < Wire.available()) {
        int c = Wire.read();
        menu(c);
    }
    Serial.println(); /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() { Wire.write("Hello NodeMCU"); /*send string on request */ }

void setup() {
    Wire.begin(I2C_BUS_ADDRESS);
    Wire.onReceive(receiveEvent); /* register receive event */
    Wire.onRequest(requestEvent); /* register request event */
    Serial.begin(9600);           /* start serial for debug */
    startDisplay();
    startButtons();
}

void loop() {
    int seconds = millis() / 1000;
    String text = String(seconds) + " Segundos";
    Serial.println(seconds);
    writeDisplay("Prueba", text);
    checkButtons();
    delay(1000);
}