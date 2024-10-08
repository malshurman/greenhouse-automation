#pragma once

#include <Button.h>

class Valve {
public:
    Valve(const Valve&) = delete;
    Valve& operator=(const Valve&) = delete;

    static Valve& getInstance();
    static void toggle();

    bool isOpen();

    void reset();
private:
    Valve();
    Button valveButton;
};