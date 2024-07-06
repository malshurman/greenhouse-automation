#pragma once

#include <Button.h>

class Switch {
public:
    Switch(const int offPin, const int offLedPin, const int onPin, const int onLedPin, callbackFunction offFunction, const char* offFunctionName, callbackFunction onFunction, const char* onFunctionName);

    void switchOff(SoundSetting soundSetting = SOUND);
    void switchOn(SoundSetting soundSetting = SOUND);
    bool isOn();

private:
    Button left;
    Button right;
};