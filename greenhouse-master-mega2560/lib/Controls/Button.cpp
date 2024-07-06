#include <Button.h>
#include <ButtonManager.h>
#include <ManualMode.h>
#include <Pins.h>

Button::Button(
    const int pin,
    const int ledPin,
    callbackFunction function,
    const char* taskName,
    const bool activeLow,
    const bool pullupActive,
    const int longPressTime,
    const Melody offSound,
    const Melody onSound
    ) : button(pin, activeLow, pullupActive), ledPin(ledPin), offSound(offSound), onSound(onSound), toggled(false), function(function) {
        pinMode(ledPin, OUTPUT);

        button.setPressTicks(longPressTime);
        button.attachLongPressStart([](void* btn) {
            Button* button = static_cast<Button*>(btn);
            button->function();
            if (!isResetButton(button))
            {
                ManualMode::getInstance().turnOffAuto();
            }
        }, this);

        ButtonManager::getInstance().registerButton(this);
}

void Button::toggle() {
    toggled ? toggleOff() : toggleOn();
}

void Button::toggleOn(const SoundSetting soundSetting) {
    if (toggled) {
        if (soundSetting == SOUND) playMelody(FEEDBACK);
        return;
    }
    
    toggled = true;
    digitalWrite(ledPin, HIGH);
    if (soundSetting == SOUND) playMelody(onSound);
}

void Button::toggleOff(const SoundSetting soundSetting) {
    if (!toggled) {
        if (soundSetting == SOUND) playMelody(FEEDBACK);
        return;
    }

    toggled = false;
    digitalWrite(ledPin, LOW);
    if (soundSetting == SOUND) playMelody(offSound);
}

bool Button::isToggled() const {
    return toggled;
}

void Button::tick() {
    button.tick();
}

bool Button::isResetButton(Button* button) {
    return button->ledPin == RESET_LED;
}