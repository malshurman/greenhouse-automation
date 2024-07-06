#pragma once

#include <LiquidCrystal_I2C.h>

class LCDModule {
public:
    LCDModule(const LCDModule&) = delete;
    LCDModule& operator=(const LCDModule&) = delete;

    static LCDModule& getInstance();

    static void writeToLCD(int column, int row, const char* message);
private:
    LCDModule();
    LiquidCrystal_I2C lcd;
};