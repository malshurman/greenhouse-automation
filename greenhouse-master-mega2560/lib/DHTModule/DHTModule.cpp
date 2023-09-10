#include <DHTModule.h>
#include <Pins.h>
#include <DHT.h>

DHT dht(DHT_SENSOR, DHT22);

void initializeDHT()
{
    dht.begin();
}

float getTemperature()
{
    float temperature  = dht.readTemperature();
    if (isnan(temperature))
    {
        Serial.println("Failed to read temperature from DHT sensor!");
        return -1;
    }
    
    return temperature;
}

float getHumidity()
{
    float humidity  = dht.readHumidity();
    if (isnan(humidity))
    {
        Serial.println("Failed to read humidity from DHT sensor!");
        return -1;
    }
    
    return humidity;
}