#include <Arduino.h>
#include <Protocol.h>

#define BAUDRATE 115200

void setup() {
	Serial.begin(BAUDRATE);
	initializeCommunicationProtocol();
	delay(1000);
}

void loop() {
    updateCommunication();
	delay(500);
}
