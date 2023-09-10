#include <LCDModule.h>
#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHTModule.h>
#include <RTCModule.h>
#include <Ticker.h>

LiquidCrystal_I2C lcd(0x27,20,4);

void writeDateToLCD();
void writeDHTReadingsToLCD();

Ticker updateDateTicker(writeDateToLCD, 1000, 0, MILLIS);
Ticker updateDHTReadingsTicker(writeDHTReadingsToLCD, 20000, 0, MILLIS);

void initializeLCD()
{
    lcd.init();
    lcd.backlight();
    updateDateTicker.start();
    updateDHTReadingsTicker.start();
}

void updateLCD()
{
    updateDateTicker.update();
    updateDHTReadingsTicker.update();
}

void writeDateToLCD()
{
    DateTime date = getCurrentDateTime();

    char buffer[20];

    sprintf(buffer,"%02u-%02u-%04u  %02u:%02u:%02u", date.day(), date.month(), date.year(), date.hour(), date.minute(), date.second());

    lcd.setCursor(0, 0);
    lcd.print(buffer);
}

void writeDHTReadingsToLCD()
{
    double temperature = getTemperature();
    double humidity = getHumidity();

    char buffer[10];

    if (temperature == -1 || humidity == -1)
    {
        sprintf(buffer, "DHT-ERROR");
    } else {
        sprintf(buffer, "%.2f°C-%.2f%% ", temperature, humidity);
    }
    
    lcd.setCursor(1, 0);
    lcd.print(buffer);
}

