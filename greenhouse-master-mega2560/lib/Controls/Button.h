#pragma once

#include <OneButton.h>
#include <SoundPlayer.h>

#define DEFAULT_BTN_LONG_PRESS_MS 150

class Button {
public:
    Button(
        const int pin,
        const int ledPin,
        callbackFunction function,
        const char* taskName,
        const bool activeLow = true,
        const bool pullupActive = true,
        const int longPressTime = DEFAULT_BTN_LONG_PRESS_MS,
        const Melody offSound = BUTTON_OFF,
        const Melody onSound = BUTTON_ON
        );

    static bool isResetButton(Button* button);

    void toggle();
    void toggleOn(const SoundSetting soundSetting = SOUND);
    void toggleOff(const SoundSetting soundSetting = SOUND);
    bool isToggled() const;
    void tick();
private:   
    OneButton button;
    const int ledPin;
    const Melody offSound;
    const Melody onSound;
    bool toggled;
    callbackFunction function;
};