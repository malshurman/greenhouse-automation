#include <LiquidCrystal_I2C.h>
#include <DHTModule.h>
#include <RTCModule.h>
#include <Buttons.h>
#include <Pump.h>

LiquidCrystal_I2C lcd(0x27,20,4);

char dateTimeBuffer[20];
char tempBuffer[10];
char humBuffer[10];
char pumpBuffer[20];

byte degreeSymbol[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void initializeLCD()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.createChar(0, degreeSymbol);
}

void writeDateAndTimeToLCD()
{
    DateTime date = getCurrentDateTime();

    sprintf(dateTimeBuffer,"%02u-%02u-%04u  %02u:%02u:%02u", date.day(), date.month(), date.year(), date.hour(), date.minute(), date.second());

    lcd.setCursor(0, 0);
    lcd.print(dateTimeBuffer);
}

void writeDHTReadingsToLCD()
{
    float temperature = getTemperature();
    float humidity = getHumidity();

    int tempInt = (int)(temperature * 10);
    int humInt = (int)(humidity * 10);

    lcd.setCursor(0, 1);

    if (temperature == -1 || humidity == -1)
    {
        lcd.print("DHT-ERROR");
    } else {
        char tempBuffer[16];
        sprintf(tempBuffer, " T: %d.%d", tempInt / 10, abs(tempInt % 10));
        lcd.print(tempBuffer);
        
        lcd.write(byte(0));
        
        char humBuffer[16];
        sprintf(humBuffer, "C H: %d.%d%%", humInt / 10, abs(humInt % 10));
        lcd.print(humBuffer);
    }
}

void writePumpStatusToLCD() {
    lcd.setCursor(0, 2);
    int totalSeconds = getSecondsRemaingingAutoPumpTime();
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    if (isAutoPumping())
    {
        sprintf(pumpBuffer, "     Pump: %02d:%02d", minutes, seconds);
        lcd.print(pumpBuffer);
    }
    else if (isPumping())
    {
        lcd.print("     Pump:   ON     ");
    }
    else
    {
        lcd.print("     Pump:   OFF    ");
    }
    

    
}

void writeConnectionStatusToLCD() {
    lcd.setCursor(0, 3);
    lcd.print("Connected: YES");
}