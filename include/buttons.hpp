// Constructor
void startButtons()
{
    pinMode(BUTTON1PIN, INPUT);
    pinMode(BUTTON2PIN, INPUT);
    pinMode(BUTTON3PIN, INPUT);
    pinMode(BUTTON4PIN, INPUT);
    pinMode(INNERBUTTONPIN, INPUT);
}

// Main function for buttons.
void checkButtons()
{
    int button1State = digitalRead(BUTTON1PIN);
    int button2State = digitalRead(BUTTON2PIN);
    int button3State = digitalRead(BUTTON3PIN);
    int button4State = digitalRead(BUTTON4PIN);
    int innerButtonState = digitalRead(INNERBUTTONPIN);

    if (innerButtonState == HIGH)
    {
        // TO-DO activate Servo
    }
    if (passwordProcess == true && isPasswordFillable() == true &&
        (button1State == HIGH || button2State == HIGH || button3State == HIGH ||
         button4State == HIGH))
    {
        // When the password array has at least one 0.
        if (button1State == HIGH)
        {
            setPasswordValue(1);
        }
        else if (button2State == HIGH)
        {
            setPasswordValue(2);
        }
        else if (button3State == HIGH)
        {
            setPasswordValue(3);
        }
        else if (button4State == HIGH)
        {
            setPasswordValue(4);
        }

        if (isPasswordFillable() == false)
        {
            if (comparePasswords() == true)
            {
                // TO-DO: Turn on the Servo
                writeDisplay("Ingrese clave:", "Clave correcta");
            }
            else
            {
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