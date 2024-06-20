#include <Buttons.h>
#include <OneButton.h>
#include <Pins.h>
#include <SoundPlayer.h>
#include <Pump.h>

#define DEFAULT_BTN_LONG_PRESS_MS 500
#define RESET_BTN_LONG_PRESS_MS 3000
#define PUMPING_TASK_DURATION_MINUTES 10
#define CONTROLS_BUFFER_SIZE 10

enum SoundSetting {
    SOUND,
    SILENT
};

void leftWingOffTrigger();
void leftWingOnTrigger();
void rightWingOffTrigger();
void rightWingOnTrigger();
void pumpOffTrigger();
void pumpOnTrigger();
void valveTrigger();
void autoPumpTrigger();
void resetTrigger();

void disableAuto();

struct Button {
    OneButton button;
    const int ledPin;
    const Melody onSound;
    const Melody offSound;
    bool isToggled;

    Button(const int pin, const int ledPin, callbackFunction function, const bool activeLow = true, const bool pullupActive = true, const int longPressTime = DEFAULT_BTN_LONG_PRESS_MS, const Melody offSound = buttonOff, const Melody onSound = buttonOn)
        : ledPin(ledPin), onSound(onSound), offSound(offSound)
    {
        button = OneButton(pin, activeLow, pullupActive);
        button.setPressTicks(longPressTime);
        button.attachLongPressStart(function);
        pinMode(ledPin, OUTPUT);
        isToggled = false;
    }

    void toggle() {
        isToggled ? toggleOff() : toggleOn();
    }

    void toggleOn(const SoundSetting soundSetting = SOUND) {
        if (isToggled)
        {
            if (soundSetting == SOUND) playMelody(feedback);
            return;
            
        }
        
        isToggled = true;
        digitalWrite(ledPin, HIGH);
        if (soundSetting == SOUND) playMelody(onSound);
    }

    void toggleOff(const SoundSetting soundSetting = SOUND) {
        if (!isToggled)
        {
            if (soundSetting == SOUND) playMelody(feedback);
            return;
            
        }

        isToggled = false;
        digitalWrite(ledPin, LOW);
        if (soundSetting == SOUND) playMelody(offSound);
    }

    void tick() {
        button.tick();
    }
};

struct Switch {
    Button left;
    Button right;

    Switch(const int offPin, const int offLedPin, const int onPin, const int onLedPin, callbackFunction offFunction, callbackFunction onFunction)
        : left(offPin, offLedPin, offFunction), right(onPin, onLedPin, onFunction) {}

    void switchOff(const SoundSetting soundSetting = SOUND) {
        left.toggleOn(SILENT);
        right.toggleOff(soundSetting);
    }

    void switchOn(const SoundSetting soundSetting = SOUND) {
        left.toggleOff(SILENT);
        right.toggleOn(soundSetting);
    }
    
    bool isOn() {
        return right.isToggled;
    }

    void tick() {
        left.tick();
        right.tick();
    }
};

Switch leftWingSwitch(LEFT_WING_SW_1, LEFT_WING_LEFT_LED, LEFT_WING_SW_2, LEFT_WING_RIGHT_LED, leftWingOffTrigger, leftWingOnTrigger);
Switch rightWingSwitch(RIGHT_WING_SW_1, RIGHT_WING_LEFT_LED, RIGHT_WING_SW_2, RIGHT_WING_RIGHT_LED, rightWingOffTrigger, rightWingOnTrigger);
Switch pumpSwitch(PUMP_SW_1, PUMP_OFF_LED, PUMP_SW_2, PUMP_ON_LED, pumpOffTrigger, pumpOnTrigger);
Button valve(PUSH_BTN_VALVE, VALVE_ON_LED, valveTrigger, false, false);
Button autoPump(PUSH_BTN_AUTO_PUMP, AUTO_PUMP_ON_LED, autoPumpTrigger, false, false);
Button resetAuto(PUSH_BTN_RESET_AUTO, AUTO_ON_LED, resetTrigger, true, true, RESET_BTN_LONG_PRESS_MS, reset, reset);

char controlsBuffer[CONTROLS_BUFFER_SIZE];

void initializeButtons()
{
    resetAuto.toggleOn(SILENT);
    leftWingSwitch.switchOff(SILENT);
    rightWingSwitch.switchOff(SILENT);
    pumpSwitch.switchOff(SILENT);
    valve.toggleOff(SILENT);
    autoPump.toggleOff(SILENT);
}

void updateControls()
{
    leftWingSwitch.tick();
    rightWingSwitch.tick();
    pumpSwitch.tick();
    valve.tick();
    autoPump.tick();
    resetAuto.tick();
}

void leftWingOffTrigger()
{
    leftWingSwitch.switchOff();
    disableAuto();
}

void leftWingOnTrigger()
{
    leftWingSwitch.switchOn();
    disableAuto();
}

void rightWingOffTrigger()
{
    rightWingSwitch.switchOff();
    disableAuto();
}

void rightWingOnTrigger()
{
    rightWingSwitch.switchOn();
    disableAuto();
}

void pumpOffTrigger()
{
    pumpSwitch.switchOff();
    autoPump.toggleOff(SILENT);
    turnPumpOff();
    turnAutoPumpOff();
    disableAuto();
}

void pumpOnTrigger()
{
    pumpSwitch.switchOn();
    autoPump.toggleOff(SILENT);
    turnPumpOn();
    turnAutoPumpOff();
    disableAuto();
}

void valveTrigger()
{
    valve.toggle();
    disableAuto();
}

void autoPumpTrigger()
{
    autoPump.toggle();
    if (autoPump.isToggled)
    {
        pumpSwitch.switchOn(SILENT);
        turnPumpOn();
        turnAutoPumpOn();
    }
    else
    {
        pumpSwitch.switchOff(SILENT);
        turnPumpOff();
        turnAutoPumpOff();

    }
    disableAuto();
}

void resetTrigger()
{
    resetAuto.toggleOn();
    leftWingSwitch.switchOff(SILENT);
    rightWingSwitch.switchOff(SILENT);
    pumpSwitch.switchOff(SILENT);
    valve.toggleOff(SILENT);
    autoPump.toggleOff(SILENT);
    turnPumpOff();
    turnAutoPumpOff();
}

void disableAuto()
{
    resetAuto.toggleOff(SILENT);
}

void finishAutoPump()
{
    pumpOffTrigger();
}

char* getButtonStatuses()
{
    controlsBuffer[0] = leftWingSwitch.left.isToggled ? 'H' : 'L';
    controlsBuffer[1] = leftWingSwitch.right.isToggled ? 'H' : 'L';
    controlsBuffer[2] = rightWingSwitch.left.isToggled ? 'H' : 'L';
    controlsBuffer[3] = rightWingSwitch.right.isToggled ? 'H' : 'L';
    controlsBuffer[4] = pumpSwitch.left.isToggled ? 'H' : 'L';
    controlsBuffer[5] = pumpSwitch.right.isToggled ? 'H' : 'L';
    controlsBuffer[6] = valve.isToggled ? 'H' : 'L';
    controlsBuffer[7] = autoPump.isToggled ? 'H' : 'L';
    controlsBuffer[8] = resetAuto.isToggled ? 'H' : 'L';
    controlsBuffer[9] = '\0';
    return controlsBuffer;
}