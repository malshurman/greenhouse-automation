#include <Controls.h>
#include <Pins.h>
#include <OneButton.h>
#include <SoundPlayer.h>
#include <Ticker.h>

void tickButtons();

void leftWingLeftTrigger();
void leftWingRightTrigger();
void rightWingLeftTrigger();
void rightWingRightTrigger();
void pumpLeftTrigger();
void pumpRightTrigger();
void pushBtnValveTrigger();
void pushBtnAutoPumpTrigger();
void pushBtnResetAutoTrigger();

Ticker buttonsTicker(tickButtons, 10);

OneButton leftWingSwitchLeft(LEFT_WING_SW_1);
OneButton leftWingSwitchRight(LEFT_WING_SW_2);
OneButton rightWingSwitchLeft(RIGHT_WING_SW_1);
OneButton rightWingSwitchRight(RIGHT_WING_SW_2);
OneButton pumpSwitchLeft(PUMP_SW_1);
OneButton pumpWingSwitchRight(PUMP_SW_2);
OneButton pushBtnValve(PUSH_BTN_VALVE, false, false);
OneButton pushBtnAutoPump(PUSH_BTN_AUTO_PUMP, false, false);
OneButton pushBtnResetAuto(PUSH_BTN_RESET_AUTO);

bool leftWingLeftPosition(false);
bool leftWingRightPosition(false);
bool rightWingLeftPosition(false);
bool rightWingRightPosition(false);
bool pumpSwitchLeftPosition(false);
bool pumpSwitchRightPosition(false);
bool pushBtnValvePressed(false);
bool pushBtnAutoPumpPressed(false);
bool pushBtnResetPressed(false);

void initializeControls()
{
    leftWingSwitchLeft.attachLongPressStart(leftWingLeftTrigger);
    leftWingSwitchRight.attachLongPressStart(leftWingRightTrigger);
    rightWingSwitchLeft.attachLongPressStart(rightWingLeftTrigger);
    rightWingSwitchRight.attachLongPressStart(rightWingRightTrigger);
    pumpSwitchLeft.attachLongPressStart(pumpLeftTrigger);
    pumpWingSwitchRight.attachLongPressStart(pumpRightTrigger);
    pushBtnValve.attachLongPressStart(pushBtnValveTrigger);
    pushBtnAutoPump.attachLongPressStart(pushBtnAutoPumpTrigger);
    pushBtnResetAuto.attachLongPressStart(pushBtnResetAutoTrigger);

    for (int i = LEFT_WING_LEFT_LED; i <= PUMP_ON_LED; i+=2)
    {
        pinMode(i, OUTPUT);
    }
    

    buttonsTicker.start();
}

void updateControls()
{
    buttonsTicker.update();
}

void tickButtons() {
    leftWingSwitchLeft.tick();
    leftWingSwitchRight.tick();
    rightWingSwitchLeft.tick();
    rightWingSwitchRight.tick();
    pumpSwitchLeft.tick();
    pumpWingSwitchRight.tick();
    pushBtnValve.tick();
    pushBtnAutoPump.tick();
    pushBtnResetAuto.tick();
}

void leftWingLeftTrigger()
{
    if (leftWingRightPosition)
    {
        digitalWrite(LEFT_WING_RIGHT_LED, LOW);
        leftWingRightPosition = false;
    }
    digitalWrite(LEFT_WING_LEFT_LED, HIGH);
    leftWingLeftPosition = true;
    playSwitchOff();
};

void leftWingRightTrigger()
{
    if (leftWingLeftPosition)
    {
        digitalWrite(LEFT_WING_LEFT_LED, LOW);
        leftWingLeftPosition = false;
    }
    digitalWrite(LEFT_WING_RIGHT_LED, HIGH);
    leftWingRightPosition = true;
    playSwitchOn();
};

void rightWingLeftTrigger()
{
    if (rightWingRightPosition)
    {
        digitalWrite(RIGHT_WING_RIGHT_LED, LOW);
        rightWingRightPosition = false;
    }
    digitalWrite(RIGHT_WING_LEFT_LED, HIGH);
    rightWingLeftPosition = true;
    playSwitchOff();
};

void rightWingRightTrigger()
{
    if (rightWingLeftPosition)
    {
        digitalWrite(RIGHT_WING_LEFT_LED, LOW);
        rightWingLeftPosition = false;
    }
    digitalWrite(RIGHT_WING_RIGHT_LED, HIGH);
    rightWingRightPosition = true;
    playSwitchOn();
};

void pumpLeftTrigger()
{
    if (pumpSwitchRightPosition)
    {
        digitalWrite(PUMP_ON_LED, LOW);
        pumpSwitchRightPosition = false;
    }
    digitalWrite(PUMP_OFF_LED, HIGH);
    pumpSwitchLeftPosition = true;
    playSwitchOff();
};

void pumpRightTrigger()
{
    if (pumpSwitchLeftPosition)
    {
        digitalWrite(PUMP_OFF_LED, LOW);
        pumpSwitchLeftPosition = false;
    }
    digitalWrite(PUMP_ON_LED, HIGH);
    pumpSwitchRightPosition = true;
    playSwitchOn();
};

void pushBtnAutoPumpTrigger()
{
    if(pushBtnAutoPumpPressed) {
        digitalWrite(AUTO_PUMP_ON_LED, LOW);
        playButtonOff();
    } else {
        digitalWrite(AUTO_PUMP_ON_LED, HIGH);
        playButtonOn();
    }

    pushBtnAutoPumpPressed = !pushBtnAutoPumpPressed;
};

void pushBtnValveTrigger()
{
    if(pushBtnValvePressed) {
        digitalWrite(VALVE_ON_LED, LOW);
        playButtonOff();
    } else {
        digitalWrite(VALVE_ON_LED, HIGH);
        playButtonOn();
    }

    pushBtnValvePressed = !pushBtnValvePressed;
};

void pushBtnResetAutoTrigger()
{
    digitalWrite(AUTO_ON_LED, HIGH);
    playReset();
};