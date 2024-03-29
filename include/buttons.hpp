

unsigned long lastBeat = 0;
const unsigned long timeout = 10000;

int lastButton1State = 0;
int lastButton2State = 0;
int lastButton3State = 0;
int lastButton4State = 0;
int lastInnerButton = 0;

// index de password.
int index = 0;

// Constructor
void startButtons()
{
    pinMode(BUTTON1PIN, INPUT);
    pinMode(BUTTON2PIN, INPUT);
    pinMode(BUTTON3PIN, INPUT);
    pinMode(BUTTON4PIN, INPUT);
    pinMode(INNERBUTTONPIN, INPUT);
}

void openServo()
{
    MG995_Servo.attach(Servo_PWM); // Connect D6 of Arduino with PWM signal pin of servo motor
    MG995_Servo.write(180);        //Turn left high speed
    isServoOpen = true;
    delay(2000);
    MG995_Servo.detach(); //Stop. You can use deatch function or use write(x), as x is the middle of 0-180 which is 90, but some lack of precision may change this value
}

void closeServo()
{
    MG995_Servo.attach(Servo_PWM); // Connect D6 of Arduino with PWM signal pin of servo motor
    MG995_Servo.write(0);          // Turn clockwise at high speed
    isServoOpen = false;
    delay(2000);
    MG995_Servo.detach(); //Stop. You can use deatch function or use write(x), as x is the middle of 0-180 which is 90, but some lack of precision may change this value
}

void Servo()
{
    if (isServoOpen)
    {
        closeServo();
    }
    else
    {
        openServo();
    }
}

void onButtonPressed(int value)
{
    if (DEBUG)
    {
        writeIn(String(value), index, 1);
    }
    else
    {
        writeIn("*", index, 1);
    }

    lastBeat = millis();

    Serial.print("Boton ");
    Serial.println(value);
    password[index] = value;
    index++;
}

void onEndProcess(bool &_passwordProcess)
{
    ms = 1000;
    _passwordProcess = false;
    writeDisplay("Club ExDev", "");
    index = 0;
    lastBeat = 0;
}

void checkTimeout(bool &_passwordProcess)
{
    if (lastBeat == 0)
    {
        lastBeat = millis();
    }

    unsigned long difference = millis() - lastBeat;

    if (DEBUG)
    {
        //Serial.println(differnece);
    }

    if (difference >= timeout)
    {
        writeDisplay("Ingrese clave:", "Tiempo excedido");
        Serial.println("Timeout");
        delay(1000);
        onEndProcess(_passwordProcess);
    }
}

String arrToString(int *arr, size_t size)
{
    String result = "";
    size_t n = size / sizeof(arr);
    for (size_t i = 0; i < n; i++)
    {
        result += arr[i];
    }
    return result;
}

// Main function for buttons.
void checkButtons(bool &_passwordProcess)
{
    checkTimeout(_passwordProcess);

    int button1State = digitalRead(BUTTON1PIN);
    int button2State = digitalRead(BUTTON2PIN);
    int button3State = digitalRead(BUTTON3PIN);
    int button4State = digitalRead(BUTTON4PIN);

    if (password[passSize - 1] == 0)
    {
        //Serial.println("password");
        // When the password array has at least one 0.
        if (button1State == HIGH && lastButton1State != button1State)
        {
            onButtonPressed(1);
        }
        if (button2State == HIGH && lastButton2State != button2State)
        {
            onButtonPressed(2);
        }
        if (button3State == HIGH && lastButton3State != button3State)
        {
            onButtonPressed(3);
        }
        if (button4State == HIGH && lastButton4State != button4State)
        {
            onButtonPressed(4);
        }
    }
    else
    {
        String p = arrToString(password, sizeof(password));
        if (comparePasswords(p))
        {
            // TO-DO: Turn on the Servo
            writeDisplay("Ingrese clave:", "Clave correcta");
            Serial.println("Clave correcta");
            Servo();
        }
        else
        {
            // If the comparison is wrong, this won't let in.
            writeDisplay("Ingrese clave:", "Clave incorrecta");
            cleanPassword();
            Serial.println("Clave incorrecta");
        }
        delay(2000);
        onEndProcess(_passwordProcess);
    }
    // save the current state as the last state, for next time through the loop
    lastButton1State = button1State;
    lastButton2State = button2State;
    lastButton3State = button3State;
    lastButton4State = button4State;
}

void innerButton()
{
    int innerButtonState = digitalRead(INNERBUTTONPIN);
    if (innerButtonState == HIGH /*&& lastInnerButton != innerButtonState*/)
    {
        Serial.println("InnerButton");
        // TO-DO activate Servo
        Servo();
    }
    lastInnerButton = innerButtonState;
}
