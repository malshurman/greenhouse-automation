#pragma once

#include <Switch.h>
#include <RTCModule.h>

class Pump {
public:
    Pump(const Pump&) = delete;
    Pump& operator=(const Pump&) = delete;

    static Pump& getInstance();

    static void pumpOn();
    static void pumpOff();
    static void autoPumpToggle();

    bool isPumpOn();
    bool isAutoPumpOn();
    int getSecondsRemaingingAutoPumpTime();
    void writePumpStatusToLCD();
    uint32_t getAutoPumpStartTime();
    uint32_t getAutoPumpEndTime();
    void reset();
private:
    Pump();
    void setAutoPumpTimer();

    Switch pumpSwitch;
    Button autoPumpButton;
    DateTime autoPumpStartTime;
    DateTime autoPumpEndTime;
};