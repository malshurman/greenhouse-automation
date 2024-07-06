#include <Curtains.h>
#include <Pins.h>

Curtains& Curtains::getInstance() {
    static Curtains instance;
    return instance;
}

Curtains::Curtains()
    : leftCurtainSwitch(LEFT_WING_SW_1, LEFT_WING_LEFT_LED, LEFT_WING_SW_2, LEFT_WING_RIGHT_LED, leftCurtainDown, "leftCurtainDown", leftCurtainUp, "leftCurtainUp"),
      rightCurtainSwitch(RIGHT_WING_SW_1, RIGHT_WING_LEFT_LED, RIGHT_WING_SW_2, RIGHT_WING_RIGHT_LED, rightCurtainDown, "rightCurtainDown", rightCurtainUp, "rightCurtainUp") {
        leftCurtainSwitch.switchOff(SILENT);
        rightCurtainSwitch.switchOff(SILENT);
      }

void Curtains::leftCurtainUp() {
    getInstance().leftCurtainSwitch.switchOn();
}

void Curtains::leftCurtainDown() {
    getInstance().leftCurtainSwitch.switchOff();
}

void Curtains::rightCurtainUp() {
    getInstance().rightCurtainSwitch.switchOn();
}

void Curtains::rightCurtainDown() {
    getInstance().rightCurtainSwitch.switchOff();
}

void Curtains::reset() {
    leftCurtainSwitch.switchOff(SILENT);
    rightCurtainSwitch.switchOff(SILENT);
}