// Constructor
void startButtons()
{
    pinMode(BUTTON1PIN, INPUT);
    pinMode(BUTTON2PIN, INPUT);
    pinMode(BUTTON3PIN, INPUT);
    pinMode(BUTTON4PIN, INPUT);
    pinMode(INNERBUTTONPIN, INPUT);
}

int lastButton1State = 0;
int lastButton2State = 0;
int lastButton3State = 0;
int lastButton4State = 0;

// index de password.
//int index = 0;
// Main function for buttons.
void checkButtons(int& _ms)
{
    ms = 50;
    int index = 0;
    int button1State = digitalRead(BUTTON1PIN);
    int button2State = digitalRead(BUTTON2PIN);
    int button3State = digitalRead(BUTTON3PIN);
    int button4State = digitalRead(BUTTON4PIN);
    int innerButtonState = digitalRead(INNERBUTTONPIN);
    if (innerButtonState == HIGH)
    {
        // TO-DO activate Servo
    }   
    if (password[passSize-1] == 0)
    {
        //Serial.println("password");
        // When the password array has at least one 0.
        if (button1State == HIGH && lastButton1State != button1State)
        {
            Serial.println("Boton 1");
            password[index] = 1;
            index++;
        }
        if (button2State == HIGH && lastButton2State != button2State)
        {
            Serial.println("Boton 2");
            password[index] = 2;
            index++;
        }
        if (button3State == HIGH && lastButton3State != button3State)
        {
            Serial.println("Boton 3");
            password[index] = 3;
            index++;
        }
        if (button4State == HIGH && lastButton4State != button4State)
        {
            Serial.println("Boton 4");
            password[index] = 4;
            index++;
        }
    } else
      {
            if (comparePasswords())
            {
                // TO-DO: Turn on the Servo
                writeDisplay("Ingrese clave:", "Clave correcta");
                Serial.println("Clave correcta");
            }
            else
            {
                // If the comparison is wrong, this won't let in.
                writeDisplay("Ingrese clave:", "Clave incorrecta");
                //cleanPassword();
                Serial.println("Clave incorrecta");
            }
            passwordProcess = false;
            delay(2000);
            writeDisplay("Club ExDev", "");
            index = 0;
            _ms = 1000;
      }
    // save the current state as the last state, for next time through the loop
    lastButton1State = button1State;
    lastButton2State = button2State;
    lastButton3State = button3State;
    lastButton4State = button4State;
}
