#include <Buttons.h>
#include <Ticker.h>

#define NUM_OF_BUTTONS 9

void initializeLEDs();
void tickButtons();

Ticker buttonsTicker(tickButtons, 10);

void initializeButtons()
{
    initializeLEDs();
    buttonsTicker.start();
}

void updateButtons() { buttonsTicker.update(); }

void leftWingLeftTrigger();
void leftWingRightTrigger();
void rightWingLeftTrigger();
void rightWingRightTrigger();
void pumpLeftTrigger();
void pumpRightTrigger();
void pushBtnValveTrigger();
void pushBtnAutoPumpTrigger();
void pushBtnResetAutoTrigger();

struct Button {
    OneButton button;
    bool isActive;
    Button(const int pin, callbackFunction function, const bool activeLow = true, const bool pullupActive = true)
    {
        button = OneButton(pin, activeLow, pullupActive);
        button.attachLongPressStart(function);
        isActive = false;
    }
};

enum Switch {
    LEFT_WING_SW_LEFT,
    LEFT_WING_SW_RIGHT,
    RIGHT_WING_SW_LEFT,
    RIGHT_WING_SW_RIGHT,
    PUMP_SW_LEFT,
    PUMP_SW_RIGHT,
    BTN_VALVE,
    BTN_AUTO_PUMP,
    BTN_RESET_AUTO
};

Button buttons[] = {
    Button(LEFT_WING_SW_1, leftWingLeftTrigger),
    Button(LEFT_WING_SW_2, leftWingRightTrigger),
    Button(RIGHT_WING_SW_1, rightWingLeftTrigger),
    Button(RIGHT_WING_SW_2, rightWingRightTrigger),
    Button(PUMP_SW_1, pumpLeftTrigger),
    Button(PUMP_SW_2, pumpRightTrigger),
    Button(PUSH_BTN_VALVE, pushBtnValveTrigger, false, false),
    Button(PUSH_BTN_AUTO_PUMP, pushBtnAutoPumpTrigger, false, false),
    Button(PUSH_BTN_RESET_AUTO, pushBtnResetAutoTrigger)
};

void initializeLEDs()
{
    for (int i = LEFT_WING_LEFT_LED; i <= PUMP_ON_LED; i+=2) pinMode(i, OUTPUT);
}

void tickButtons() {
    for (int i = 0; i < NUM_OF_BUTTONS; i++) buttons[i].button.tick();
}

void getButtonStatuses(char* buffer)
{
    for (int i = 0; i < NUM_OF_BUTTONS; i++)
    {
        buffer[i] = buttons[i].isActive ? 'H' : 'L';
    }
}

void leftWingLeftTrigger()
{
    if (buttons[LEFT_WING_SW_RIGHT].isActive)
    {
        digitalWrite(LEFT_WING_RIGHT_LED, LOW);
        buttons[LEFT_WING_SW_RIGHT].isActive = false;
    }
    digitalWrite(LEFT_WING_LEFT_LED, HIGH);
    buttons[LEFT_WING_SW_LEFT].isActive = true;
    playSwitchOff();
};

void leftWingRightTrigger()
{
    if (buttons[LEFT_WING_SW_LEFT].isActive)
    {
        digitalWrite(LEFT_WING_LEFT_LED, LOW);
        buttons[LEFT_WING_SW_LEFT].isActive = false;
    }
    digitalWrite(LEFT_WING_RIGHT_LED, HIGH);
    buttons[LEFT_WING_SW_RIGHT].isActive = true;
    playSwitchOn();
};

void rightWingLeftTrigger()
{
    if (buttons[RIGHT_WING_SW_RIGHT].isActive)
    {
        digitalWrite(RIGHT_WING_RIGHT_LED, LOW);
        buttons[RIGHT_WING_SW_RIGHT].isActive = false;
    }
    digitalWrite(RIGHT_WING_LEFT_LED, HIGH);
    buttons[RIGHT_WING_SW_LEFT].isActive = true;
    playSwitchOff();
};

void rightWingRightTrigger()
{
    if (buttons[RIGHT_WING_SW_LEFT].isActive)
    {
        digitalWrite(RIGHT_WING_LEFT_LED, LOW);
        buttons[RIGHT_WING_SW_LEFT].isActive = false;
    }
    digitalWrite(RIGHT_WING_RIGHT_LED, HIGH);
    buttons[RIGHT_WING_SW_RIGHT].isActive = true;
    playSwitchOn();
};

void pumpLeftTrigger()
{
    if (buttons[PUMP_SW_RIGHT].isActive)
    {
        digitalWrite(PUMP_ON_LED, LOW);
        buttons[PUMP_SW_RIGHT].isActive = false;
    }
    digitalWrite(PUMP_OFF_LED, HIGH);
    buttons[PUMP_SW_LEFT].isActive = true;
    playSwitchOff();
};

void pumpRightTrigger()
{
    if (buttons[PUMP_SW_LEFT].isActive)
    {
        digitalWrite(PUMP_OFF_LED, LOW);
        buttons[PUMP_SW_LEFT].isActive = false;
    }
    digitalWrite(PUMP_ON_LED, HIGH);
    buttons[PUMP_SW_RIGHT].isActive = true;
    playSwitchOn();
};

void pushBtnAutoPumpTrigger()
{
    if(buttons[BTN_AUTO_PUMP].isActive) {
        digitalWrite(AUTO_PUMP_ON_LED, LOW);
        playButtonOff();
    } else {
        digitalWrite(AUTO_PUMP_ON_LED, HIGH);
        playButtonOn();
    }

    buttons[BTN_AUTO_PUMP].isActive = !buttons[BTN_AUTO_PUMP].isActive;
};

void pushBtnValveTrigger()
{
    if(buttons[BTN_VALVE].isActive) {
        digitalWrite(VALVE_ON_LED, LOW);
        playButtonOff();
    } else {
        digitalWrite(VALVE_ON_LED, HIGH);
        playButtonOn();
    }

    buttons[BTN_VALVE].isActive = !buttons[BTN_VALVE].isActive;
};

void pushBtnResetAutoTrigger()
{
    buttons[BTN_RESET_AUTO].isActive = true;
    digitalWrite(AUTO_ON_LED, HIGH);
    playReset();
};