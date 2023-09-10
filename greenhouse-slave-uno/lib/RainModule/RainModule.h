#include <Arduino.h>

typedef enum {
    DRY,
    RAINING,
    WET
} RainReading;

RainReading parseRainModuleReading(uint8_t pin);