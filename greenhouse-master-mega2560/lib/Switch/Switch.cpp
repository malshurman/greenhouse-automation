#include <Switch.h>

Switch::Switch(const int offPin, const int offLedPin, const int onPin, const int onLedPin, callbackFunction offFunction, const char* offFunctionName, callbackFunction onFunction, const char* onFunctionName)
    : left(offPin, offLedPin, offFunction, offFunctionName), right(onPin, onLedPin, onFunction, onFunctionName) {}

void Switch::switchOff(SoundSetting soundSetting) {
    left.toggleOn(SILENT);
    right.toggleOff(soundSetting);
}

void Switch::switchOn(SoundSetting soundSetting) {
    left.toggleOff(SILENT);
    right.toggleOn(soundSetting);
}

bool Switch::isOn() {
    return right.isToggled();
}