#include <ButtonManager.h>
#include <TaskManager.h>

#define BUTTON_TICK_RATE 10

static Array<Button*, BUTTON_COUNT_MAX> buttons;

Array<Button*, BUTTON_COUNT_MAX> getButtons() {
    return buttons;
}

static void tickAllButtons() {
    for (Button* button : getButtons()) {
        button->tick();
    }
}

ButtonManager& ButtonManager::getInstance() {
    static ButtonManager instance;
    return instance;
}

ButtonManager::ButtonManager() {
    TaskManager::getInstance().createTask(BUTTON_TICK_RATE, tickAllButtons);
}

void ButtonManager::registerButton(Button* button) {
    buttons.push_back(button);
}

Array<Button*, BUTTON_COUNT_MAX> ButtonManager::getAllButtons() {
    return buttons;
}