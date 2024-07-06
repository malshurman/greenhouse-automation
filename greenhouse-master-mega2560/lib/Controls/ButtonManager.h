#pragma once

#include <Button.h>
#include <Array.h>

const int BUTTON_COUNT_MAX = 10;

class ButtonManager {
public:
    ButtonManager(const ButtonManager&) = delete;
    ButtonManager& operator=(const ButtonManager&) = delete;

    static ButtonManager& getInstance();

    void registerButton(Button* button);
    Array<Button*, BUTTON_COUNT_MAX> getAllButtons();
private:
    ButtonManager();
};