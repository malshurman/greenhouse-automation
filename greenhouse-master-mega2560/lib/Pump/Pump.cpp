#include <Pump.h>
#include <Pins.h>
#include <LCDModule.h>
#include <TaskManager.h>
#include <Protocol.h>

#define PUMP_READINGS_REFRESH_RATE 250

static char pumpStatusBuffer[21];

Pump& Pump::getInstance()
{
    static Pump instance;
    return instance;
}

static void writePumpStatusToLCDTask()
{
    Pump::getInstance().writePumpStatusToLCD();
}

static void updateAutoPumpTask()
{
    if (!Pump::getInstance().isAutoPumpOn()) return;
    if (Pump::getInstance().getSecondsRemaingingAutoPumpTime() <= 0) Pump::getInstance().autoPumpToggle();
}

Pump::Pump()
    : pumpSwitch(PUMP_SW_1, PUMP_OFF_LED, PUMP_SW_2, PUMP_ON_LED, pumpOff, "pumpOff", pumpOn, "pumpOn"),
      autoPumpButton(PUSH_BTN_AUTO_PUMP, AUTO_PUMP_ON_LED, autoPumpToggle, "autoPumpToggle", false, false),
      autoPumpStartTime(DateTime(SECONDS_FROM_1970_TO_2000)), autoPumpEndTime(DateTime(SECONDS_FROM_1970_TO_2000)) {
        pumpSwitch.switchOff(SILENT);
        TaskManager::getInstance().createTask(PUMP_READINGS_REFRESH_RATE, writePumpStatusToLCDTask);
        TaskManager::getInstance().createTask(PUMP_READINGS_REFRESH_RATE, updateAutoPumpTask);
    }

void Pump::pumpOn()
{
    getInstance().pumpSwitch.switchOn();
    getInstance().autoPumpButton.toggleOff(SILENT);
}

void Pump::pumpOff()
{
    getInstance().pumpSwitch.switchOff();
    getInstance().autoPumpButton.toggleOff(SILENT);
}

void Pump::autoPumpToggle()
{
    getInstance().autoPumpButton.toggle();
    if (getInstance().autoPumpButton.isToggled()) {
        getInstance().setAutoPumpTimer();
        getInstance().pumpSwitch.switchOn(SILENT);
    } else {
        getInstance().pumpSwitch.switchOff(SILENT);
    }
}

bool Pump::isPumpOn()
{
    return pumpSwitch.isOn();
}

bool Pump::isAutoPumpOn()
{
    return autoPumpButton.isToggled();
}

int Pump::getSecondsRemaingingAutoPumpTime()
{
    if (!autoPumpButton.isToggled()) return 0;
    DateTime now = RTCModule::getInstance().getCurrentDateTime();
    return (autoPumpEndTime - now).totalseconds();
}

void Pump::writePumpStatusToLCD()
{
    int totalSeconds = getSecondsRemaingingAutoPumpTime();
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    if (isAutoPumpOn())
    {
        sprintf(pumpStatusBuffer, "     Pump: %02d:%02d", minutes, seconds);
    }
    else if (isPumpOn())
    {
        sprintf(pumpStatusBuffer, "     Pump:   ON    ");
    }
    else
    {
        sprintf(pumpStatusBuffer, "     Pump:   OFF   ");
    }

    LCDModule::getInstance().writeToLCD(0, 3, pumpStatusBuffer);
}

void Pump::setAutoPumpTimer()
{
    autoPumpStartTime = RTCModule::getInstance().getCurrentDateTime();
    autoPumpEndTime = autoPumpStartTime + TimeSpan(0, 0, Protocol::getInstance().getAutoPumpDurationMinutes(), 0);
}

uint32_t Pump::getAutoPumpStartTime()
{
    return autoPumpStartTime.unixtime();
}

uint32_t Pump::getAutoPumpEndTime()
{
    return autoPumpEndTime.unixtime();
}

void Pump::reset()
{
    pumpSwitch.switchOff(SILENT);
    autoPumpButton.toggleOff(SILENT);
    autoPumpStartTime = DateTime(SECONDS_FROM_1970_TO_2000);
    autoPumpEndTime = DateTime(SECONDS_FROM_1970_TO_2000);
}