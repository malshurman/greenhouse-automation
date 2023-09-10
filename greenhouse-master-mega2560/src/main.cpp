#include <Arduino.h>
#include <Pins.h>
#include <SoundPlayer.h>
#include <Controls.h>
#include <LCDModule.h>
#include <RTCModule.h>
#include <DHTModule.h>

#define BAUDRATE 9600

void setup ()
{
    Serial.begin(BAUDRATE);

    initializeSpeaker(SPEAKER);
    initializeRTC();
    initializeLCD();
    initializeDHT();
    initializeControls();

    playTheLick();
}

void loop()
{
    updateControls();
    updateLCD();
}