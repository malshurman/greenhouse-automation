#pragma once

#include <Button.h>

class ManualMode {
public:
    ManualMode(const ManualMode&) = delete;
    ManualMode& operator=(const ManualMode&) = delete;

    static ManualMode& getInstance();
    static void toggleResetButton();

    bool isAutoOn();
    void turnOnAuto();
    void turnOffAuto();
private:
    ManualMode();
    Button resetButton;
};