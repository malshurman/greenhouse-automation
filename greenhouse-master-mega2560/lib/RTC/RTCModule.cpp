#include <RTCModule.h>
#include <LCDModule.h>
#include <TaskManager.h>

#define LCD_REFRESH_RATE 250

static char dateTimeBuffer[21];

bool isDST(DateTime now);

static void writeDateAndTimeToLCDTask() {
    RTCModule::getInstance().writeDateAndTimeToLCD();
}

RTCModule& RTCModule::getInstance() {
    static RTCModule instance;
    return instance;
}

RTCModule::RTCModule() {
    if(!rtc.begin()) {
        Serial.println("Couldn't find RTC!");
    }

    if(rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        if (isDST(rtc.now()))
        {
            rtc.adjust(DateTime(rtc.now().unixtime() - 3600)); // Because we added an hour extra
        }
    }

    rtc.disable32K();
    rtc.writeSqwPinMode(DS3231_OFF);
    rtc.disableAlarm(1);
    rtc.disableAlarm(2);

    TaskManager::getInstance().createTask(LCD_REFRESH_RATE, writeDateAndTimeToLCDTask);
}

DateTime RTCModule::getCurrentDateTime() {
    DateTime now = rtc.now();
    if (isDST(now)) {
        // If it's within DST, add one hour
        return DateTime(now.unixtime() + 3600);
    }
    return now; // Return unmodified time if not in DST
}

bool isDST(DateTime now) {
    // DST starts on the last Sunday in March
    int beginDSTDay = (31 - (5 * now.year() / 4 + 4) % 7);
    DateTime beginDST(now.year(), 3, beginDSTDay, 2, 0, 0);

    // DST ends on the last Sunday in October
    int endDSTDay = (31 - (5 * now.year() / 4 + 1) % 7);
    DateTime endDST(now.year(), 10, endDSTDay, 3, 0, 0);

    // Check if current time is within the DST period
    return now.unixtime() >= beginDST.unixtime() && now.unixtime() < endDST.unixtime();
}

void RTCModule::writeDateAndTimeToLCD() {
    DateTime date = getCurrentDateTime();

    sprintf(dateTimeBuffer,"%02u-%02u-%04u  %02u:%02u:%02u", date.day(), date.month(), date.year(), date.hour(), date.minute(), date.second());

    LCDModule::getInstance().writeToLCD(0, 0, dateTimeBuffer);
}