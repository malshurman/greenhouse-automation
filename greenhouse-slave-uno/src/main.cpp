#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>
#include <RainModule.h>

#define RAIN_MODULE_PIN A0

RTC_DS3231 rtc;

void initializeRTC();

void setup () {
  Serial.begin(9600);
}

void loop () {
	RainReading range = parseRainModuleReading(RAIN_MODULE_PIN);
  
  Serial.println(range);

  delay(1);  // delay between reads
}

void initializeRTC() {
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
}