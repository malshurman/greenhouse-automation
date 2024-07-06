#include <LCDModule.h>
#include <Arduino.h>

LCDModule& LCDModule::getInstance()
{
    static LCDModule instance;
    return instance;
}

LCDModule::LCDModule() : lcd(0x27,20,4) {
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void LCDModule::writeToLCD(int column, int row, const char* message)
{
    getInstance().lcd.setCursor(column, row);
    getInstance().lcd.print(message);
}