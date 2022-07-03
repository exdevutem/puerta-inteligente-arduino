#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#include "../lib/Display/src/Display.h"

// -- Globals --
// Pins
#define BUTTON1PIN 2       // Set for Button 1. Uses pin 2.
#define BUTTON2PIN 3       // Set for Button 2. Uses pin 3.
#define BUTTON3PIN 4       // Set for Button 3. Uses pin 4.
#define BUTTON4PIN 5       // Set for Button 4. Uses pin 5.
#define INNERBUTTONPIN 6   // Set for inner Button. Uses pin 6.
#define I2C_BUS_ADDRESS 8  //
#define RFIDRESETPIN 9     // Set for RFID. Uses pin 9.
#define RFIDSSPIN 10       // Set for RFID. Uses pin 10.
#define RFIDMOSIPIN 11     // Set for RFID. Uses pin 11.
#define RFIDMISOPIN 12     // Set for RFID. Uses pin 12.
#define RFIDSCKPIN 13      // Set for RFID. Uses pin 13.
// inits
LiquidCrystal_I2C lcd(0x3F, 16, 2);     // Set for Display. Uses pin A4, A5
MFRC522 rfid(RFIDSSPIN, RFIDRESETPIN);  // Set for RFID.
bool passwordProcess = false;           // Password process status
int password[4];                        // Array for password.
byte nuidPICC[4];                       // Array to store UUID of the NFC
// Dummys
const int dummyPassword[4] = {1, 2, 3, 4};  // Array with an example password.

// -- Display functions --
// Constructor
void startDisplay() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Club ExDev");
}
// Writer
void writeDisplay(String firstLine, String secondLine) {
    lcd.clear();
    lcd.print(firstLine);
    lcd.setCursor(0, 1);
    lcd.print(secondLine);
}

// -- Buttons and Password functions --
// Constructor
void startButtons() {
    pinMode(BUTTON1PIN, INPUT);
    pinMode(BUTTON2PIN, INPUT);
    pinMode(BUTTON3PIN, INPUT);
    pinMode(BUTTON4PIN, INPUT);
    pinMode(INNERBUTTONPIN, INPUT);
}
// Boolean to check if password array is "empty". Empty means {0, 0, 0, 0}
bool isPasswordFillable() {
    bool fillable = false;
    for (int i = 0; i < sizeof(password); i++) {
        if (password[i] == 0) {
            fillable = true;
        }
    }
    return fillable;
}
// Makes the password "empty".
void cleanPassword() {
    for (int i = 0; i < sizeof(password); i++) {
        password[i] = 0;
    }
}
// Replace the first 0 with the value.
void setPasswordValue(int value) {
    for (int i = 0; i < sizeof(password); i++) {
        bool done = false;
        if (password[i] == 0 && done == false) {
            password[i] = value;
            done = true;
        }
    }
}
// Comparison of passwords. If equal, return true.
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
// Main function for buttons.
void checkButtons() {
    int button1State = digitalRead(BUTTON1PIN);
    int button2State = digitalRead(BUTTON2PIN);
    int button3State = digitalRead(BUTTON3PIN);
    int button4State = digitalRead(BUTTON4PIN);
    int innerButtonState = digitalRead(INNERBUTTONPIN);

    if (innerButtonState == HIGH) {
        // TO-DO activate Servo
    }
    if (passwordProcess == true && isPasswordFillable() == true &&
        (button1State == HIGH || button2State == HIGH || button3State == HIGH ||
         button4State == HIGH)) {
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

        if (isPasswordFillable() == false) {
            if (comparePasswords() == true) {
                // TO-DO: Turn on the Servo
                writeDisplay("Ingrese clave:", "Clave correcta");
            } else {
                // If the comparison is wrong, this won't let in.
                writeDisplay("Ingrese clave:", "Clave incorrecta");
                cleanPassword();
            }
            passwordProcess = false;
            delay(2000);
            writeDisplay("Club ExDev", "");
        }
    }
}

// -- RFID functions --
// Constructor
void startRFID() {
    SPI.begin();      // Init SPI bus
    rfid.PCD_Init();  // Init MFRC522
}
// Comparison of RFID UIDs.
bool compareRFIDuids() {
    if (rfid.uid.uidByte[0] == nuidPICC[0] &&
        rfid.uid.uidByte[1] == nuidPICC[1] &&
        rfid.uid.uidByte[2] == nuidPICC[2] &&
        rfid.uid.uidByte[3] == nuidPICC[3]) {
        return true;
    } else {
        return false;
    }
}
// Main function for RFID.
void checkRFID() {
    // Detecting and reading card
    if (!rfid.PICC_IsNewCardPresent()) return;
    if (!rfid.PICC_ReadCardSerial()) return;

    if (compareRFIDuids()) {
        // If the uid are equals, start the password process.
        writeDisplay("Club ExDev", "Bienvenide");
        passwordProcess = true;
        cleanPassword();
        delay(2000);
        writeDisplay("Ingrese clave:", "");
    } else {
        // This is just for test. Should delete later.
        // This store the card uid.
        for (byte i = 0; i < 4; i++) {
            nuidPICC[i] = rfid.uid.uidByte[i];
        }
    }

    // These are to prevent two PICC actives at the same time.
    rfid.PICC_HaltA();       // Halt PICC
    rfid.PCD_StopCrypto1();  // Stop encryption on PCD
}

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
    startRFID();
    startDisplay();
    startButtons();
}

void loop() {
    int seconds = millis() / 1000;
    String text = String(seconds) + " Segundos";
    Serial.println(seconds);

    checkRFID();
    checkButtons();

    delay(1000);
}