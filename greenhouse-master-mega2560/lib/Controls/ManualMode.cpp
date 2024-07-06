#include <ManualMode.h>
#include <Pins.h>
#include <Pump.h>
#include <Curtains.h>
#include <Valve.h>

#define RESET_BUTTON_LONG_PRESS_MS 500

ManualMode::ManualMode() : resetButton(PUSH_BTN_RESET, RESET_LED, toggleResetButton, "toggleResetButton", true, true, RESET_BUTTON_LONG_PRESS_MS, RESET, RESET)
{
    resetButton.toggleOn(SILENT);
}

ManualMode& ManualMode::getInstance()
{
    static ManualMode instance;
    return instance;
}

void ManualMode::toggleResetButton()
{
    if (getInstance().resetButton.isToggled())
    {
        getInstance().resetButton.toggleOff();
    } else {
        getInstance().turnOnAuto();
    }
}

void ManualMode::turnOnAuto()
{
    if (getInstance().resetButton.isToggled())
    {
        return;
    }

    getInstance().resetButton.toggleOn();
    Valve::getInstance().reset();
    Pump::getInstance().reset();
    Curtains::getInstance().reset();
}

void ManualMode::turnOffAuto()
{
    if (!getInstance().resetButton.isToggled()) return;
    
    getInstance().resetButton.toggleOff(SILENT);
}

bool ManualMode::isAutoOn() { return resetButton.isToggled(); }