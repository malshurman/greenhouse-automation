#pragma once

#include <DHT.h>

class DHTModule {
public:
    DHTModule(const DHTModule&) = delete;
    DHTModule& operator=(const DHTModule&) = delete;

    static DHTModule& getInstance();

    static float getTemperature();
    static float getHumidity();

    void writeDHTReadingsToLCD();
    char* getDHTBuffer();
private:
    DHTModule();
    DHT dht;
};