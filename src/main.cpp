#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>

#include "../lib/Display/src/Display.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);         // Set for Display. Uses pin A4, A5
#define BUTTON1PIN 2                        // Set for Button 1. Uses pin 2.
#define BUTTON2PIN 3                        // Set for Button 2. Uses pin 3.
#define BUTTON3PIN 4                        // Set for Button 3. Uses pin 4.
#define BUTTON4PIN 5                        // Set for Button 4. Uses pin 5.
#define INNERBUTTONPIN 6                    // Set for inner Button. Uses pin 6.
int password[4];                            // Array for password.
const int dummyPassword[4] = {1, 2, 3, 4};  // Array with an example password.
#define RFIDRESETPIN 7                      // Set for RFID. Uses pin 7.
#define RFIDSSPIN 8                         // Set for RFID. Uses pin 8.
#define RFIDMOSIPIN 9                       // Set for RFID. Uses pin 9.
#define RFIDMISOPIN 10                      // Set for RFID. Uses pin 10.
#define RFIDSCKPIN 11                       // Set for RFID. Uses pin 11.
MFRC522 rfid(RFIDSSPIN, RFIDRESETPIN);      // Set for RFID.
byte nuidPICC[4];                           // Array to store UUID of the NFC
bool passwordProcess = false;               // Password process status

#define I2C_BUS_ADDRESS 8

// Display functions
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

// Buttons and Password functions
void startButtons() {
    pinMode(BUTTON1PIN, INPUT);
    pinMode(BUTTON2PIN, INPUT);
    pinMode(BUTTON3PIN, INPUT);
    pinMode(BUTTON4PIN, INPUT);
    pinMode(INNERBUTTONPIN, INPUT);
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
    int button1State = digitalRead(BUTTON1PIN);
    int button2State = digitalRead(BUTTON2PIN);
    int button3State = digitalRead(BUTTON3PIN);
    int button4State = digitalRead(BUTTON4PIN);
    int innerButtonState = digitalRead(INNERBUTTONPIN);

    if (innerButtonState == HIGH) {
        // TO-DO activate Servo
    }
    if (passwordProcess == true && passwordIsFillable() == true &&
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

        if (passwordIsFillable() == false) {
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

// RFID functions
void startRFID() {
    SPI.begin();      // Init SPI bus
    rfid.PCD_Init();  // Init MFRC522
}
bool compareRFIDuuids() {
    if (rfid.uid.uidByte[0] == nuidPICC[0] &&
        rfid.uid.uidByte[1] == nuidPICC[1] &&
        rfid.uid.uidByte[2] == nuidPICC[2] &&
        rfid.uid.uidByte[3] == nuidPICC[3]) {
        return true;
    } else {
        return false;
    }
}
void checkRFID() {
    // Verify if the NUID has been readed
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    };

    if (compareRFIDuuids()) {
        // If the uuid are equals, start the password process.
        writeDisplay("Club ExDev", "Tarjeta reconocida");
        passwordProcess = true;
        cleanPassword();
        delay(2000);
        writeDisplay("Ingrese clave:", "");
    } else {
        // This is just for test. Should delete later.
        // This store the card uuid.
        for (byte i = 0; i < 4; i++) {
            nuidPICC[i] = rfid.uid.uidByte[i];
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