void receiveEvent(int howMany) {
    while (0 < Wire.available()) {
        int c = Wire.read();
        // menu(c);
    }
    Serial.println(); /* to newline */
}

void requestEvent() {
    Wire.write("Hello NodeMCU"); /*send string on request */
}

void startWireConnection() {
    Wire.begin(I2C_BUS_ADDRESS);
    Wire.onReceive(receiveEvent); /* register receive event */
    Wire.onRequest(requestEvent); /* register request event */
}