// Boolean to check if password array is "empty". Empty means {0, 0, 0, 0}





bool isPasswordFillable() {
    bool fillable = false;
    for (int i = 0; i < passSize; i++) {
        if (password[i] == 0) {
            fillable = true;
        }
    }
    return fillable;
}
// Makes the password "empty".
void cleanPassword() {
    for (int i = 0; i < passSize; i++) {
        password[i] = 0;
    }
}

// Replace the first 0 with the value.
void setPasswordValue(int value) {
    for (int i = 0; i < passSize; i++) {
        bool done = false;
        if (password[i] == 0 && done == false) {
            password[i] = value;
            done = true;
        }
    }
}

void startPasswordProcess(bool& _passwordProcess) {
    Serial.println("Bienvenide");
    writeDisplay("Bienvenidx", membersList[indexMember].name);
    _passwordProcess = true;
    cleanPassword();
    delay(1500);
    writeDisplay("Ingrese clave:", "");
    ms = 50;
}

// Comparison of passwords. If equal, return true.
bool comparePasswords(String password) {
    return password.equals(membersList[indexMember].pass) ? true : false;
}