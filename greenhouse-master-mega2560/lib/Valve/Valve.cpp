#include <Valve.h>
#include <Pins.h>

Valve& Valve::getInstance() {
    static Valve instance;
    return instance;
}

Valve::Valve() : valveButton(PUSH_BTN_VALVE, VALVE_ON_LED, toggle, "toggle", false, false) {
    valveButton.toggleOff(SILENT);
}

void Valve::toggle() {
    getInstance().valveButton.toggle();
}

void Valve::reset() {
    valveButton.toggleOff(SILENT);
}