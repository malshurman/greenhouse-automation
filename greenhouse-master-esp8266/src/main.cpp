#include <Arduino.h>
#include <protocol.h>

#define BAUDRATE 115200

void setup() {
	Serial.begin(BAUDRATE);
	delay(1000);
}

void loop() {
	loopCommunicationProtocol();
	delay(10);
}
