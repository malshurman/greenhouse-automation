#include <Arduino.h>
#include <Pump.h>
#include <Pins.h>
#include <RTCModule.h>
#include <Buttons.h>

#define AUTO_PUMP_DURATION_SECONDS 10 * 60
#define DEFAULT_UNIX_TIME 1577880000

struct PumpingTask
{
    bool isActive;
    uint32_t startTimeUnix;
    uint32_t endTimeUnix;

    PumpingTask()
        : isActive(false), startTimeUnix(DEFAULT_UNIX_TIME), endTimeUnix(DEFAULT_UNIX_TIME) {}
};

bool isPumpOn;
PumpingTask autoPumpTask;

void initializePump()
{
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, LOW);
    isPumpOn = false;
    autoPumpTask = PumpingTask();
}

void updatePump()
{
    if (autoPumpTask.isActive)
    {
        if (getCurrentDateTime().unixtime() >= autoPumpTask.endTimeUnix)
        {
            isPumpOn = false;
            autoPumpTask.isActive = false;
            finishAutoPump();

        }
    }
    digitalWrite(RELAY, isPumpOn ? HIGH : LOW);
}

void turnPumpOn()
{
    digitalWrite(RELAY, HIGH);
    isPumpOn = true;
}

void turnPumpOff()
{
    digitalWrite(RELAY, LOW);
    isPumpOn = false;
}

void turnAutoPumpOn()
{
    turnPumpOn();
    autoPumpTask.startTimeUnix = getCurrentDateTime().unixtime();
    autoPumpTask.endTimeUnix = autoPumpTask.startTimeUnix + AUTO_PUMP_DURATION_SECONDS;
    autoPumpTask.isActive = true;
}

void turnAutoPumpOff()
{
    autoPumpTask.isActive = false;
}

bool isPumping()
{
    return isPumpOn;
}

bool isAutoPumping()
{
    return autoPumpTask.isActive;
}

int getSecondsRemaingingAutoPumpTime()
{
    if (!autoPumpTask.isActive) return 0;
    return autoPumpTask.endTimeUnix - getCurrentDateTime().unixtime();
}

uint32_t getAutoPumpStartTime()
{
    return autoPumpTask.startTimeUnix;
}

uint32_t getAutoPumpEndTime()
{
    return autoPumpTask.endTimeUnix;
}