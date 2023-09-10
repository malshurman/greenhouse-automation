#include <RTCModule.h>

RTC_DS3231 rtc;

void initializeRTC()
{
    Serial.println("Initializing RTC...");
    if(!rtc.begin()) {
        Serial.println("Couldn't find RTC!");
    }

    if(rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    rtc.disable32K();
    rtc.writeSqwPinMode(DS3231_OFF);
    rtc.disableAlarm(1);
    rtc.disableAlarm(2);

    Serial.print("Initialized RTC!");
}

DateTime getCurrentDateTime()
{
    return rtc.now();
}