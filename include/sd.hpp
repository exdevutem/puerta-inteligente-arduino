#include "../include/ArduinoJson-v6.21.2.h"

void printDirectory(File dir, int numTabs = 0) {
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

// Check if file already exists in SD
bool checkFileExists(String filename)
{
  return SD.exists(filename) ? true : false;
}

// Find or create
File findOrCreateSD() {
    String filename = "exdev.json"; // Como estamos ocupando la librería SdFat en vez de Sd, se puede poner la ruta completa real
    if (!checkFileExists(filename)) {
        writeDisplay("No se encontro", "el archivo JSON");
        Serial.println("No se encontró el archivo JSON");
        delay(2000);
        exit(EXIT_FAILURE);
        
    }
    File exDevFile = SD.open(filename);
    return exDevFile;
}

void searchForMemberWithId(String id) {
  StaticJsonDocument<64> filter;
}

// Find in file
void loadMembersToList() {
    File exDevFile = findOrCreateSD();
    DynamicJsonDocument doc(400); // CAMBIAR A MEDIDA QUE SE AGREGAN USUARIOS
    DeserializationError error = deserializeJson(doc, exDevFile);

  if (error)
  {
    Serial.print(F("Fallo al leer el archivo JSON: "));
    Serial.println(error.f_str());

    writeDisplay("Fallo al leer", "el archivo JSON");
    delay(2000);
    exit(EXIT_FAILURE);
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
    
    exDevFile.close();
}

// Constructor
void startSD()
{
  if (!SD.begin(SD_PIN))
  {
    Serial.println("No se pudo inicializar");
    writeDisplay("Inicializacion", "de SD fallida");
    exit(EXIT_FAILURE);
  }
  Serial.println("Inicializacion exitosa");
  writeDisplay("Inicializacion", "exitosa");
  delay(2000);
  writeDisplay("Club ExDev");
  loadMembersToList();
}

