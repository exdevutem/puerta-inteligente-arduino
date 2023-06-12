#include "../include/ArduinoJson-v6.21.2.h"

void printDirectory(File dir, int numTabs) {
   while (true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // No more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
// Constructor
void startSD() {
    if (!SD.begin(SDPIN)) {
        Serial.println("No se pudo inicializar");
        return;
    }
    Serial.println("inicializacion exitosa");
}

// Check if file already exists in SD
bool checkFileExists(String filename) {
    return SD.exists(filename) ? true : false;
}

// Find or create
File findOrCreateSD() {
    String filename = "EXDE~1.JSO"; // NO AGREGA LA N A JSON
    //String filename = "ExDev.json";
    //printDirectory(SD.open("/"), 0);
    if (!checkFileExists(filename)) {
        Serial.println("No se encontró el archivo");
        delay(2000);
        //exit(EXIT_FAILURE);
    }
    exDevFile = SD.open(filename);
    return exDevFile;
    /*
    boolean created = SD.mkdir(filename); //crea carpetas...
    if (created)
    {
        Serial.println("exDev.json creado correctamente");
        exDevFile = SD.open(filename);
    }
    else
    {
        Serial.println("No se pudo crear el archivo");
     }*/
}

// Write
void writeSD(String text, File exDevFile) {
    findOrCreateSD();
    if (exDevFile) {
        exDevFile.println(text);
        exDevFile.close();
    }
}

// Find in file
void findSD() {  // should be int
    if (exDevFile) return;
    
    exDevFile = findOrCreateSD();
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, exDevFile);

    if (error) {
        Serial.print(F("Failed to parse JSON file: "));
        Serial.println(error.f_str());
        return;
    }

    sizeMembers = doc.size();

    delete membersList;
    membersList = new Member[sizeMembers];

    for (size_t i = 0; i < sizeMembers; i++) {
        membersList[i].name = doc[i]["name"];
        membersList[i].rfid = doc[i]["rfid"];
        membersList[i].pass = doc[i]["pass"];
    }
}
