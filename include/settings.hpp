// Pins
#define BUTTON1PIN 2      // Set for Button 1. Uses pin 2.
#define BUTTON2PIN 3      // Set for Button 2. Uses pin 3.
#define BUTTON3PIN 4      // Set for Button 3. Uses pin 4.
#define BUTTON4PIN 5      // Set for Button 4. Uses pin 5.
#define INNERBUTTONPIN 6  // Set for inner Button. Uses pin 6.
#define SDPIN 7           // Set for SD module. Uses pin 7.
#define I2C_BUS_ADDRESS 8 //
#define RFIDRESETPIN 9    // Set for RFID. Uses pin 9.
#define RFIDSSPIN 10      // Set for RFID. Uses pin 10.
#define RFIDMOSIPIN 11    // Set for RFID. Uses pin 11.
#define RFIDMISOPIN 12    // Set for RFID. Uses pin 12.
#define RFIDSCKPIN 13     // Set for RFID. Uses pin 13.

// inits
LiquidCrystal_I2C lcd(0x3F, 16, 2);    // Set for Display. Uses pin A4, A5
MFRC522 rfid(RFIDSSPIN, RFIDRESETPIN); // Set for RFID.
bool passwordProcess = false;          // Password process status
int password[4];                       // Array for password.
byte nuidPICC[4];                      // Array to store UUID of the NFC
File exDevFile;                        // File to manipulate the SD file.

// Dummys
const int dummyPassword[4] = {1, 2, 3, 4}; // Array with an example password.