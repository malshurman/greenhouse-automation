#include <DHTModule.h>
#include <Pins.h>
#include <DHT.h>
#include <LCDModule.h>
#include <TaskManager.h>

#define DHT_READINGS_REFRESH_RATE 2000

static char dhtBuffer[21];

DHTModule& DHTModule::getInstance()
{
    static DHTModule instance;
    return instance;
}

static void writeDHTReadingsToLCDTask()
{
    DHTModule::getInstance().writeDHTReadingsToLCD();
}

DHTModule::DHTModule() : dht(DHT_SENSOR, DHT21) {
    dht.begin();
    TaskManager::getInstance().createTask(DHT_READINGS_REFRESH_RATE, writeDHTReadingsToLCDTask);
}

float DHTModule::getTemperature()
{
    float temperature  = getInstance().dht.readTemperature();
    if (isnan(temperature))
    {
        Serial.println("Failed to read temperature from DHT sensor!");
        return -1;
    }
    return temperature;
}

float DHTModule::getHumidity()
{
    float humidity  = getInstance().dht.readHumidity();
    if (isnan(humidity))
    {
        Serial.println("Failed to read humidity from DHT sensor!");
        return -1;
    }
    return humidity;
}

char* DHTModule::getDHTBuffer()
{
    return dhtBuffer;
}

void DHTModule::writeDHTReadingsToLCD()
{
    float temperature = getTemperature();
    float humidity = getHumidity();

    int tempInt = (int)(temperature * 10);
    int humInt = (int)(humidity * 10);

    if (temperature == -1 || humidity == -1)
    {
        sprintf(dhtBuffer, "     DHT-ERROR     ");
    } else {
        sprintf(dhtBuffer, "T: %d.%dC    H: %d.%d%%", tempInt / 10, abs(tempInt % 10), humInt / 10, abs(humInt % 10));
    }

    LCDModule::getInstance().writeToLCD(0, 1, dhtBuffer);
}