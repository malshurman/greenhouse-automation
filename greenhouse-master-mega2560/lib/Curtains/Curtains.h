#pragma once

#include <Switch.h>

class Curtains {
public:
    enum class State {
        ROLLED_UP,
        ROLLED_DOWN
    };

    Curtains(const Curtains&) = delete;
    Curtains& operator=(const Curtains&) = delete;

    static Curtains& getInstance();

    static void leftCurtainUp();
    static void leftCurtainDown();
    static void rightCurtainUp();
    static void rightCurtainDown();

    bool isLeftCurtainUp();
    bool isRightCurtainUp();

    void reset();

private:
    Curtains();

    Switch leftCurtainSwitch;
    Switch rightCurtainSwitch;
};