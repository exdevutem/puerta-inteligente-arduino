// Constructor
void startSD()
{
    if (!SD.begin(SDPIN))
    {
        Serial.println("No se pudo inicializar");
        return;
    }
    Serial.println("inicializacion exitosa");
}
// Check if file already exists in SD
bool checkFileExists(String filename)
{
    if (SD.exists(filename))
    {
        return true;
    }
    else
    {
        return false;
    }
}
// Find or create
void findOrCreateSD()
{
    String filename = "exDev.txt";
    if (checkFileExists(filename))
    {
        exDevFile = SD.open(filename);
    }
    else
    {
        boolean created = SD.mkdir(filename);
        if (created)
        {
            exDevFile = SD.open(filename);
        }
        else
        {
            Serial.println("No se pudo crear el archivo");
        }
    }
}
// Write
void writeSD(String text)
{
    findOrCreateSD();
    if (exDevFile)
    {
        exDevFile.println(text);
        exDevFile.close();
    }
}
// Find in file
bool findSD(String text)
{
    findOrCreateSD();
    if (exDevFile)
    {
        bool textFound = false;

        while (exDevFile.available())
        {
            int fileText = exDevFile.read();
            if (fileText == text[0])
            {
                textFound = true;
            }
        }
        exDevFile.close();
        return textFound;
    }
    return false;
}