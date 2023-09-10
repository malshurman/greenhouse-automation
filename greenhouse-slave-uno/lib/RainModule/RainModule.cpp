#include <Arduino.h>
#include "RainModule.h"

#define RAIN_SENSOR_MAX 1024

RainReading parseRainModuleReading(uint8_t pin) {
    int sensorReading = analogRead(pin);
    
    if (sensorReading < 330)
    {
        return DRY;
    } else if (sensorReading < 660)
    {
        return RAINING;
    } else
    {
        return WET;
    }
}