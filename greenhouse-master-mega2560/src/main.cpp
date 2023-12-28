#include <Arduino.h>
#include <Pins.h>
#include <SoundPlayer.h>
#include <Buttons.h>
#include <LCDModule.h>
#include <RTCModule.h>
#include <DHTModule.h>
#include <Protocol.h>

#define BAUDRATE 9600

void setup ()
{
    Serial.begin(BAUDRATE);

    initializeSpeaker(SPEAKER);
    initializeRTC();
    initializeLCD();
    initializeDHT();
    initializeButtons();

    // playTheLick();
}

void loop()
{
    // updateButtons();
    // updateLCD();
    // loopCommunicationProtocol();

    char buffer[9];
    getButtonStatuses(buffer);

    for (int i = 0; i < 9; i++)
    {
        Serial.print(buffer[i]);
    }

    Serial.println();

    // for (int i = 0; i < 9; i++)
    // {
    //     Serial.print(buffer[i]);
    // }
    
    // Serial.println();
    delay(500);
}