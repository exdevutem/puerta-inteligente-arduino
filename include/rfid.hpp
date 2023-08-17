// Constructor
void startRFID() {
    SPI.begin();     // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522
}

String uidToString(MFRC522::Uid& uid) {  
    String rfidUid = "";
    for (byte i = 0; i < uid.size; i++) {
        rfidUid += uid.uidByte[i] < 0x10 ? " 0" : "";
        rfidUid += String(uid.uidByte[i], HEX);
    }
     rfidUid.toUpperCase();
     return rfidUid;
}

bool isMember(String rfid) {
    //size_t n = 6; // sizeof(members) / sizeof(members[0]);
    for (size_t i = 0; i < sizeMembers; i++) {
        // Serial.print("isMember name ");
        // Serial.println(membersList[i].name);
        if (rfid.equals(membersList[i].rfid)) {
            indexMember = i;
            return true;
        }
    }
   return false;
}

bool compareRFIDuids() {
    //findSD("1234");
    /* Serial.println(rfid.uid.uidByte[0]);
    Serial.println(rfid.uid.uidByte[1]);
    Serial.println(rfid.uid.uidByte[2]);
    Serial.println(rfid.uid.uidByte[3]); */
    String currentRfid = uidToString(rfid.uid);
    Serial.println(currentRfid);
    if (isMember(currentRfid)) {
        Serial.println("Valido");
        return true;
    } else {
        Serial.println("Invalido");
        return false;
    }
    /*
    if (rfid.uid.uidByte[0] == nuidPICC[0] &&
        rfid.uid.uidByte[1] == nuidPICC[1] &&
        rfid.uid.uidByte[2] == nuidPICC[2] &&
        rfid.uid.uidByte[3] == nuidPICC[3])
    {
        return true;
    }
    else { 

    }
    */
}

// Main function for RFID.
void checkRFID(bool& _passwordProcess) {
    // Detecting and reading card
    if (!rfid.PICC_IsNewCardPresent()) {
        Serial.println("PICC_IsNewCardPresent");
        return;
    }
    if (!rfid.PICC_ReadCardSerial()){
        Serial.println("PICC_ReadCardSerial()");
        return;
    } 
    
    if (compareRFIDuids()) {
        // If the uid are equals, start the password process.
        startPasswordProcess(_passwordProcess);
    }
    
    // These are to prevent two PICC actives at the same time.
    rfid.PICC_HaltA();      // Halt PICC
    rfid.PCD_StopCrypto1(); // Stop encryption on PCD
}

