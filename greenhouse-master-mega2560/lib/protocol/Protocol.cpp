#include <Protocol.h>
#include <Buttons.h>
#include <Ticker.h>
#include <Pump.h>
#include <inttypes.h>

const int BUTTON_BUFFER_SIZE = 10;
const int AUTO_PUMP_BUFFER_SIZE = 21;
const int ARRAY_SIZE = BUTTON_BUFFER_SIZE + AUTO_PUMP_BUFFER_SIZE + 2; // Include start and stop characters
const int RETRANSMISSION_TIMEOUT = 100; // Timeout after 1 second

const char START_CHAR = '>';
const char STOP_CHAR = '<';
const char ACK = 'A';
const char NAK = 'N';

char buttonBuffer[BUTTON_BUFFER_SIZE];
char autoPumpBuffer[AUTO_PUMP_BUFFER_SIZE];
bool shouldToSendData(true);

void trySendData();
void tryReceiveData();
void resetState();


void initializeCommunicationProtocol()
{
    resetState();
}

void loopCommunicationProtocol()
{
    if (shouldToSendData)
    {
        trySendData();
    } else {
        tryReceiveData();
    }
}

void trySendData()
{
    Serial.write(START_CHAR);
    Serial.write(getButtonStatuses(), BUTTON_BUFFER_SIZE - 1);
    uint32_t autoPumpStartTime = getAutoPumpStartTime();
    uint32_t autoPumpEndTime = getAutoPumpEndTime();

    // sprintf(autoPumpBuffer, "%010u%010u", autoPumpStartTime, autoPumpEndTime);
    sprintf(autoPumpBuffer, "%010lu%010lu", autoPumpStartTime, autoPumpEndTime);
    Serial.write(autoPumpBuffer, AUTO_PUMP_BUFFER_SIZE - 1);

    Serial.write(STOP_CHAR);

    // Wait for ACK or NAK
    unsigned long startTime = millis();
    while (!Serial.available())
    {
        if (millis() - startTime > RETRANSMISSION_TIMEOUT)
        {
            return;
        }
    }

    char response = Serial.read();
    if (response == NAK) // NAK received, retransmit
    {
        resetState();
    }
}

void tryReceiveData() {
    char incomingData[ARRAY_SIZE];
    int index = 0;
    bool startCharFound = false;
    while(Serial.available() && index < ARRAY_SIZE) {
        char receivedChar = Serial.read();
        if(receivedChar == START_CHAR) {
            startCharFound = true;
        } else if(receivedChar == STOP_CHAR && startCharFound) {
            // Complete message received, send ACK
            Serial.write(ACK);
            Serial.write(incomingData, BUTTON_BUFFER_SIZE);
            return;
        } else if(startCharFound) {
            // Add received character to buffer
            incomingData[index++] = receivedChar;
        }
    }
    // Incomplete or corrupted message received, send NAK
    Serial.write(NAK);

    resetState();
}

void resetState() {
    shouldToSendData = true;
    memset(buttonBuffer, 0, sizeof(buttonBuffer));
    memset(autoPumpBuffer, 0, sizeof(autoPumpBuffer));
}