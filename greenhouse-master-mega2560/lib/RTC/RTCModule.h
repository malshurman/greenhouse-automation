#pragma once

#include <RTClib.h>

class RTCModule {
public:
    RTCModule(const RTCModule&) = delete;
    RTCModule& operator=(const RTCModule&) = delete;

    static RTCModule& getInstance();

    void writeDateAndTimeToLCD();
    DateTime getCurrentDateTime();
private:
    RTCModule();
    RTC_DS3231 rtc;
};